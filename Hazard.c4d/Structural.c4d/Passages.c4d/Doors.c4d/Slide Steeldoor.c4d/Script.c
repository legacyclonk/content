/*-- Slide Door --*/

#strict

#include GBDR

local pathfree;

public func CanBeLaserRecycled() { return(2); }

public func Initialize() 
{
	pathfree = false;
	SetAction("Closed");
	_inherited();
	SetMaxDamage(300);
}

public func OnDmg(int iDmg, int iType) {
	// diese Tür ist besonders dick
	if(iType == DMG_Projectile)	return(60);
	if(iType == DMG_Melee)	return(80);
	if(iType == DMG_Energy)	return(30);
	if(iType == DMG_Explosion)	return(0);

	return(100);
}

public func OnOpen()
{
	if(GetAction() eq "Open" || GetAction() eq "Opened") return();
	SetAction("Open");
	Sound("SlideSteeldoorOpen");
}

public func OnClose()
{
	if(GetAction() eq "Closed" || GetAction() eq "Close") return();
	SetAction("Close");
	Sound("SlideSteeldoorClose");
	SetSolidMask(140, 0, 4, 30, 7, 0);
}

public func SetPathFree()
{
	pathfree = true;
	SetSolidMask();
}

public func OnDestroyed(iType)
{
    //Kleinen Explosionseffekt
    CreateObject(TIM1, 0,0, -1)->Explode(20);
    SetAction("Destroyed");
}

/* Serialisierung */

public func Serialize(array& extra)
{
	_inherited(extra);
	if (destroyed)
		extra[GetLength(extra)] = "SetAction(\"Destroyed\")";
	if (pathfree)
		extra[GetLength(extra)] = "SetPathFree()";
}
