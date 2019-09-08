/*-- Wertsachen --*/

#strict 2
#appendto CRWN
#appendto GBLT
#appendto EGBL
#appendto _BOK

// Können nicht wieder gekauft werden
public func SellTo() { return(0); }

// Damit man nicht gleich alle Wertsachen mitnehmen kann
public func RejectEntrance(object pObj)
{
  if(GetID() == _BOK) return(_inherited(pObj));
  if(GetOCF(pObj) & OCF_CrewMember)
    if(FindContents(GBLT,pObj) || FindContents(EGBL,pObj) || FindContents(CRWN,pObj))
      return(1);
  return(_inherited(pObj));
}

public func IsValuable() { return(true); }