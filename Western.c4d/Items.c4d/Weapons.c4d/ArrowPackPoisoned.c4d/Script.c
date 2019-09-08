/*-- Pfeilpaket --*/

#strict

#include ARWP

/* Paketeigenschaften */

public func UnpackTo() { return(TARW); }
public func IsAnvilProduct() { return(0); }

public func GetResearchBase() { return(ARPI); }

/* Kann von Indianern erforscht werden */
public func IsIndianResearch() { return(1); }

/* Kann vom Indianer in Handarbeit hergestellt werden */
public func IsIndianHandcraft() { return(1); }

