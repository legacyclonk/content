#strict
#include GNRT

local lastMalfunction; 


public func Damage()
{
	var eff = GetEffect("DEST_TargetCheck",this);
	var maxdamage;
	if(eff) maxdamage = EffectVar(0, this, eff);
	else    maxdamage = 800;
	if(FrameCounter() - lastMalfunction > 10)
	{
		lastMalfunction = FrameCounter();
		Malfunc();
	}
	if(GetDamage() >= maxdamage) Explode(70);
}
