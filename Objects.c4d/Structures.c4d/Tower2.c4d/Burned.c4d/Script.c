/*-- Verbrannter Wachturm --*/

#strict

#include BAS3

local exit, gate_left, gate_right;

protected func Incineration()
{
  if (exit)       RemoveObject(exit);
  if (gate_left)  RemoveObject(gate_left);
  if (gate_right) RemoveObject(gate_right);
  _inherited();
}

protected func IncinerationEx() { Incineration(); _inherited(); }
protected func Destruction() { Incineration(); _inherited(); }
