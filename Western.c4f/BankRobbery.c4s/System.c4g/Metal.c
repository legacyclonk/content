/*-- Metall --*/

#strict
#appendto METL

// Man soll nicht mit unzerstörbarem Granit bauen können :-)
public func BridgeMaterial() {
  return(Material("Rock"));
}
