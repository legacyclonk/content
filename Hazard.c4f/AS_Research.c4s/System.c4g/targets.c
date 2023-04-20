/*-- Ziele --*/

#strict 2
#appendto LBPC
#appendto CCP2
#appendto CCP1

public func Damage()
{
	var maxdamage = EffectVar(0, this, GetEffect("AS_Target",this));
	if(maxdamage)
		if(GetDamage() >= maxdamage) Explode(35);
}

public func IsBulletTarget() { return GetEffect("AS_Target",this); }
public func IgnoreFriendlyFire() { return true; }