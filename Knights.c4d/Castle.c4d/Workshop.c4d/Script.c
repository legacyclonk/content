/*-- Burg-Werkstatt --*/

#strict

#include DOOR // Türsteuerung
#include WRKS // Werkstatt
#include CPW2 // Burgteilfunktion

/* Burgbauteil */

public func CastlePartWidth() { return (80); }
public func CastlePartHeight() { return (74); }
public func CastlePartTop() { return (-13); }
public func CastlePartBasement() { return(BAS8); }

protected func Initialize()
{
  // Ursprungsfunktion
  _inherited();
  // Amboss erzeugen
  CreateObject(ANVL, -20, +37, GetOwner());
}

/* Forschung */

public func GetResearchBase() { return(CPW2); }
