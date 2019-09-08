/*-- Verbrannter Bohrturm --*/

#strict

#include DRBS

local pDrillHead;
local pBarrel;

protected func Incineration() { if (pBarrel) RemoveObject(pBarrel); pBarrel=0; if (pDrillHead) RemoveObject(pDrillHead); pDrillHead=0; }
protected func IncinerationEx() { if (pBarrel) RemoveObject(pBarrel); pBarrel=0; if (pDrillHead) RemoveObject(pDrillHead); pDrillHead=0; }
protected func Destruction() { if (pBarrel) RemoveObject(pBarrel); pBarrel=0; if (pDrillHead) RemoveObject(pDrillHead); pDrillHead=0; return(_inherited() ); }
