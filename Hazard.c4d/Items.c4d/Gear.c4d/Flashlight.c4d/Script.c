/*-- Flashlight --*/

#strict
#include GEAR

local light;
local user;

/* Status */

public func GetGearType()	{ return(GEAR_Light); }
public func IsEquipment()	{ return(true); }
public func IsLamp() 		{ return(true); }

/* Callbacks */

public func GearEffect(object pClonk)
{
  user = pClonk;
  light = AddLightCone(1000,RGBa(255,255,220,90),user);
  light->ChangeSizeXY(1900,6000);
  light->ChangeOffset(0,0, true);
  SetAction("On");
  Sound("Click");
}

public func GearEffectReset(object pClonk)
{
  user = 0;
  if(light) RemoveObject(light);
  SetAction("Idle");
  Sound("Click");
}


private func CheckRotation() {
  light->ChangeR(user->~AimAngle(20,180));
}
