/*--Springer --*/

#strict

local glob;
local lure; //lauern!

/* Initialisierung */

protected func Initialize()
{
  SetAction("Walk");
  SetDir(Random(2));
  // Gruppen-KI
  AddEffect("GroupKI", this(), 1, 0, this());
  // Schadenseffekt
  AddEffect("DmgCheck",this(),1,0);
  
  Need4Speed();
}

protected func Need4Speed()
{
	SetPhysical("Walk", 90000 + RandomX(-10,10)*1000, PHYS_Temporary);
	ScheduleCall(this(), "Need4Speed", Random(200)+100);
}

/* Aktivität */

protected func Activity()
{
  // Gerade verhindert?
  if(!Ready()) return();

  // Rumangreifen
  if(ReadyToBite())
  {
    var l = lure+1;
    // Nur wenn möglich
    for(var prey in FindTargets(this, 150/l))
          // Hinrennen / anspringen
          return(Attack(prey));
  }

	// Wir machen auch mal Zufallsjump! Aber Hechtjumplike!
	if(!Random(31))
	{
		Jump();
		SetYDir(GetYDir()/2);
	}

  // Kommando?
  if(GetCommand()) return();

  // Schleimglobs nehmen
  if(!glob)
  {
    var goodglob = FindObject2(Find_ID(GLOB), Find_InRect(-15, -5, 30, 10), Find_Action("Globbing"));
    if(goodglob)
    {
      glob = goodglob;
      glob->SetAction("AGlobbing", this());
    }
  }
  else
  {
    if(GetAction() eq "Scale")
      glob->SetVertexXY(0, 0, -9*(GetDir()*2-1));
    else
      glob->SetVertexXY(0, -9*(GetDir()*2-1), 3);
  }

  if(IsLuring())
    return();
  
  //Auflauern!
  if(!GetEffect("JumpedToTop",this()))
  if(!Random(50) && (GetAction() S= "Hangle" || GetAction() S= "Scale") && GetActTime() > 100)
    if(GetLength(FindObjects(Find_Func("IsAlien"),Find_Func("IsLuring"),Find_InRect(-150, -150, 300, 300))) < 3)
    {
      //Sind wir eh nicht zu weit über dem Boden? Sonst finden wir ja nie Beute.
      for(var x=1; x <= 5; x++)
        if(GBackSolid(0,10*x-1))
        {
          StartLuring();
          break;
        }
      //Wir wollen ja nicht IRGENDWO rumklettern wo uns kein Mensch findet :(
      if(!IsLuring())
        SetAction("Walk");
    }
  
  // Rumlaufen, aber nicht zuviel wechseln, sonst sieht es so aus, als wäre er verwirrt!
  if(!Random(10)) {
    if(GetAction() eq "Walk" || GetAction() eq "Hangle")
      {
      if(Random(2)) SetComDir(COMD_Left());
      else SetComDir(COMD_Right());
      }
    if(GetAction() eq "Scale")
      {
      if(Random(2)) SetComDir(COMD_Up());
      else SetComDir(COMD_Down());
      }
  }
  // Niedrige Decke in der nähe? noch wenige kletternde?
  if(!Random(10)) {
    var aliencount = GetLength(FindObjects(Find_ID(GetID()),Find_InRect(-150, -50, 300, 100)));
    var hanglingaliens = GetLength(FindObjects(Find_ID(GetID()),Find_InRect(-150, -50, 300, 100),Find_Action("Hangle")));
    if(hanglingaliens < aliencount/2) {
      // An die Decke springen!
			JumpToTop();
    }
  }
}

private func JumpToTop() {
  var dist;
  for(dist=0; dist < 80; dist += 2)
    if(GBackSolid(0,-dist))
    {
      SetAction("Walk"); //teh hax!
      Jump();
      SetXDir(GetXDir()/4);
      SetYDir(AdjustSpeed(-dist));
      //Erfolgreich gesprungen!
      return(true);
    }
    
  return(false);
}

public func Ready() // Wenn das Alien gerade nichts tun kann
{
  if(GetAction() eq "Jump") return();

  return(1);
}

/* Aktionen */

