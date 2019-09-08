/*-- Verbrannter Wasserturm --*/

#strict

local pBarrel;
local iFillLevel;

public func GetFillLevel(pObject)
{
  var i;
  if(!pObject) return(iFillLevel + pBarrel->~GetAmount());
  if(pObject == this()) return(iFillLevel);
  if(pObject == pBarrel) return(pBarrel->~GetAmount());
}

protected func Destruction() { CastPXS("Water",GetFillLevel(),30); if(pBarrel) RemoveObject(pBarrel); iFillLevel = 0; return(_inherited()); }
protected func Incineration() { CastPXS("Water",GetFillLevel(),30); if(pBarrel) RemoveObject(pBarrel); iFillLevel = 0; return(_inherited()); }
protected func IncinerationEx() { CastPXS("Water",GetFillLevel(),30); if(pBarrel) RemoveObject(pBarrel); iFillLevel = 0; return(_inherited()); }