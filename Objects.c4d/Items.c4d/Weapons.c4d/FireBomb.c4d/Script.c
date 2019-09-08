/*-- Brandbombe --*/

#strict

local pProduced;

/* Fertigstellung */
protected func Initialize()
{
  // Wurden wir produziert? Dann gibts das leere Ölfass zurück
  if(pProduced && Contained() == pProduced)
    CreateContents(BARL, pProduced);
}

/* Aufschlag */

protected func Hit()
{
  CastObjects(DFLM, 20, 20);
  Sound("Inflame");
  Sound("Fuse");
  RemoveObject();
  return(1);
}

/* Produktion */
public func OnStartProduction(pChemicalPlant)
{
  pProduced = pChemicalPlant;
}

/* Produkteigenschaften */

public func IsChemicalProduct() { return(1); }

/* Forschung */

public func GetResearchBase() { return(FLNT); }
