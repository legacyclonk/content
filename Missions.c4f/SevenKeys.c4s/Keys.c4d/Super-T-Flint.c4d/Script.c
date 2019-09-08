
#strict

#include STFN
#include _KEY

local iX, iY;

public func Init(object oWhere)
{
	if(SearchPositionEarth()) { iX = GetX(); iY = GetY(); return(1); }
	Message("$TxtPlacementFailed$", oWhere);
}

public func Announce(object oWhere)
{
	Message("$TxtAnnounce$", oWhere);
}

public func Timer()
{
	// Bewegt und noch nicht aktiv? Aktivieren
	if(GetAction() ne "Activated")
		if(iX != GetX() || iY != GetY())
			Hit();
	return(inherited());
}

public func GetKeyDesc() { return "$TxtAnnounce$"; }

public func DoExtinguish() {
	// Disallow extinguish via spell. Sorry guys, that's too easy.
	BlowUp();
}