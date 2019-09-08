/*-- Der Clonk --*/

#strict
#include CLNK
#appendto CLNK

func Construction () {
  SetGraphics(0, this, _ARC);
  return (_inherited (...));
}