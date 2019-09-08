/*-- Schatztruhe --*/

#strict

#include CHST

protected func Initialize()
{
  _inherited();
  // Alle Verbündeten des Besitzers speichern
  for(var i=0, l=0 ; i < GetPlayerCount() ; i++)
    if(GetPlayerByIndex(i) > -1)
      if(!Hostile(GetOwner(), i))
        {
        Local(l) = i+1;
        l++;
        }
}
  
public func RejectCollect(id idObj, object pObj)
{
  // Hineinlegen durch Basisimplementation verhindert
  if (_inherited(idObj, pObj)) return(1);
  // Objekt ist Gold oder ein Sack voll Geld
  if ((idObj != GOLD) && (idObj != MBAG))
    { Sound("Error", 1, 0, 0, GetOwner(pObj) + 1); return(1); } // TODO: sollte auch noch passende Meldung ausgeben...
  // Objekt wird von einem Clonk getragen
  if (Contained(pObj))
    if (GetOCF(Contained(pObj)) & OCF_CrewMember())
    {
      // Nur für Banditen
      if (!Contained(pObj)->~IsBandit()) 
        { Sound("Error", 1, 0, 0, GetOwner(pObj) + 1); return(1); } // TODO: sollte auch noch passende Meldung ausgeben...  
      // Reinlegen ok
      DoWealth(GetOwner(Contained(pObj)), GetValue(pObj));
      Sound("Cash", 1, 0,0, GetOwner(pObj));
      RemoveObject(pObj);
      return(1);
    }
}
  
protected func ChangeOwner()
{
  // Besitzer ist verschwunden?
  if(GetOwner() > -1) return();
  // Den nächsten Verbündeten nehmen
  var iNewOwner = GetNewOwner();
  if(iNewOwner)
    SetOwner(iNewOwner-1);
  else
    RemoveObject();
}

private func GetNewOwner()
{
  var i, r;
  while(Local(i))
    i++;
  r = Local(i-1);
  Local(i-1)=0;
  return(r);
}
