#strict

protected func Activate(iPlr) {
  MessageWindow(GetDesc(),iPlr);
}

global func NoAmmo() {
  return(ObjectCount(NOAM));
}

// Kann mittels des Spielzielauswählers ausgewählt werden
public func IsChooseable() { return(1); }
