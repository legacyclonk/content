/*-- Verbrannte Werkstatt --*/

#strict

#include BAS7

func Ejection(obj) 
{
  if (GetCon(obj) == 100) return();
  if (OnFire()) 
    if (GetDefCoreVal("BlastIncinerate", "DefCore", GetID(obj)) || GetDefCoreVal("ContactIncinerate", "DefCore", GetID(obj)))
      return(Incinerate(obj));
  if (~GetOCF(obj) & OCF_CrewMember())
    Split2Components(obj);
}
