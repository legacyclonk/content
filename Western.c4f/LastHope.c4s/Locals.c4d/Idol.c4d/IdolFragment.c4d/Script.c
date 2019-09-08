#strict

local living_time;	// Lebenszeit

protected func Initialize() {
  SetAction("Be");
}

public func Set(phase) {
  SetPhase(phase);
  var width = GetDefCoreVal("Width", "DefCore", GetID());
  var height = GetDefCoreVal("Height", "DefCore", GetID());
  SetPicture(width*phase, 0, width, height);
}

protected func SellTo() {
  return(GetComponent(0, 0));
}

// Timercall
protected func Timer() {
  if (living_time++ > 60*5) {
    Split2Components();
  }
}

protected func Hit() {
  Sound("RockHit*");
}  