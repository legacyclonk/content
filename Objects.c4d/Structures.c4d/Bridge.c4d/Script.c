/*-- Brückensegment --*/

#strict

local is_locked;	// Ist im Boden verankert?

protected func Initialize() {
  // Im Freien erzeugte Brückensegmente sind verankert
  if (!Contained())
    Lock(1, 1);
  return(1);
}

protected func Entrance() {
  // Im Behälter: lösen
  Release(1);
  return(1);
}

public func ControlDigDouble(caller) {
  [$TxtLock$]
  // Wenn verankert lösen
  if (is_locked) Release();
  // sonst verankern
  else Lock();
  return(1);
}

private func Lock(quiet, dont_descend) {
  SetAction("Locked");
  SetSolidMask(0, 11, 72, 3);
  is_locked = 1;

  if (!quiet)
    Sound("Connect");

  // Verankern
  if (!dont_descend)
    if (!GBackSolid(0, 10))
      SetPosition(GetX(), GetY()+10);
  return(1);
}

private func Release(quiet, dont_ascend) {
  SetAction("Idle");
  SetSolidMask();
  is_locked = 0;

  if (!quiet)
    Sound("Connect");

  // Aus der Erde lösen
  if (!dont_ascend) {
    var ascend_pixels = 0;
    while (Stuck() && (++ascend_pixels < 12))
      SetPosition(GetX(), GetY()-1);
  }

  return(1);
}

public func IsBridge() { return(1); }

public func IsLocked() { return(is_locked); }

// Nicht an die Dampflok koppeln lassen
public func NoPull()
{
  return IsLocked();
}
