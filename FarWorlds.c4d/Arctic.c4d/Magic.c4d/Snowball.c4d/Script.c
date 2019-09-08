/* Schneeball */

#strict 2

local iOldX, iOldY;

func Activate(pCaster,pRealcaster) { 
  // Den Clonk ermitteln
  if(pRealcaster) pCaster=pRealcaster;  

  SetAction("Wait");
  AddEffect("IceSnowballNSpell", this, 110, 0, this);   
  
  // Zielen wenn möglich
  if (pCaster->~DoSpellAim(this)) return 1;
	
  Sound("Electric"); 
  SetAction("Travel");
  SetPosition(GetX()+(GetDir(pCaster)*2-1)*20,GetY());
  SetXDir((GetDir(pCaster)*2-1)*100);
  SetCategory(4);
  iOldX = GetX(); iOldY = GetY();
  return 1;
}

func FxIceSnowballNSpellStop(pTarget, iEffectNumber, iReason, fTemp)
{
  if(!fTemp)  RemoveObject(pTarget);
  return 1;
}

// Zwischenaufruf: Zielrichtung geändert
public func AimingAngle(object pCaller, int iAimAngle)
  {
  SetPosition(GetX(pCaller)+Sin(iAimAngle, 20),GetY(pCaller)-Cos(iAimAngle, 20));
  return 1;
  }

// Aktivierung nach Zielen (iAngle: -90=links; 0=oben; +90=rechts)
public func ActivateAngle(object pCaller, int iAimAngle)
{
  Sound("Electric"); 
  SetAction("Travel");
  SetPosition(GetX(pCaller)+Sin(iAimAngle, 20),GetY(pCaller)-Cos(iAimAngle, 20));
  SetXDir(Sin(iAimAngle, 100));
  SetYDir(-Cos(iAimAngle, 100));
  SetCategory(4);
  iOldX = GetX(); iOldY = GetY();
  return 1;
}
  
/* Fliegt */

func Travel() {
  var pLiving;
  CastPXS("Snow", RandomX(8,12), RandomX(20,30));
  if(GetXDir()==0 && GetYDir()==0) Hit();
  // Treffer-Überprüfung
  CheckHit();
  iOldX = GetX(); iOldY = GetY();
}

private func CheckHit()
{
  // Nur wenn schon passende Werte da sind
  if(!iOldX && !iOldY) return;
  // Distance zur letzten Position berechnen
  var iXDist = GetX()-iOldX;
  var iYDist = GetY()-iOldY;
  // Treffer-Überprüfung
  var iSteps = Abs(iXDist/4);
  if(Abs(GetXDir())<Abs(GetYDir())) iSteps = Abs(iYDist/4);
  var iX, iY, pObj;
  // Mit Schleife alle Zwischenpunkte abklappern
  for(var i = iSteps; i; i--)
  {
    iX = -iXDist*i/iSteps; iY = -iYDist*i/iSteps;
    if(!iY) iY = 1;
//    CreateParticle("NoGravSpark", iX, iY, 0, 0, 30, RGB(i*255/iSteps, (iSteps-i)*255/iSteps));
    if(pObj = FindObject(0, iX, iY, 0, 0, OCF_Alive, 0, 0, NoContainer())) return HitLiving(pObj);
  }
}

/* Treffer */

func Hit() {
  CastPXS("Snow", 80, 20);
  RemoveObject();
}
    
func HitLiving(pTarget) {
  CastPXS("Snow", 80, 20);
  DoEnergy(-40, pTarget);
  for (var i=0;i<5;++i)
    CreateParticle("PxSpark", RandomX(-5,5), RandomX(-5,5), 0, 0, RandomX(80,200), RGB(255,40,0));
  // Ist pTarget ein Eskimo, dann wird ihm auch noch Wärme entzogen
  pTarget->~MagicWarm(-650);
  Hit();
}

public func IsAimerSpell() { return 1; }

public func GetSpellClass(object pMage) { return(WATR); }
public func GetSpellCombo(pMage) { return ("366"); } // (1: Backward; 2: Down; 3: Forward; 4: Throw; 5: Jump; 6: Dig)
