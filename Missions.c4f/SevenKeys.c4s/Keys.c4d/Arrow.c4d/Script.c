
#strict

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
	// Suche Crewmember in Umgebung
	var pObj = FindObject(0, -1, -1, -1, -1, OCF_CrewMember());
	// Nah?
	if(pObj)
		if(ObjectDistance(pObj) < 200) {
			var dx = GetX() - pObj->GetX(), dy = GetY() - pObj->GetY();
			if(2 * dx > Abs(dy)) { SetXDir(10); SetPosition(GetX()+1, GetY()); }
			if(2 * dx < -Abs(dy)) { SetXDir(-10); SetPosition(GetX()-1, GetY()); }
			if(2 * dy > Abs(dx)) { SetYDir(10); SetPosition(GetX(), GetY()+1); }
			if(2 * dy < -Abs(dx)) { SetYDir(-10); SetPosition(GetX(), GetY()-1); }
		}
	return(inherited());
}

public func GetKeyDesc() { return "$TxtAnnounce$"; }
