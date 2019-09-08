/*-- Giftpfeilpaket --*/

#strict
#include ARWP

/* Paketeigenschaften */

public func UnpackTo() { return(PARW); }
public func MaxPackCount() {  return(6); }

// kann durch das Amulett des Gelehrten erforscht werden
public func IsJungleResearch() { return(1); }


// ohne Pfeile keine Giftpfeile
public func GetResearchBase() { return(ARP2); }
