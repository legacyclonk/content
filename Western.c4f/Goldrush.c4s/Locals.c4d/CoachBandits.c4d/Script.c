/*--- Planwagen ---*/

#strict

#include COAC

/* Initialisierung */

protected func Initialize()
{
  SetAction("Drive0");  
  SetEntrance(1);
  SetGraphics(0, this(), COAC);
}

/* Status */

public func IsHorseWagon() { return(0); }
