/*-- Haste --*/

#strict

public func IsBonus() { return(true); }

public func Color() { return(RGB(255,255,0)); }

public func Activate(object obj) {
  if(!AddEffect("HasteBonus",obj,60,5,obj))
    return();
  Sound("Speed", 1, 0,0, GetOwner(obj)+1);
  return(true);
}

global func FxHasteBonusStart(object pTarget, int iEffectNumber, int tmp) {
  if(tmp)
    return();
  pTarget->SetPhysical("Walk",pTarget->GetPhysical("Walk") + 15000,PHYS_StackTemporary);
  pTarget->SetPhysical("Jump",pTarget->GetPhysical("Jump") + 15000,PHYS_StackTemporary);
  pTarget->SetPhysical("Scale",pTarget->GetPhysical("Scale") + 10000,PHYS_StackTemporary);
  pTarget->SetPhysical("Hangle",pTarget->GetPhysical("Hangle") + 10000,PHYS_StackTemporary);
}

global func FxHasteBonusTimer(object pTarget, int iEffectNumber, int iEffectTime) {
  CreateParticle("NoGravSpark",RandomX(-5,5),RandomX(-6,6),0,0,35,RGB(250,250,Random(20)),0,Random(2));
  if(iEffectTime > BONUS_Duration)
    return(-1);
}

global func FxHasteBonusStop(object pTarget, int iEffectNumber, int tmp) {
  if(tmp)
    return();
  pTarget->SetPhysical("Walk",pTarget->GetPhysical("Walk") - 15000,PHYS_StackTemporary);
  pTarget->SetPhysical("Jump",pTarget->GetPhysical("Jump") - 15000,PHYS_StackTemporary);
  pTarget->SetPhysical("Scale",pTarget->GetPhysical("Scale") - 10000,PHYS_StackTemporary);
  pTarget->SetPhysical("Hangle",pTarget->GetPhysical("Hangle") - 10000,PHYS_StackTemporary);
}

global func FxHasteBonusEffect(string szNewEffectName, object pTarget, int iEffectNumber, int iNewEffectNumber) {
  if(szNewEffectName S= "HasteBonus")
  {
    return(-1);
  }
}
