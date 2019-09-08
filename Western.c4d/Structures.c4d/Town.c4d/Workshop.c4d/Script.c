/*-- Werkstatt --*/

#strict

#include WRKS
#include BAS9

local pAnvil;

/* Initialisierung */
protected func Initialize()
{
  pAnvil = CreateObject(ANVL, -32, 28, GetOwner());
  SetObjectOrder(this(), pAnvil);
  return(_inherited());
}
