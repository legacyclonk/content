/*-- Fußangel --*/

#strict 2
#include TRAP

protected func Check()
{
  if(GetActionTarget()) {
    if(FindObject2(Find_Not(Find_Exclude(GetActionTarget())),Find_AtPoint(),Find_OCF(OCF_Alive)))
      return(0);
    SetActionTargets();
  }
  // Nach geeigneten Lebewesen suchen
  var pLiving = FindObject2(Sort_Distance(),Find_AtPoint(),Find_Or(Find_OCF(OCF_Alive),Find_Func("IsHorse")),
		  Find_NoContainer(),Find_Not(TrapIgnoreTargets()));
  if(pLiving) return(Trap(pLiving));
}

private func Trap(object pToTrap)
{
  // Geräusch
  Sound("ArrowHit");
  // Schaden machen
  if(GetCategory(pToTrap) & C4D_Vehicle) DoDamage(RandomX(10,20), pToTrap);
  else DoEnergy(-RandomX(10,20), pToTrap);
  pToTrap->~Hurt();
  // Wegschleudern
  if(pToTrap->~IsClonk()) {
    ObjectSetAction(pToTrap, "KneelUp");
    SetXDir(GetXDir(pToTrap)/3, pToTrap);
  }
  else Fling(pToTrap, RandomX(-1, 1), -1);
  // Nach Zufall verschwinden
  if(!Random(3)) return(RemoveObject());
  // Ansonsten kleine Auszeit gönnen
  SetActionTargets(pToTrap);
}