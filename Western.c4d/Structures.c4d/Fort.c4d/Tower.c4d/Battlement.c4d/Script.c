/*-- Brustwehr --*/

#strict

local iX, iY;

public func AttachTo(object pTarget, int iDir)
{
  iX = GetX() - GetX(pTarget);
  iY = GetY() - GetY(pTarget);
  if(iDir == DIR_Left())
    {
    SetAction("AttachLeft", pTarget);
    SetSolidMask(7, 15, 10, 15, -4);
    }
  if(iDir == DIR_Right())
    {
    SetAction("AttachRight", pTarget);
    SetSolidMask(7, 0, 10, 15);
    }
  return(1);
}

public func Positioning()
{
  if(!GetActionTarget()) return(RemoveObject());
  if(OnFire(GetActionTarget())) return(RemoveObject());

  if(GetX() - GetX(GetActionTarget()) != iX)
    SetPosition(GetX(GetActionTarget())+iX, GetY());
  if(GetY() - GetY(GetActionTarget()) != iY)
    SetPosition(GetX(), GetY(GetActionTarget())+iY);
}
