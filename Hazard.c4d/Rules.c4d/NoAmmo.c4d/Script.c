#strict

protected func Activate(iPlr) {
  MessageWindow(GetDesc(),iPlr);
}

global func NoAmmo() {
  return(ObjectCount(NOAM));
}

// Kann mittels des Spielzielausw�hlers ausgew�hlt werden
public func IsChooseable() { return(1); }
