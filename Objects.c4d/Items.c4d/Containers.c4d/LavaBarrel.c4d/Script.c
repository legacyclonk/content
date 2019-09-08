/*-- Lavafass --*/

#strict

#include BARL

// Fassdaten
public func BarrelMaterial()
  {
  // Wenn möglich tatsächliches Füllmaterial ausgeben
  if (iFillMaterial) return (iFillMaterial-1);
  // iFillMaterial nicht gesetzt bei Fässern aus alten Spielständen, oder z.B. bei gekauften Fässern
  // Dann ist das Fass mit normaler Lava gefüllt
  return(Material("Lava"));
  }
  
public func BarrelMaterialName()
  {
  // Wenn möglich tatsächliches Füllmaterial ausgeben
  if (iFillMaterial) return (MaterialName(iFillMaterial-1));
  return("Lava");
  // iFillMaterial nicht gesetzt bei Fässern aus alten Spielständen, oder z.B. bei gekauften Fässern
  // Dann ist das Fass mit normaler Lava gefüllt
  }

// Fassermittlungskette
global func GetBarrelType(iMaterial)
{
  if (iMaterial == Material("Lava")) return(LBRL);
  if (iMaterial == Material("DuroLava")) return(LBRL);
  return(_inherited(iMaterial));
}