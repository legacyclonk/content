/*-- Br�ckensegment --*/

#strict

local is_locked;	// Ist im Boden verankert?

protected func Initialize() {
  // Im Freien erzeugte Br�ckensegmente sind verankert
  if (!Contained())
    Lock(1, 1);
  return(1);
}

protected func Entrance() {
  // Im Beh�lter: l�sen
  Release(1);
  return(1);
}

public func ControlDigDouble(caller) {
  [$TxtLock$]
  // Wenn verankert l�sen
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
    if (!GBackSolid(0, 11))
      SetPosition(GetX(), GetY()+11);
  return(1);
}

private func Release(quiet, dont_ascend) {
  SetAction("Idle");
  SetSolidMask();
  is_locked = 0;

  if (!quiet)
    Sound("Connect");

  // Aus der Erde l�sen
  if (!dont_ascend) {
    var ascend_pixels = 0;
    while (Stuck() && (++ascend_pixels < 12))
      SetPosition(GetX(), GetY()-1);
  }

  return(1);
}

public func IsBridge() { return(1); }

public func IsLocked() { return(is_locked); }
