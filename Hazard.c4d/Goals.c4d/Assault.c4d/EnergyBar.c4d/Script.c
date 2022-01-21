/*-- Energiebalken --*/

#strict

local obj;

protected func Initialize()
{
  SetGraphics("Row", this(), GetID(), 1, 1);
}

public func Set(object target, int color)
{
  obj = target;
  
  SetVertex(0,0,GetVertex(0,0,target));
  SetVertex(0,1,GetVertex(0,1,target) + GetObjHeight(target)/2+10);
  SetAction("Attach",target);
  
  SetClrModulation(color,this(),1);
  
  Update(100);
}

public func Update(int percent)
{
  SetObjDrawTransform(10*percent,0,-500*(100-percent), 0,1000,0, 0, 1);
}
