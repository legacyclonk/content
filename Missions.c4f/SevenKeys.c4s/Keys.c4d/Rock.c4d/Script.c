/*--- Der Stein ---*/

#strict
#include ROCK
#include _KEY

public func Init(object oWhere)
{
	if(SearchPositionInMatEx(Material("Rock"))) return(1);
	Message("$TxtPlacementFailed$", oWhere);
}

public func Announce(object oWhere)
{
	Message("$TxtAnnounce$", oWhere);
}

public func GetKeyDesc() { return "$TxtAnnounce$"; }
