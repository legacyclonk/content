/*-- Verbranntes Büro --*/

#strict
#include BAS9

local arms, prison;

protected func Incineration()
{
  // Angezündet!
  if(!prison) return();
  // wenn Banditen fliehen können, gleich wieder löschen
  if(prison->Jailbreak()) AddEffect("Extinguish", this(), 99, 50, this());
}

public func FxExtinguishTimer(target)
{
  Extinguish(target);
  return(-1);
}
