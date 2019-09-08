/*-- Burg --*/

#strict

#include BAS8
#include DOOR

/* Initialisierung */

protected func Initialize()
{
  SetObjectOrder(this(), CreateObject(TWR1,-23,+44,GetOwner()));
}
  
/* Türsteuerung */

private func SoundOpenDoor()
{
  Sound("GateOpen");
}
  
private func SoundCloseDoor()
{
  Sound("GateClose");
}
  
/* Forschung */

public func GetResearchBase() { return(CST1); }


/* Status */

public func IsCastle() { return true; }
