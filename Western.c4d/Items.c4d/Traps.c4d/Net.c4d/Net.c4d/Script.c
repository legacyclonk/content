/*-- Netz --*/

#strict 2

public func ControlThrow(object pClonk)
{
  [$GetLiving$]
  if(pClonk == GetActionTarget(1)) return(0);
//  if(!(GetActionTarget(1)->~IsClonk())) 
//    if(!(GetActionTarget(1)->~ForceEnter(pClonk)))
//      Enter(pClonk,GetActionTarget(1));
  if(GetActionTarget()) GetActionTarget()->~DeArm(pClonk);
  RemoveObject(this());
}
