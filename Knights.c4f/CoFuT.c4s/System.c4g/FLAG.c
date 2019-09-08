#strict
#appendto FLAG

protected func RejectEntrance(object pClonk)
  {
  if (ActIdle()) return();
  return (Hostile(GetOwner(), GetOwner(pClonk)));
  }
