/*-- Batterie --*/

#strict

/* Aufschlag */

protected func Hit()
{
  Sound("MetalHit*");
}

/* Produkteigenschaften */

public func IsChemicalProduct() { return(1); }

/* Forschung */

public func GetResearchBase() { return(ACT3); }
