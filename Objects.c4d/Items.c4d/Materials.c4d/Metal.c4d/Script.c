/*-- Metall --*/

#strict
#include LOAM

protected func Initialize()
  {
  // Kein Komponentenupdate aus LOAM
  }

public func BridgeMaterial() {
  return(Material("Granite"));
}

protected func Hit() {
  Sound("MetalHit*");
  return(1);
}

private func RuleNoStructure(clonk) {
  if (FindObject(NMTT)) {
    var obj;
    while (obj = FindObject(0, 0, 1, 0, 0, 0, 0, 0, NoContainer(), obj))
      if (GetCategory(obj) & C4D_Structure())
        if (Hostile(GetOwner(obj), GetOwner(clonk))) {
          Message("$TxtNobridgeallowedhere$", this());
          Sound("Error");
          return(1);
        }
  }
  return(0);
}

private func OnBridge() {
  // Eventuelle Bewegung anhalten
  SetXDir(0, Contained());
  SetComDir(COMD_Stop(), Contained());
}

public func Activate(caller) {
  [$TxtBuildmetalbridge$]
  if (RuleNoStructure(caller)) return(0);
  return(_inherited(caller));
}

// Wir leiten von LOAM ab, sind aber kein Alchemie-Container
public func IsAlchemContainer() { return(false); }

// Und auch kein Indianerzeug
public func IsIndianResearch() { return(false); }
public func IsIndianHandcraft() { return(false); }
