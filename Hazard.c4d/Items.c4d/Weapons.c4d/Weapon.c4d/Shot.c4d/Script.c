#strict

/* Schuss */

local iTime, lx, ly, pTrail, iDamage, iPrec, iRefl, iTrailLength, iTrailSize, iXDir, iYDir;
local shooter; // Dingens/Clonk das den Schuss abgefeuert hat.
local bGlow;


func Construction(object byObj) {
  // nichts? :C
  if(!byObj)
    return();
  // Waffe?
  shooter = GetShooter(byObj);
  // Team?
  if(shooter->GetTeam())
  	SetTeam(shooter->GetTeam());
}

public func Initialize() {
  SetObjectBlitMode(1);
}

public func Launch(int iAngle, int iSpeed, int iDist, int iSize, int iTrail, int iDmg, int iDmgPrec, int iGlowSize, int iAPrec, int iReflections)
{
  // Standardwerte setzen
  if(!iSize)    iSize = 8;
  //if(!iGlowSize)
  //  iGlowSize = iSize;
  if(!iTrail)   iTrail = 300;
  if(!iDmg)     iDamage = 3;
  else iDamage = iDmg;

  // und zuweisen
  iSize = Min(iSize+2,GetDefWidth());
  iPrec = iDmgPrec;

  // Positionieren
  SetPosition(GetX(),GetY()+GetDefHeight()/2);

  DoCon(100*iSize/GetDefWidth()-100);

  lx = GetX();
  ly = GetY();

  iTime = 10*iDist/iSpeed;
  
  if(!iTime)
    return(RemoveObject());

  var self = this();
  SetAction("Travel");
  if(!self) return();   // Kleiner Sicherheitscheck, ob die Kugel nicht sofort verschwindet  

  SetXDir(+Sin(iAngle,iSpeed, iAPrec));
  SetYDir(-Cos(iAngle,iSpeed, iAPrec));
  SetR(+iAngle);

  // Trail erzeugen
  CreateTrail(iSize, iTrail);
  
  bGlow = false;
  // Tolles Leuchten erzeugen
  if(iGlowSize) {
    bGlow = true;
    SetGraphics(0,this(),LIGH,1,GFXOV_MODE_Base, 0, 1);
    SetObjDrawTransform(100+35*iGlowSize,0,0, 0,100+35*iGlowSize,0, this(),1);
    SetClrModulation(GlowColor(1),this(),1);
  }
  
  AddEffect("HitCheck", this(), 1,1, 0,GetID(),shooter);

  // Werte für Reflektionen speichern
  iRefl = iReflections;
  iTrailSize = iSize;
  iTrailLength = iTrail;
  iXDir = GetXDir(0, 100);
  iYDir = GetYDir(0, 100);

}

// Extern für Überladung
private func CreateTrail(int iSize, int iTrail) {
  pTrail = CreateObject(TRAI,0,0,-1);
  if(pTrail) {
    pTrail->Set(iSize-2,iTrail,this());
    SetObjectBlitMode(GetObjectBlitMode(),pTrail);
  }
}

/* Timer */

private func Traveling()
{
  SetPosition(GetX(), GetY(), pTrail);
  var iATime = GetActTime();

  // ausfaden
  SetClrModulation(Color(iATime));
  if(bGlow) {
    SetClrModulation(GlowColor(iATime),0,1);
  }
  // löschen
  if(iATime >= iTime) return(Remove());

  // außerhalb der Landschaft: löschen
  if(GetY()<0) return(Remove());
}
  
/* Treffer */

private func Hit()
{
  // Reflektion an Wänden (Eintrittswinkel = Austrittwinkel)
  if(iRefl)
  {
    if(iXDir != 0)
    {
      // Positive XDir -> Rechts auf Material prüfen
      if(iXDir > 0)
      {
        if(GBackSolid(1,0))
          iXDir *= -1;
      // Und andersrum das gleiche Spiel
      }
      else
        if(GBackSolid(-1,0))
          iXDir *= -1;
    }
    if(iYDir != 0)
    {
      // Positive YDir -> Unten auf Material prüfen
      if(iYDir > 0)
      {
        if(GBackSolid(0,1))
          iYDir *= -1;
      }
      // Und andersrum das gleiche Spiel
      else
        if(GBackSolid(0,-1))
          iYDir *= -1;
    }
    // Geschwindigkeit gehört neu gesetzt
    SetXDir(iXDir, 0, 100);
    SetYDir(iYDir, 0, 100);
    SetR(Angle(0, 0, iXDir, iYDir));
    // Trail wird entfernt!
    pTrail->SetPosition(GetX(), GetY());
    pTrail->SetSpeed(0,0);
    pTrail->Remove();
    // Und neu erzeugt
    CreateTrail(iTrailSize, iTrailLength);

    // Eine Reflektion weniger
    iRefl--;
    // Und erstmal nicht auftreffen
    return;
  }

  // umliegende Objekte beschädigen.
  var objs = FindObjects(	Find_AtPoint(),
							Find_NoContainer(),
							Find_Or(
								Find_Func("IsBulletTarget",GetID(),this(),shooter),
								Find_OCF(OCF_Alive)
							),
							Find_Func("CheckEnemy",this())
							/*Find_Not(Find_Func("HitExclude"))*/);  
 
  for(var pTarget in objs) {
    BulletStrike(pTarget);
  }

  HitObject();
}

