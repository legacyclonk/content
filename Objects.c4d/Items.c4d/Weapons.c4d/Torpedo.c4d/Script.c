/*-- Torpedo --*/

#strict

/* Initialisierung */

protected func Initialize() {
  SetAction("Walk");
  SetComDir(COMD_Stop());
}

public func Launch (object by) {
  SetAction ("Swim");
  SetDir(GetDir(by));
  SetXDir(GetXDir(by));
  SetYDir(0);
  if (GetDir()==DIR_Right()) SetComDir(COMD_Right());
  if (GetDir()==DIR_Left()) SetComDir(COMD_Left());
  return(1);
}

/* Einsammeln */

func Entrance (object container) {
  SetAction("Walk");
}

/* Effekte */

private func Swimming() {
  var iDir=(GetDir()*2-1);
  Bubble(-16*iDir,0);
  Bubble(-16*iDir,0);
  if (GetActTime() > 25) SetYDir(0);
}

/* Aufschlag */

public func Hit() { Explode(30); }

/* Produkteigenschaften */

public func IsChemicalProduct() { return(1); }

/* Status */

public func IsTorpedo() { return(1); }

/* Forschung */

public func GetResearchBase() { return(SUB1); }

