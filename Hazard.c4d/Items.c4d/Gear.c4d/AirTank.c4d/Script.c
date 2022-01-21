/*-- Atemflasche --*/

#strict
#include GEAR

/* Status */

public func GetGearType()	{ return(GEAR_Backpack); }
protected func Hit()		{ Sound("MetalHit*"); }
public func IsEquipment()	{ return(true); }

/* Callbacks */

public func GearEffect(object pClonk)
{
  // add breath effect
  AddEffect("AirTank", pClonk, 99, 3, this());
  Sound("Pshshsh");
}

public func GearEffectReset(object pClonk)
{
  // remove effect
  RemoveEffect("AirTank", pClonk);
}

/* Effect */

public func FxAirTankTimer(object pClonk)
{
  // clonk needs air?
  if(Inside(pClonk->GetBreath(), 1, pClonk->GetPhysical("Breath")/1000-1))
  {
    // Breath!
    pClonk->DoBreath(1);
  }
}
