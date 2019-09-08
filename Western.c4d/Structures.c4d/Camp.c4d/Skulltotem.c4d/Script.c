/*-- Totenschädeltotem --*/

#strict

#include TTPL

public func GetMystic()
{
  return(5);	
}

// Gibt an wie viele Objekte der Spieler davon bauen kann
public func GetBuildLimit() { return(5); }

/* Kann vom Indianer hergestellt werden */
public func IsIndianHandcraft() { return(1); }
