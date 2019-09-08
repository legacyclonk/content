/*-- Fallstrick --*/

#strict 2
#include TRAP

protected func Initialize()
{
  fCollectible = true;
  return(1);
}

protected func Check()
{
  // Nach geeigneten Lebewesen suchen
  var pLiving = FindObject2(Sort_Distance(),Find_AtPoint(),Find_OCF(OCF_CrewMember),Find_NoContainer(),
		  Find_Not(Find_Or(Find_FuncInside("GetXDir()",-10,10),TrapIgnoreTargets())),Find_FuncEqual("GetContact(0,-1,CNAT_Bottom)",8));
  if(pLiving) return(Trap(pLiving));
}

private func Trap(object pToTrap)
{
  // Lebewesen wegschleudern
  Fling(pToTrap, GetDir(pToTrap)*2-1, -3);
  // Geräusch
  Sound("CatapultSet");
  // vielleicht verschwinden
  if(!Random(9)) {
    // vielleicht ein Seil zurücklassen	  
    if(!Random(2)) CastObjects(ROPE, 1, 1);
    RemoveObject(this());	  
  }
}

public func DeArmExtra(object pTrapper)
{
  if(pTrapper)
    if(Collect(this(),pTrapper))
      Split2Components(this());
  return(1);
}