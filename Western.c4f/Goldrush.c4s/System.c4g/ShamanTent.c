/* Schamanenzelt: Erst verfügbar, wenn durchs Szenario freigegeben */

#strict

#appendto NTIP

protected func ContainedUp(pClonk)
{
  [$Production$|Image=CXCN]
  if(!Local(2)) return();
  return(_inherited(pClonk));
}

public func ContextConstruction(pCaller) {
  [$Production$|Image=CXCN|Condition=IsBuildAllowed]
  return MenuProduction(pCaller);
}

public func IsBuildAllowed()
{
  if(!IsBuilt()) return;
  if(!Local(2)) return;
  return 1;
}

protected func ContextRecruitChief(object pClonk) { [|Condition=Never] return(false); }
protected func ContextGetMystic(object pClonk) { [|Condition=Never] return(false); }
protected func ContainedDig(object pClonk, int iMenu) { return(false); }
protected func Never() { return(false); }