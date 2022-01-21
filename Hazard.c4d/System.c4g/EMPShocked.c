/*-- EMP Shocked--*/

#strict 2

global func EMPShockEffect(int iTime) {
  AddEffect("EMPDamaged", this(), 190, 5, 0, 0,iTime);
}

global func EMPShocked() {
  return GetEffect("EMPDamaged",this());
}

global func FxEMPDamagedStart(object pTarget, int iEffectNumber, int iTemp, int iTime)
{
  EffectVar(0, pTarget, iEffectNumber) = iTime;
  EffectVar(1, pTarget, iEffectNumber) = pTarget->GetClrModulation();
  pTarget->SetClrModulation(RGB(200,200,200));
}

global func FxEMPDamagedTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  
  if(EffectVar(0, pTarget, iEffectNumber) < iEffectTime)
    return -1;
  // effects
  if(!(pTarget->~Contained())) {
    var wdt = GetDefWidth(GetID(pTarget));
    var hgt = GetDefHeight(GetID(pTarget));
    Sparks(2,0,GetX(pTarget)+RandomX(-wdt/2,wdt/2),GetY(pTarget)+RandomX(-hgt/2,hgt/2));
    if(!Random(3)) {
		var pLaser = CreateObject(LASR, GetX(pTarget)+RandomX(-wdt/4,wdt/4), GetY(pTarget)+RandomX(-hgt/4,hgt/4), -1);
		pLaser->SetClrModulation(RGBa(255, 200, 0, 64));
		pLaser->Set(Random(360), 8, Sqrt(wdt*wdt/4+hgt*hgt/4), 3);
	}
  }
}

global func FxEMPDamagedAdd(object pTarget, int iEffectNumber, string szNewEffectName, int iNewEffectTimer, iTime) {
  EffectVar(0,pTarget,iEffectNumber) = Max(EffectVar(0,pTarget,iEffectNumber),iTime); 
}

global func FxEMPDamagedEffect(string szNewEffectName, object pTarget, int iEffectNumber, int iNewEffectNumber) {
  if(szNewEffectName == "EMPDamaged")
    return -2;
}

global func FxEMPDamagedStop(object pTarget, int iEffectNumber) {
  pTarget->SetClrModulation(EffectVar(1, pTarget, iEffectNumber));
  pTarget->~EMPShockEnd();
}