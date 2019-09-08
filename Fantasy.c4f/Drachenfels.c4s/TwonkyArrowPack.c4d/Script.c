/*-- Twonkys Pfeilpaket --*/

#strict
#include ARWP

/* Initialisierung */

/* Aufschlag */

protected func Hit()
{
  Sound("WoodHit");
}

/* Paketeigenschaften */

public func UnpackTo() { return(_TAR); }
public func MaxPackCount() { return(20); }

