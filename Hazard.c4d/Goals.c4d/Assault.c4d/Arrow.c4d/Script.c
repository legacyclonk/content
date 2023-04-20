/*-- Pfeil --*/

#strict

protected func Initialize() { SetAction("Turn"); }

global func SetArrow(iX,iY) {
  RemoveObject(FindObject(_AR2));
  CreateObject(_AR2,iX,iY,-1);
}
  
global func RemoveArrow() {
  RemoveObject(FindObject(_AR2));
}
