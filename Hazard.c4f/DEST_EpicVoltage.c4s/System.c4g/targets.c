/*-- Ziele --*/

#strict 2
#appendto SEGU
#appendto GNR2

public func MaxDamage() { 
	var eff = GetEffect("DEST_TargetCheck",this);
	if(!eff) return _inherited(...);

	var maxdamage = EffectVar(0, this, eff);
	if(!maxdamage) maxdamage = 220;
	return maxdamage;
}

public func IsBulletTarget() {
	var eff = GetEffect("DEST_TargetCheck",this);
	if(!eff) return _inherited(...);
	return true;
}
public func IgnoreFriendlyFire() {
	var eff = GetEffect("DEST_TargetCheck",this);
	if(!eff) return _inherited(...);
	return true;
}