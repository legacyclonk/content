/*-- SEGU --*/

#strict 2
#appendto SEGU

public func MaxDamage() { 
	var maxdamage = EffectVar(0, this, GetEffect("AS_Target",this));
	if(!maxdamage) maxdamage = 220;
	return maxdamage;
}
