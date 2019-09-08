#strict
#appendto EXTG

private func OnFire(o)
  {
  if (GetCategory(o) & 3) return(); // Burgteile nicht löschen, da man sie neu aufbauen könnte
  return (_inherited(o));
  }