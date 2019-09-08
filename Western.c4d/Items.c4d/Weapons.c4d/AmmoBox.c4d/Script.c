/*-- Munitionskiste --*/

#strict

#include ARWP

/* Paketeigenschaften */

public func UnpackTo() { return(CSHO); }
public func IsCartridgePack() { return(1); }
public func MaxPackCount() { return(20); }

/* Keine Zerlegung */
public func Unpack(pTo) { return(); }

// Kein Ambosprodukt und kein Pfeilpaket
public func IsAnvilProduct() { return(0); }
public func IsArrowPack() { return(0); }

// Kann in der Waffenschmiede hergestellt werden
public func IsArmoryProduct() { return(1); }
