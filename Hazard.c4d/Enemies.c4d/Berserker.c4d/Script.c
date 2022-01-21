/*--Berserker --*/

#strict

/* Initialisierung */

protected func Initialize()
{
  SetAction("Walk");
  SetDir(Random(2));
  // Gruppen-KI
  AddEffect("GroupKI", this(), 1, 0, this());
  // Schadenseffekt
  AddEffect("DmgCheck",this(),1,0);
}

/* Aktivität */

protected func Activity()
{
  // Tot?
  if(!GetAlive()) return();
  // Gerade verhindert?
  if(!Ready()) return();

  // Rumangreifen
  if(ReadyToAttack()) {
    // Nur wenn möglich
    for(var prey in FindTargets(this, 200)) {
        Attack(prey);
		return();
	}
  }

  // Kommando?
  if(GetCommand()) return();

  // Brüllen!
  if(!Random(500)) return(Roar());

  // Rumlaufen
  if(GetAction() eq "Walk")
    {
    if(Random(5)) return();
    if(Random(2)) SetComDir(COMD_Left());
    else SetComDir(COMD_Right());
    }
}

public func Ready() // Wenn das Alien gerade nichts tun kann
{
  if(GetAction() eq "Jump") return();
  if(GetAction() eq "KneelDown") return();
  if(GetAction() eq "Death") return();

  return(1);
}

/* Aktionen */

// Unglaublich schrecklicher Schrei!
public func Roar()
{
  if(GetAction() eq "Roar") return();
  SetAction("Roar");
  Sound("BerserkerRoar");
}

// Angreifen oder Hinlaufen
public func Attack(object pPrey, bool noKI)
{
  // Gruppe benachrichtigen
  if(!noKI)
    GroupAttack(pPrey);
  // Geth nicht?!
  if(!ReadyToAttack()) return();
  // Nah genug zum krassen Angriff?
  if(ObjectDistance(pPrey) <= 64)
    // RAAR!
    return(ClawAttack(pPrey));
  // Hinrennen
  SetCommand(this(), "MoveTo", pPrey);
}

// Klauenangriff
private func ClawAttack(object pPrey)
{
  // Puh, jetzt wirds kompliziert. Richtige Attackanimation auswählen :D
  
  // Ist oben Platz?
  if(!GBackSolid(0,-60))
    // Wir haben genug Platz für alle Angriffe. Random!
    SetAction(Format("Attack%d",Random(3)+1)); // *hax*
    
  // Genug Platz für Angriff von der Seite? 
  else if(!GBackSolid(0,-45))
    // Attack! :D
    SetAction(Format("Attack%d",Random(2)+2));
    
  // Okay, bleibt nur noch Beißen. >:(
  else
    SetAction("Attack3");

  // Geräusch
  Sound("BerserkerAttack");
  Sound("JumperBite");
}

// Beißen
private func EndAttack()
{
  // Feind suchen
  var pPrey;
  for(pPrey in FindTargets(this,64)) { break; }
  // Kein Feind
  if(!pPrey) return();
  // Hauen!!1
  DoDmg(35+Random(50)*GetCon()/100, DMG_Melee, pPrey);
  Sound("BerserkerStab");
  // Hacksperre, damit man nicht dauerhackt
  AddEffect("NoAttack", this(), 99, 30, this());
}

public func Birth()
{
  // Miniwinzig!
  SetCon(50);
}

protected func Death(int iKilledBy)
{
  // Sterben
  SetAction("Death");
  Sparks(20, RGBa(25, 255, 25, 20));
  // Tönen!
  Sound("BerserkerDeath");
  GameCallEx("OnAlienDeath", this, iKilledBy);
}

func CheckLiquid()
{
  if(!GBackLiquid(0,-3))
    return();
  if(GetMaterial(0,-3) == Material("Water"))
  {
    ExtractMaterialAmount(0,0, Material("Water"), GetMass());
    CastPXS("Acid",GetMass(),GetMass());
    Death(-1);
    FadeOut();
  }
}

/* Gruppen-KI */

protected func GroupAttack(object pPrey)
{
  // Andere Aliens benachrichtigen
  for(var alien in FindObjects(Find_Distance(100), Find_Func("IsAlien"), Find_Exclude(this())))
    if(!CheckEnemy(alien))
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

public func OnHit(int iDmg, int iType, object pFrom) {
  if(iType == DMG_Fire)
    AddFireEffect(this(),(5*iDmg*(105))/100);
}

/* Status */

public func ReadyToAttack()
{
  if(GetEffect("NoAttack", this())) return();

  return(GetAction() eq "Walk");
}

public func IsAlien() { return(GetAction() ne "Death"); }
public func IsBulletTarget() { return(true); }

/* Effekte */

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
