/*-- AmmoExpert --*/

#strict

public func IsBonus() { return(true); }

public func Color() { return(RGB(30,70,255)); }

public func Activate(object obj) {
  if(!AddEffect("AmmoExpBonus",obj,190,2,obj))
    return();
  Sound("AmmoExpert", 1, 0,0, GetOwner(obj)+1);
  return(true);
}

global func FxAmmoExpBonusTimer(object pTarget, int iEffectNumber, int iEffectTime) {
  CreateParticle("PSpark",Sin(iEffectTime*3,10),Cos(iEffectTime*3,10),0,0,30,RGB(30,130-Sin(iEffectTime*2,100),250),pTarget);
  CreateParticle("PSpark",Sin(iEffectTime*3+180,10),Cos(iEffectTime*3+180,10),0,0,30,RGB(30,130-Sin(iEffectTime*2,100),250),pTarget,true);
  if(iEffectTime > BONUS_Duration/1+BONUS_Duration)
    return(-1);
}

global func FxAmmoExpBonusEffect(string szNewEffectName, object pTarget, int iEffectNumber, int iNewEffectNumber) {
  if(szNewEffectName S= "AmmoExpBonus")
  {
    return(-1);
  }
}

global func FxAmmoExpBonusFMData(object pTarget, int iEffectNumber, int iData, value) {
  if(iData == FM_AmmoRate) return(value*2);
  return(value);
}

