/*-- Pfeilpaket --*/

#strict
#include ARWP

/* Paketeigenschaften */

public func UnpackTo() { return(ARW2); }
public func MaxPackCount() {  return(6); }

// kann durch das Amulett des Gelehrten erforscht werden
public func IsJungleResearch() { return(1); }

// ohne Blasrohr keine Pfeile
public func GetResearchBase() { return(BLTB); }
