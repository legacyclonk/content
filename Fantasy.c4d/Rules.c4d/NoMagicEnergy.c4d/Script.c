/*-- Keine Zauberenergie --*/

#strict

protected func Activate(iPlr) {
  MessageWindow(GetDesc(),iPlr);
}

global func GetMagicEnergy(object pObject)
{
  // wenn aktiviert, immer 1000 zurückgeben
  if(ObjectCount(NMGE)) return(1000);
  return(inherited(pObject));
}

global func DoMagicEnergy(int iChange, object pObject, bool fAllowPartial)
{
  // wenn aktiviert,
  if(ObjectCount(NMGE))
  {
    // Energie abziehen immer erfolgreich
    if(iChange<0) return(true);
    // Energie draufrechnen nie erfolgreich (weil theoretisch "voll aufgetankt")
    if(iChange>0) return(false);
    return(true);
  }
  return(inherited(iChange, pObject, fAllowPartial));
}
