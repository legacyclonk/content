/*-- Bisonamulett --*/

#strict 2

#include AMBR

// Menü zum Ablegen
public func AttachContextUnWear(object pCaller, object pClonk)
{
  [$TxtUnWear$|Image=AMBI|Condition=CheckCaller]
  return(_inherited(pCaller,pClonk));
}

public func AttachContextDecompose(object pCaller, object pClonk) {
  [$MnuDecompose$|Image=SKUL|Condition=FindDecomposeItemCheck]
  return(_inherited(pCaller,pClonk));
}

public func AttachContextUse(object pCaller, object pClonk)
{
  [$TxtMagic$|Image=MCMS|Condition=CheckCaller]
  return(_inherited(pCaller,pClonk));
}

/* Verbesserungseffekt */

protected func FxWearBlessStart(object pTarget, int iEffectNumber, int iTemp)
{
  // Temporäre Aufrufe ignorieren
  if(iTemp) return(0);
  // Erhöhung der Stärke
  IncPhysical("Fight", 30000,3,pTarget);
  IncPhysical("Push",  15000,3,pTarget);
  IncPhysical("Throw", 15000,3,pTarget);
  IncPhysical("Scale", 15000,3,pTarget);
  IncPhysical("Hangle",15000,3,pTarget);
  return(1);
}

protected func FxWearBlessStop(object pTarget, int iEffectNumber, int iReason, bool fTemp)
{
  if(fTemp) return(0);
  // Attribute wiederherstellen
  ResetPhysical(pTarget,"Fight");
  ResetPhysical(pTarget,"Push");
  ResetPhysical(pTarget,"Throw");
  ResetPhysical(pTarget,"Scale");
  ResetPhysical(pTarget,"Hangle");
  // Bei Tod Amulett verlieren
  var pAmulet = FindObject(0,0,0,0,0,0,"WearAmulet",pTarget);
  if(pAmulet && iReason==4) ObjectSetAction(pAmulet,"Idle");
  SetCategory(C4D_Object,pAmulet);
  return(1);
}

public func GetSpell(iIndex)
{
// Von nun an gibt es neue Zauber
/*
  if(!iIndex--) return(MQKE);
  if(!iIndex--) return(MGUP);
  if(!iIndex--) return(MSSH);
  if(!iIndex--) return(MGFL);
  if(!iIndex--) return(ETFL);
*/
  if(!iIndex--) return(MSSH);
  if(!iIndex--) return(MQKE);
  if(!iIndex--) return(MGRP);
  if(!iIndex--) return(MGFL);
  return(0);
}