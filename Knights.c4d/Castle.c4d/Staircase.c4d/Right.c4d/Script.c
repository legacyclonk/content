/*-- Treppenhaus rechts --*/

#strict

#include CPT1 // Burgteil - Treppenhausfunktion

/* Grafische Anpassung in der Burg */

protected func Initialize()
{
  // Reguläre Initialisierung
  _inherited();
  // Burgfunktion prüfen
  return(CastleChange());
}

protected func UpdateTransferZone()
{
  // Burgfunktion prüfen. Das ererbte CastleChange wuerde erneut
  // UpdateTransferZone aufrufen, also wird hier auf den Aufruf an
  // die ueberladene Funktion verzichtet.
  CastleChangeImpl();
  // Überladener Aufruf
  return(_inherited());
}

public func CastleChange()
{
  CastleChangeImpl();
  return(_inherited());
}

public func CastleChangeImpl()
{
  // Ist rechts ein Burgteil?
  var cpr;
  if (cpr=FindCastlePart(CastlePartWidth()/2+CastlePartRange()))
  {
    // Immer vor diesem Burgteil
    SetObjectOrder(cpr);
    // Aktivität anpassen
    SetAction("InCastle");
  }
  else
  {
    // Sonst idle
    SetAction("Idle");
  }
}

/* Forschung */

public func GetResearchLink() { return(CPWL); }

