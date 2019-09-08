/*-- Pferd --*/

#strict

#include HORS

// Lässt sich wiederkaufen
public func Sale(int iByPlr) {
  DoHomebaseMaterial(iByPlr,SADL,1);
  return(_inherited(iByPlr));
}

public func SellTo() { return(HORS); }