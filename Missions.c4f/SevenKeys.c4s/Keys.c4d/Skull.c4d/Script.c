
#strict

#include SKUL
#include _KEY

public func Init(object oWhere)
{
	if(SearchPositionEarth()) return(1);
	Message("$TxtPlacementFailed$", oWhere);
}

public func Announce(object oWhere)
{
	Message("$TxtAnnounce$", oWhere);
}

public func Timer()
{
	var obj;
	while(obj = FindObject(0, -100, -100, 200, 200, OCF_Alive, 0, 0, 0, obj))
		if(ObjectDistance(obj) <= 100)
			obj->DoEnergy(-1);
	return(inherited());
}

public func GetKeyDesc() { return "$TxtAnnounce$"; }
