/*-- Physicalamulett --*/

#strict

public func IsAmulet() { return(1); }

protected func Initialize() { Entrance(Contained()); }

/* Eintritt */

protected func Entrance(object pContainer)
{
  if(!(GetOCF(pContainer) & OCF_CrewMember())) return(0);
  AddEffect("PhysicalBless",pContainer,182,0,0,GetID());
  return(1);
}

private func IncPhysical(string szPhysical, int iInc, int iMode, object pObject)
{
  SetPhysical(szPhysical,Min(GetPhysical(szPhysical,0,pObject)+iInc,100000),iMode,pObject);
}

/* Austritt oder Zerstörung */

protected func Departure(object pContainer) { RemoveEffect("PhysicalBless",pContainer); }
protected func Destruction() { RemoveEffect("PhysicalBless",Contained()); }

/* Verbesserungseffekt */

func FxPhysicalBlessStart(object pTarget,int iEffectNumber, int iTemp)
{
  // Attributserhöhung
  SetPhysical("CanHangle" ,1,PHYS_StackTemporary,pTarget);
  IncPhysical("Dig" ,  10000,PHYS_StackTemporary,pTarget);
  IncPhysical("Walk",  10000,PHYS_StackTemporary,pTarget);
  IncPhysical("Jump",  10000,PHYS_StackTemporary,pTarget);
  IncPhysical("Throw", 10000,PHYS_StackTemporary,pTarget);
  IncPhysical("Swim",  30000,PHYS_StackTemporary,pTarget);
  IncPhysical("Scale", 50000,PHYS_StackTemporary,pTarget);
  IncPhysical("Hangle",50000,PHYS_StackTemporary,pTarget);
  IncPhysical("Fight", 40000,PHYS_StackTemporary,pTarget);
  IncPhysical("Breath",20000,PHYS_StackTemporary,pTarget);
}

func FxPhysicalBlessStop(object pTarget,int iEffectNumber,int iReason,bool fTemp)
{
  // Attribute wiederherstellen
  ResetPhysical(pTarget, "Breath");
  ResetPhysical(pTarget, "Fight");
	ResetPhysical(pTarget, "Hangle");
	ResetPhysical(pTarget, "Scale");
	ResetPhysical(pTarget, "Swim");
	ResetPhysical(pTarget, "Throw");
	ResetPhysical(pTarget, "Jump");
	ResetPhysical(pTarget, "Walk");
  ResetPhysical(pTarget, "Dig");
	ResetPhysical(pTarget, "CanHangle");
  return(FX_OK);
}

func FxPhysicalBlessEffect(string szNewEffectName, object pTarget, int iEffectNumber, int iNewEffectNumber)
{
  // keine weiteren Verbesserungen zulassen während diese noch in Aktion ist
  if(szNewEffectName eq "PhysicalBless") return(-1);
  // Vergiftung löscht die Wirkung des Amuletts aus
  if(szNewEffectName eq "PoisonCurse") return(RemoveEffect(0,pTarget,iEffectNumber));
}