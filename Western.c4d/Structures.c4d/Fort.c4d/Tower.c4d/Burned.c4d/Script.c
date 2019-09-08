/*-- Verbrannter Wachturm --*/

#strict

local pExit, pGrab;

protected func Incineration() { if(pExit) RemoveObject(pExit); if(pGrab) RemoveObject(pGrab); }
protected func IncinerationEx() { Incineration(); _inherited(); }
protected func Destruction() { Incineration(); _inherited(); }
