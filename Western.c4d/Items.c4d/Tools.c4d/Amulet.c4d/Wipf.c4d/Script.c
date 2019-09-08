/*-- Wipfamulett --*/

#strict 2

#include AMBR

// Menü zum Ablegen
public func AttachContextUnWear(object pCaller, object pClonk)
{
  [$TxtUnWear$|Image=AMWI|Condition=CheckCaller]
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
  // Geschwindigkeitsschub
  IncPhysical("Walk",  20000,3,pTarget);
  IncPhysical("Dig",   24000,3,pTarget);
  return(1);
}

protected func FxWearBlessStop(object pTarget, int iEffectNumber, int iReason, bool fTemp)
{
  if(fTemp) return(0);
  // Attribute wiederherstellen
  ResetPhysical(pTarget,"Walk");
  ResetPhysical(pTarget,"Dig");
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
  if(!iIndex--) return(MGRP);   
  if(!iIndex--) return(BORW);
  if(!iIndex--) return(GZ9Z);
  if(!iIndex--) return(LGCN);
  if(!iIndex--) return(MSRK);
*/
  if(!iIndex--) return(BORW);
  if(!iIndex--) return(LGCN);
  if(!iIndex--) return(GZ9Z);
  if(!iIndex--) return(MSRK);
  return(0);
}