/*-- Verbranntes Schloss --*/

#strict

#include BAS9

local entr_left, entr_right;

/* Fundament und Niedergänge entfernen */

protected func Destruction()
{
  if (entr_left) RemoveObject(entr_left);
  if (entr_right) RemoveObject(entr_right);
  return(_inherited() );
}

protected func Incineration()
{
  if (entr_left) RemoveObject(entr_left);
  if (entr_right) RemoveObject(entr_right);
  return(_inherited() );
}

protected func IncinerationEx()
{
  if (entr_left) RemoveObject(entr_left);
  if (entr_right) RemoveObject(entr_right);
  return(_inherited() );
}
