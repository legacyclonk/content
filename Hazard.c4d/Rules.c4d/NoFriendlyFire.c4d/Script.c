/*-- No Friendly Fire --*/

#strict

protected func Activate(iPlr) {
  MessageWindow(GetDesc(),iPlr);
}

global func NoFriendlyFire() {
  return(ObjectCount(NOFF));
}

global func FriendlyFire() {
  return(!NoFriendlyFire());
}

// Kann mittels des Spielzielausw�hlers ausgew�hlt werden
public func IsChooseable() { return(1); }
