/*-- Verbrannter Holz-Wachturm --*/

#strict

local exit;
local battlements;

/* Zerstörung */

protected func Incineration()
{
  if (exit) RemoveObject(exit);
}

protected func IncinerationEx() { Incineration(); }
protected func Destruction() { Incineration(); }
