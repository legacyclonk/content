/*-- Fangeisen --*/

#strict 2
#include TRAP

protected func Check()
{
  // Nach geeigneten Lebewesen suchen
  var pLiving = FindObject2(Sort_Distance(),Find_AtPoint(),Find_Or(Find_OCF(OCF_Alive),Find_Func("IsHorse")),
		  Find_NoContainer(),Find_Not(TrapIgnoreTargets()));
  if(pLiving) return(Trap(pLiving));
}

private func Trap(object pToTrap)
{
  // Schnapp
  Sound("GinTrap");
  // Das tut schmerzen
  if(GetCategory(pToTrap) & C4D_Vehicle) DoDamage(RandomX(30,40),pToTrap);
  else DoEnergy(-RandomX(35,45),pToTrap);
  pToTrap->~Hurt();
  // Fling!
  if(pToTrap->~IsClonk()) ObjectSetAction(pToTrap,"FlatUp");
  else Fling(pToTrap,RandomX(-1,1),-1);
  SetXDir(0,pToTrap);
  // Muss neu aufgestellt werden
  DeArm();
}

public func DeArmExtra() { SetPicture(0,8,64,64); }

public func ArmExtra()
{
  // Bild anpassen
  SetPicture();
  // Der Rest ist in der Fallensteuerung vorhanden
  return(0);
}