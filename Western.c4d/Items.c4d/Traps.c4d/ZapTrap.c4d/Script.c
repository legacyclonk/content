/*-- Zapfalle --*/

#strict
#include TRAP

protected func Check()
{
  // Nach geeigneten Lebewesen suchen
  var pLiving = FindObject2(Sort_Distance(),Find_AtPoint(),Find_Or(Find_OCF(OCF_Alive),Find_Func("IsHorse")),
		  Find_NoContainer(),Find_Not(TrapIgnoreTargets()));
  if(pLiving) return(Trap(pLiving));
}

private func Trap(pToTrap)
{
  // Zaps freilassen
  CreateObject(ZAPN,0,0,GetOwner())->ZAPN::BreakUp(17); // Einige Zaps wurden zermatscht
  // Geräusch
  Sound("Rip");
  // Weg
  CastObjects(LTHR,1,1);
  CastObjects(ROPE,1,1);
  return(RemoveObject());
}

public func DeArmExtra(object pTrapper)
{
  if(pTrapper) {
    CastObjects(LTHR,1,1);
    CastObjects(ROPE,1,1);
    Collect(CreateObject(ZAPN,0,3,GetOwner()),pTrapper);
    RemoveObject();
  }
}

protected func Hit() { return(Trap()); }