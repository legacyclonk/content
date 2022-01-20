/*-- Steeldoor --*/

#strict

#include GBDR

local Status;

public func CanBeLaserRecycled() { return(2); }

//Einstellungem

public func Initialize()
{
	SetAction("Close");
	_inherited();
	SetMaxDamage(80);
}

public func OnDmg(int iDmg, int iType) {

	// diese Tür ist nicht besonders dick
	if(iType == DMG_Projectile)	return(50);
	if(iType == DMG_Melee)	return(20);
	if(iType == DMG_Energy)	return(40);
	if(iType == DMG_Explosion)	return(0);

	return(100);
}

public func OnOpen()
{
	if( GetAction() eq "Open" || Status) return(0);

	SetAction("Open");
	SetSolidMask(0);
	Status = true;
	//Sound
	Sound("SteeldoorOpen");
}

public func OnClose()
{
	if( GetAction() eq "Close"|| !Status) return(0);  
	SetAction("Close");
	Status = false;
	SetSolidMask(12, 0, 10, 30, 12, 0);
	Sound("SteeldoorClose");
}

public func OnDestroyed(iType)
{
	SetAction("Destroyed");
}

/* Serialisierung */

public func Serialize(array& extra)
{
	if (destroyed)
		extra[GetLength(extra)] = "SetAction(\"Destroyed\")";
}
