/*-- Munitionsgürtel --*/

#strict

func Initialize()
{
  SetPosition(1,1); //ins linke obere Eck setzten
  SetVisibility(VIS_None); //und verstecken!
  return(1);
}

func IsAmmoStorage() { return(1); }

/* Serialisierung */

public func NoSerialize() { return !false; }
