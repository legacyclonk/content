/*-- Kein Friendly Fire! --*/

#strict
#appendto CSHO
#appendto ARRW

// Patrone
private func HitLiving(pTarget)
{
  if(FindObject(FF4V)) if(!Hostile(GetOwner(pTarget),GetOwner(this())) && GetOwner(pTarget) != -1) return(0);
  return(_inherited(pTarget));
}

// Pfeil
private func HitTarget(pTarget)
{
  if(FindObject(FF4V)) if(!Hostile(GetOwner(pTarget),GetOwner(this())) && GetOwner(pTarget) != -1) return(0);
  return(_inherited(pTarget));
}

protected func Activate(pClonk)
{
  if(GetID(this())==FF4V) return(MessageWindow(GetDesc(), pClonk));
  return(_inherited(pClonk));
}