// Angreifen oder Hinlaufen
public func Attack(object pPrey, bool noKI)
{
  // Geth nicht?!
  if(!ReadyToBite()) return();
  if(!pPrey) return();
  if(pPrey == this) return();

  if(GetX(pPrey)-GetX())
    SetDir(DIR_Right);
  else
    SetDir(DIR_Left);

  //Walljumpattack?
  if(GetEffect("CoolWalljumpAttack",this()))
  {
    RemoveEffect("CoolWalljumpAttack",this());
    return(JumpTo(pPrey));
  }

  // Gruppe benachrichtigen
  if(!noKI)
    GroupAttack(pPrey);
  
  // ZU nah?
  if(ObjectDistance(pPrey) < 30 && GetAction() ne "Hangle")
  {
    // Beißen!
    Bite(pPrey);
    // und etwas Distanz gewinnen
    SetAction("Walk");
    Jump();
    SetXDir((GetDir()*2-1)*10);
    SetYDir(-10);
  }
  
  // Nah genug zum krassen Angriff?
  if(ObjectDistance(pPrey) <= 100)
  {
    //kleiner Gegner, Decke da oder einfach nur Random? :D
    if(GetAction() eq "Walk" && (GetY(pPrey)-GetY() > 5 || !Random(3)) && Abs(GetX(pPrey)-GetX()) > 50)
    {
      if(!JumpToTop())
        return(JumpTo(pPrey));
      else
      {
        AddEffect("CoolWalljumpAttack",this(),200);
      
        return();
      }
    }
    else
      return(JumpTo(pPrey));
  }
  // Hinrennen
  if(!IsLuring())
    SetCommand(this(), "MoveTo", pPrey);
}

// Sprungangriff
private func JumpTo(object pPrey)
{
  if(IsLuring()) {
    StopLuring();
  }
  // Spezialangriff: Schleim ballern
  if(glob) return(GlobTo(pPrey));
  // Springen
  SetAction("Walk"); // *hax*
  Jump();
  // Geschwindigkeit anpassen
  SetXDir(AdjustSpeed(GetX(pPrey)-GetX()));
  SetYDir(AdjustSpeed(GetY(pPrey)-GetY()));
  // Geräusch
  Sound("JumperAttack");
  // Effekt dranfügen
  if(!GetEffect("Bite", this())) AddEffect("Bite", this(), 99, 1, this());
}

// Hilfsfunktion um die Sprunggeschwindigkeit anzupassen
private func AdjustSpeed(int iSpeed)
{
  // Richtung speichern
  var dir = iSpeed / Abs(iSpeed);
  // Min- und Maxwerte anpassen
  iSpeed = BoundBy(Abs(iSpeed), 20, 55);
  return(iSpeed*dir*GetCon()/100);
}

// Beißen
private func Bite(object pPrey)
{
  // Beißen!11
  DoDmg(3+Random(10)*GetCon()/100, DMG_Melee, pPrey);
  // Beißsperre, damit man nicht dauerhüpft
  AddEffect("NoBite", this(), 99, 50, this());
  Sound("JumperBite");
}

// Schleim schleudern
private func GlobTo(object pPrey)
{
  if(!glob) return();
  // Glob entfernen
  RemoveObject(glob);
  var sGlob;
  if(GetAction() eq "Scale")
     sGlob = CreateObject(SLST, 0, 18*(GetDir()*2-1), GetOwner());
  else
     sGlob = CreateObject(SLST, 18*(GetDir()*2-1), -3, GetOwner());
  // Feuern
  sGlob->Launch(AdjustSpeed(GetX(pPrey)-GetX()), AdjustSpeed(GetY(pPrey)-GetY()));
  // Beißpause
  AddEffect("NoBite", this(), 99, 25, this());
}

public func StartLuring()
{
  if(IsLuring())
    return();
  //wir lauern
  lure=1;
  //tarnen!
  FadeOut(this(),120);
  //nichstun und warten.
  SetCommand(0,"None");
  SetComDir(COMD_Stop);
}

public func StopLuring()
{
  //nicht mehr lauern
  lure = 0;
  //farbe!
  StopFading();
}

