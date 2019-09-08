
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

protected func Hit()
{
  CastObjects(DFLM, 20, 20);
  Sound("Inflame");
  Sound("Fuse");
  return(1);
}

protected func Timer()
{
	// Ein paar Flammen casten
	if(!GBackSolid()) if(!Random(100))
		CastObjects(DFLM, 2, 10);
	// Träger anzünden
	if(Contained())
		if(Contained()->GetOCF() & OCF_Inflammable)
			Contained()->Incinerate();
	return(_inherited());
}

public func GetKeyDesc() { return "$TxtAnnounce$"; }
