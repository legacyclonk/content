/*-- Sequium Door --*/

#strict

#include GBDR

local pathfree;

public func Initialize() 
{
	pathfree = false;
	SetAction("Closed");
	_inherited();
	// not destroyable
	SetMaxDamage(-1);
}

public func OnOpen()
{
	if(GetAction() eq "Open" || GetAction() eq "Opened") return();
	SetAction("Open");
	Sound("SequiumDoorOpen");
}

public func OnClose()
{
	if(GetAction() eq "Closed" || GetAction() eq "Close") return();
	SetAction("Close");
	SetSolidMask(5, 0, 8, 30, 5, 0);
	Sound("SequiumDoorClose");
}

public func SetPathFree()
{
	pathfree = true;
	SetSolidMask();
}

/* Serialisierung */

public func Serialize(array& extra)
{
	_inherited(extra);
	if (pathfree)
		extra[GetLength(extra)] = "SetPathFree()";
}
