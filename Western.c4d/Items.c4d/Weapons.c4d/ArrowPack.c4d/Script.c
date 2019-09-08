/*-- Pfeilpaket --*/

#strict
#include ARWP

/* Paketeigenschaften */

public func UnpackTo() { return(ARWI); }

// Der Köcher dient nun als Extra um die Pfeile besser zu tragen
public func GetResearchBase() { return(BOW2); }

// Kein Ambosprodukt
public func IsAnvilProduct() { return(0); }

/* Kann von Indianern erforscht werden */
// Sollen Erforschbar sein
public func IsIndianResearch() { return(1); }
//// Nur das orginal ist erforschbar, keine Überladungen
//public func IsIndianResearch() { return(GetID()==ARWP); }

/* Kann vom Indianer in Handarbeit hergestellt werden */
public func IsIndianHandcraft() { return(1); }



