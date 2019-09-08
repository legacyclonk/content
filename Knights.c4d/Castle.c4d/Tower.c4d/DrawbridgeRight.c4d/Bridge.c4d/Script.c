/*-- Zugbrücke (rechts) --*/

#strict

#include CPBL

/* Konfiguration */

public func GetTowerType() { return(CPT3); }

private func FindTower() { return(FindObject(GetTowerType(), -40,0)); }
