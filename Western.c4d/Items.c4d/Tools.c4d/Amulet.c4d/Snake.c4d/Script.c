/*-- Schlangenamulett --*/

#strict 2

#include AMBR

// Menü zum Ablegen
public func AttachContextUnWear(object pCaller, object pClonk)
{
  [$TxtUnWear$|Image=AMSN|Condition=CheckCaller]
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
  // Schutz vor Säure
  IncPhysical("CorrosionResist",1,3,pTarget);
  return(1);
}

protected func FxWearBlessStop(object pTarget, int iEffectNumber, int iReason, bool fTemp)
{
  if(fTemp) return(0);
  // Attribute wiederherstellen
  ResetPhysical(pTarget,"CorrosionResist");
  // Bei Tod Amulett verlieren
  var pAmulet = FindObject(0,0,0,0,0,0,"WearAmulet",pTarget);
  if(pAmulet && iReason==4) ObjectSetAction(pAmulet,"Idle");
  SetCategory(C4D_Object,pAmulet);
  return(1);
}

protected func FxWearBlessDamage(object pTarget, int iEffectNumber, int iDmgEngy, int iCause)
{
  if(iDmgEngy > 0) return(iDmgEngy);
  return(iDmgEngy*3/4);
}

protected func FxWearBlessEffect(string szNewEffectName)
{
  // keine Vergiftungen zulassen
  if(szNewEffectName == "Poison") return(-1);
}

public func GetSpell(iIndex)
{
// Von nun an gibt es neue Zauber
/*
  if(!iIndex--) return(MBRG);
  if(!iIndex--) return(MGPL);
  if(!iIndex--) return(MCLT);
  if(!iIndex--) return(CMFG);
  if(!iIndex--) return(XCRS);
*/
  if(!iIndex--) return(XCRS);
  if(!iIndex--) return(LGCN);
  if(!iIndex--) return(MGPL);
  if(!iIndex--) return(CMFG);
  return(0);
}