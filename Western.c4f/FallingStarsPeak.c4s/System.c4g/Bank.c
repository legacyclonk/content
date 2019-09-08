/* Bank */

#strict

#appendto BANK

public func RejectCollect(id idObj, object pObj)
{
  // Objekt ist Gold oder ein Sack voll Geld
  if(idObj != GOLD && idObj != MBAG) return();
  // Objekt wird von einem Clonk getragen
  if(Contained(pObj))
   if(GetOCF(Contained(pObj)) & OCF_CrewMember())
    {
    // Nicht für Banditen
    if(Contained(pObj)->~IsBandit()) return(Sound("Error", 1, 0, 0, GetOwner(pObj)+1));
    DoWealth(GetOwner(Contained(pObj)), GetValue(pObj));
    DoPoints(GetValue(pObj), GetOwner(Contained(pObj)));
    Sound("Cash", 1, 0,0, GetOwner(pObj));
    RemoveObject(pObj);
    return(1);
    }
}
