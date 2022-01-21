/*-- Kamikaze --*/

#strict

public func IsBonus() { return(true); }

public func Color() { return(RGB(20,20,20)); }

public func Activate(object obj) {
  if(!Random(10))
    Explode(50,CreateObject(TIM2,GetX(obj),GetY(obj),-1));
  else if(!AddEffect("KamikazeBonus",obj,190,5,obj))
    return(0);
  Sound("Kamikaze", 1, 0,0, GetOwner(obj)+1);
  return(1);
}

global func FxKamikazeBonusTimer(object pTarget, int iEffectNumber, int iEffectTime) {
  if(!(iEffectTime%1500))
    if(!Random(10))
      return(-1);
}

global func FxKamikazeBonusStop(object pTarget, int iEffectNumber, int iReason, bool tmp) {
  if(tmp)
    return();
  Explode(50,CreateObject(TIM2));
}

global func FxKamikazeBonusEffect(string szNewEffectName, object pTarget, int iEffectNumber, int iNewEffectNumber) {
  if(szNewEffectName S= "KamikazeBonus")
  {
    return(-1);
  }
}
