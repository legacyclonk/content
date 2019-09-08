/*-- Bohrturmleitung --*/

#strict

local pDerrick;

public func Init(derrick, pipehead) 
{ 
  pDerrick = derrick; 
  SetAction("Exist", pipehead); 
}

public func Adjust()
{
  if (!GetActionTarget() || !pDerrick)
    return(RemoveObject());
  SetPosition(GetX(pDerrick), GetY(pDerrick) + GetObjHeight(pDerrick) / 2);
}
