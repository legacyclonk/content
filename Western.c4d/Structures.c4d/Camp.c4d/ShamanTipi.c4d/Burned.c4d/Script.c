/*-- Verbranntes Scharmanenzelt --*/

#strict 2

// Jegliche Nicht-fertige Amulette sollen in ihre Bestandteile zerlegt werden
public func Ejection(object pObject)
{
//  if(pObject->~!IsIndianAmulet()) return(0);
  if(GetCon(pObject) == 100) return(0);
  if(OnFire())
    if(GetDefCoreVal("BlastIncinerate","DefCore",GetID(pObject)) || GetDefCoreVal("ContactIncinerate","DefCore",GetID(pObject)))
      return(Incinerate(pObject));
  if(~GetOCF(pObject) & OCF_CrewMember)
    Split2Components(pObject);
}
