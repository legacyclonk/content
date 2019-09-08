/*-- Trommelmagazin --*/

#strict 2
#include ARWP

protected func Hit() { Sound("WoodHit*"); }

/* Paketeigenschaften */

// Damit es nicht als Patronenschachtel identifiziert wird
//public func UnpackTo() { return(CSHO); }
public func GatlingUnpackTo() { return(CSHO); }
public func IsCartridgePack() { return(0); }
public func IsGatlingPack() { return(1); }
public func MaxPackCount() { return(40); }

// Keine Zerlegung
public func Unpack(pTo) { return(0); }

// Kein Ambossprodukt und kein Pfeilpaket
public func IsAnvilProduct() { return(0); }
public func IsArrowPack() { return(0); }

// Kann in der Waffenschmiede hergestellt werden
public func IsArmoryProduct() { return(1); }

// Nur eins ist tragbar
public func CarryLimit() { return(1); }

// Masse soll immer gleich bleiben
public func UpdateMass() { return(0); }

public func JoinPack(object pContainer)
{
  // Lässt sich nur im Waffenlager zusammenlegen
  if(pContainer->~ProductCondition() != "IsArmoryProduct") return(0);
  return(_inherited(pContainer));
}

protected func SellTo(int iPlr)
{
  // Nur wiederkaufbar, wenn voll
  if(PackCount() < MaxPackCount()) return(0);
  return(GetID());
}

protected func Sale(int iPlr) { return(0); }

/* Forschung */

public func GetResearchBase() { return(GT4V); }