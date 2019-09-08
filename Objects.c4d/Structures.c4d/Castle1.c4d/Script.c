/*-- Kleine Burg --*/

#strict

#include BAS5
#include DOOR

private func SoundOpenDoor()
{
  Sound("GateOpen");
}
  
private func SoundCloseDoor()
{
  Sound("GateClose");
}

public func IsCastle() { return true; }
