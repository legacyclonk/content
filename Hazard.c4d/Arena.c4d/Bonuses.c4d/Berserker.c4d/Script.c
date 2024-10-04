/*-- Berserker --*/

#strict

//Dauer: die halbe * die ganze Antwort!
static const BONUS_Duration = 882;

public func IsBonus() { return(true); }

public func Color() { return(RGB(255,100,0)); }

public func Activate(object obj) {
  if(!AddEffect("BerserkerBonus",obj,190,5,obj))
    return();
  Sound("Berserk", 1, 0,0, GetOwner(obj)+1);
  return(true);
}

global func FxBerserkerBonusStart(object pTarget, int iEffectNumber, int tmp) {
  if(tmp)
    return();
  SetClrModulation(RGB(255,50,0),pTarget);
}

global func FxBerserkerBonusTimer(object pTarget, int iEffectNumber, int iEffectTime) {
  CreateParticle("PxSpark",RandomX(-3,3),-8,0,-10,35,RGB(250,50+Random(100),Random(20)),pTarget,1);
  if(iEffectTime > BONUS_Duration)
    return(-1);
}

global func FxBerserkerBonusStop(object pTarget, int iEffectNumber, int tmp) {
  if(tmp)
    return();
  SetClrModulation(RGB(255,255,255),pTarget);
}

global func FxBerserkerBonusEffect(string szNewEffectName, object pTarget, int iEffectNumber, int iNewEffectNumber) {
  if(szNewEffectName S= "BerserkerBonus")
  {
    return(-1);
  }
}

global func FxBerserkerBonusFMData(object pTarget, int iEffectNumber, int iData, value) {
  //Damage*2!
  if(iData == FM_Damage) return(value*2);
  return(value);
}
