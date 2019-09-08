/*-- Clonk mit Fischhaut --*/

#strict

#include MCLK

protected func ContactBottom()
  {
  if (SEqual(GetAction(), "Swim"))
    SetAction("Walk");
  return(1);
  }

public func OnRevaluation()
  {
  // Die Aufwertung zum Fischclonk passiert bei magiebegabten Clonks
  //  und gewöhnlich nicht im Zauberturm
  // Hier also keine Zauberenergie geben, damit man sich nicht endlos
  //  Zauberenergie ercheaten kann!
  return true;
  }
  