func StartHangling()
{
  // Machen wir gerade Walljump-Attacke?
  if(!GetEffect("CoolWalljumpAttack", this()))
    //nö, nichts tun.
    return();
    
  if(GetCommand())
    SetCommand(this(), "None");
  
}

public func Birth()
{
  // Volle Größe!
  SetCon(100);
}

protected func Death(int iByPlayer)
{
  // Zersplattern!
  //Sparks(10, RGBa(25, 255, 25, 20));

  //Test
  AlienSplatter(35);
  AlienBlood(30);
  
  // Quieken!
  Sound("JumperDeath");
  GameCallEx("OnAlienDeath", this, iByPlayer);
  // Verschwinden!
  RemoveObject();
}


/* Gruppen-KI */

protected func GroupAttack(object pPrey)
{
  // Andere Aliens benachrichtigen
  for(var alien in FindObjects(Find_Distance(100), Find_Func("IsAlien"), Find_OCF(OCF_Alive), Find_Exclude(this())))
    if(alien)
      if(!CheckTarget(alien,this))
        alien->~Attack(pPrey, true);
}

/* Schaden */

public func OnDmg(int iDmg, int iType) {
  //Melee
  if(iType == DMG_Melee)
    return(20);
  //Projectile
  if(iType == DMG_Projectile)
    return(0);
  //Flame
  if(iType == DMG_Fire)
    return(-50);
  //Explosion
  if(iType == DMG_Explosion)
    return(-10);
  //Energy
  if(iType == DMG_Energy)
    return(50);
  //Bio
  if(iType == DMG_Bio) {
    return(90);
  }
}

// Reaktion auf Schaden
func OnHit(int iDmg, int iType, object pFrom)
{
  if(iType == DMG_Fire)
    AddFireEffect(this(),(5*iDmg*(105))/100);
  // verzögert...
  ScheduleCall(this(), "OnHit2", 5, 0, pFrom);
}

protected func OnHit2(object by)
{
  //wir gehen mal davon aus, dass es ein Spieler war.
  var obj = GetCursor(GetController(by));
  //doch nicht?
  if(!obj)
    //nach feinden suchen und so
    return(Activity());
  
  if(ObjectDistance(obj,this()) < 200)
    Attack(obj);
}

func CheckLiquid()
{
  if(!GBackLiquid(0,-3))
    return();
  if(GetMaterial(0,-3) == Material("Water"))
  {
    ExtractMaterialAmount(0,0, Material("Water"), GetMass());
    CastPXS("Acid",GetMass()*5,GetMass());
    Death(-1);
  }
}

/* Status */

public func ReadyToBite()
{
  if(GetEffect("NoBite", this())) return();

  return(
    GetAction() eq "Walk" ||
    GetAction() eq "Scale" ||
    GetAction() eq "Hangle"
  );
}

public func IsAlien() { return(true); }
public func IsBulletTarget() { return(true); }
public func IsLuring() { return(lure); }

/* Effekte */

public func FxBiteTimer()
{
  // Nicht mehr im Sprung?
  if(ReadyToBite())
    {
    AddEffect("NoBite", this(), 99, 35, this());
    return(-1);
    }
  // Ist ein fressbares Objekt in der Nähe?
  var toBite;
  for(var prey in FindTargets(this, 11))
    {
      toBite = prey;
      break;
    }
  // Eines gefunden?
  if(toBite)
    {
    Bite(toBite);
    // Und weg
    return(-1);
    }
}

public func FxBiteEffect(string EffectName)
{
  // Nur ein Beiß-Effekt
  if(EffectName == "Bite") return(-1);
}

public func FxNoBiteEffect(string EffectName)
{
  // Kein Beiß-Effekt gerade
  if(EffectName == "Bite") return(-1);
}

public func FxGroupKIDamage(object target, int no, int iDmg, int iCause)
{
  // Angeschossen?
  if(iCause == 32 || iCause == 33 || iCause == 35)
    // Feinde in weiter Ferne suchen
    for(var prey in FindTargets(this,400))
          {
          Attack(prey);
          return(iDmg);
          }
  return(iDmg);
}

public func IgnoreFriendlyFire() { return(1); }
public func IsThreat() { return(true); }