private func HitObject(object pObject) {
  BulletStrike(pObject);
  Sparks(40*GetCon()/100,Color(GetActTime()));
  var x = iXDir/500;
  var y = iYDir/500;
  var mat = GetMaterial(x,y);
  if(CheckDust(mat))
  {
    var rand = Random(3);
    var r = GetMaterialColor(mat,rand,0);
    var g = GetMaterialColor(mat,rand,1);
    var b = GetMaterialColor(mat,rand,2);
    var alpha = 190;

    var xdir = -iXDir/500;
    var ydir = -iYDir/500;
    for(var i=0; i<4; ++i)
      CreateParticle("Dust",x,y,xdir+RandomX(-2,2),ydir+RandomX(-3,3),GetCon()*3,RGBa(r,g,b,alpha));
  }
  DigFree(GetX(),GetY(),GetCon()/10);
  Remove();
}

public func BulletStrike(object pObj) {
  if(pObj)
    DoDmg(iDamage,DMG_Projectile,pObj,iPrec);
}

private func Color(int iATime) {
  var iPrg = 100*iATime/iTime;
  return(RGBa(255,255-iPrg*2,255-iPrg*2,iPrg*2));
}

public func TrailColor(int iATime) {
  var iPrg = 100*iATime/iTime;
  return(RGBa(255,255-iPrg*2,255-iPrg*2,iPrg*2));
}

public func GlowColor(int iATime) {
  return(RGBa(255,190,0,50));
}

// Dummy-func
func HitExclude()
{

}

public func Remove() {
  if(pTrail) {
    pTrail->SetPosition(GetX(),GetY());
    pTrail->Remove();
  }

  RemoveObject();
}

/* Effekt für Trefferüberprüfung */

// EffectVars:
// 0 - alte X-Position
// 1 - alte Y-Position
// 2 - Schütze (Objekt, das die Waffe abgefeuert hat, üblicherweise ein Clonk)
// 3 - ID des Schützen
// 4 - Scharf? Wenn true wird der Schütze vom Projektil getroffen 
// 5 - niemals den Schützen treffen

public func FxHitCheckStart(object target, int effect, int temp, object byObj, bool neverShooter)
{
  if(temp) return();
  EffectVar(0, target, effect) = GetX(target);
  EffectVar(1, target, effect) = GetY(target);
  if(!byObj)
    byObj = target;
  if(byObj->Contained())
    byObj = (byObj->Contained());
  EffectVar(2, target, effect) = byObj;
  EffectVar(3, target, effect) = GetID(byObj);
  EffectVar(4, target, effect) = false;
  EffectVar(5, target, effect) = neverShooter;
}

public func FxHitCheckTimer(object target, int effect, int time)
{
  var obj;
  // Oh man. :O
  var oldx = EffectVar(0, target, effect);
  var oldy = EffectVar(1, target, effect);
  var newx = GetX(target);
  var newy = GetY(target);
  var dist = Distance(oldx, oldy, newx, newy);
  EffectVar(0, target, effect) = GetX(target);
  EffectVar(1, target, effect) = GetY(target);
  
  // Schuss schon Scharf?
  var exclude = EffectVar(2, target, effect);
  // Ja, wir treffen nur uns selbst nicht (ja, Workaround .)
  if(EffectVar(4, target, effect)) exclude = target;
  
  //DrawParticleLine("NoGravSpark",newx-oldx, newy-oldy,0,0,1,25,RGB(255,0,0),RGB(0,0,255));
  // Wir suchen nach Objekten entlang der Linie die wir uns seit dem letzten Check
  // bewegt haben. Und sortieren sie nach Distanz (nähere zuerst)
  for(obj in FindObjects(Find_OnLine(oldx,oldy,newx,newy),
                         Find_NoContainer(),
                         Sort_Distance(oldx, oldy)))
  {
		// Excludes
		if(obj == target) continue;
		if(obj == exclude) continue;
		
		// CheckEnemy
		if(!CheckEnemy(obj,target)) continue;

		// IsBulletTarget oder Alive
		if(obj->~IsBulletTarget(GetID(target),target,EffectVar(2, target, effect)) || GetOCF(obj) & OCF_Alive) {
			DebugLog("%s IsBulletTarget: %i, %s, %s","HitCheck",GetName(obj),GetID(target),GetName(target),GetName(EffectVar(2, target, effect)));
			return(target-> ~HitObject(obj));
	  }
  }

  EffectVar(0, target, effect) = GetX(target);
  EffectVar(1, target, effect) = GetY(target);

  //verdammt, kommentier doch mal... Also:
  // Der Schuss wird erst "scharf gemacht", d.h. kann den Schützen selbst treffen, wenn
  // der Schuss einmal die Shape des Schützen verlassen hat.
  
  // OKOK, Ich hab sogar das Restzeug kommentiert. :P
  if(!EffectVar(5,target,effect)) {
	  if(!EffectVar(4, target, effect)) {
	    // ready gibt an, ob wir schon "scharf" sind. True = Scharf
	    var ready = true;
	    // Wir suchen alle Objekte mit der ID unseres Schützens an unserer momentanen Stelle
	    for(var foo in FindObjects(Find_AtPoint(GetX(target),GetY(target)),Find_ID(EffectVar(3, target, effect))))
	    	// und schauen, ob es der Schütze ist.
	      if(foo == EffectVar(2, target, effect))
	        // wir haben den Schützen gefunden -> Er ist noch an unserer Position
	        ready = false;
	    // wir haben den Schützen nicht gefunden
	    if(ready)
	      // -> Wir treffen ihn ab jetzt.
	      EffectVar(4, target, effect) = true;
	  }
  }
}

// Ist ein Schuss! Nicht warpen!
func NoWarp() { return(true); }
// Ist ein Schuss!
func IsBullet() { return(true); }
