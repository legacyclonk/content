/*-- Säurefass --*/

#strict

#include BARL

// Fassdaten
public func BarrelMaterial() { return(Material("Acid")); }
public func BarrelMaterialName() { return("Acid"); }

// Fassermittlungskette
global func GetBarrelType(iMaterial)
{
  if (iMaterial == Material("Acid")) return(ABRL);
  return(_inherited(iMaterial));
}