/*-- Invisible --*/

#strict

public func IsBonus() { return(true); }

public func Color() { return(RGB(200,0,255)); }

public func Activate(object obj) {
  if(!AddEffect("InvisBonus",obj,190,1,obj))
    return();
  Sound("Invisible", 1, 0,0, GetOwner(obj)+1);
  return(true);
}

global func FxInvisBonusStart(object pTarget, int iEffectNumber, int tmp) {
  if(tmp)
    return();
  //Wegen den Layern machen wir es einfach mit einem Hilfsobjekt. :)
  pTarget->SetObjectBlitMode(1);
  pTarget->SetClrModulation(RGBa(255,255,255,180));
}

global func FxInvisBonusTimer(object pTarget, int iEffectNumber, int iEffectTime) {
  SetPosition(GetX(pTarget),GetY(pTarget),EffectVar(0,pTarget,iEffectNumber));
  if(iEffectTime > BONUS_Duration)
    return(-1);
}

global func FxInvisBonusStop(object pTarget, int iEffectNumber, int tmp) {
  if(tmp)
    return();
  pTarget->SetClrModulation();
  pTarget->SetObjectBlitMode();
}

global func FxInvisBonusEffect(string szNewEffectName, object pTarget, int iEffectNumber, int iNewEffectNumber) {
  if(szNewEffectName S= "BerserkerBonus")
  {
    return(-1);
  }
}
