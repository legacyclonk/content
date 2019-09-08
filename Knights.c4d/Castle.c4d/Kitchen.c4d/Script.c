/*-- Burg-Küche --*/

#strict

#include CPW2 // Burgteilfunktion
#include DOOR // Tuersteuerung

/* Initialisierung */

protected func Initialize()
{
  // Objekte im Umkreis ihre Überprüfungen durchführen lassen
  _inherited();
  // Topf und Feuerstelle erzeugen
  CreateObject(CLD2, 20, 28,GetOwner() ); //->SetKitchen(this() );
  CreateObject(STOV, 20, 37,GetOwner() ); //->SetKitchen(this() );
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

/* Ausrichtung von Burgbauteilen */

public func CastlePartWidth() { return (80); } 
public func CastlePartHeight() { return (74); }
public func CastlePartTop() { return (-13); }
public func CastlePartBasement() { return(BSC3); }

/* Status */

public func IsKitchen() { return(1); }

/* Forschung */

public func GetResearchBase() { return(CPW2); }

