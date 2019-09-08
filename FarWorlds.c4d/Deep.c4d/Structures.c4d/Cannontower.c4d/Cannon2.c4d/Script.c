/*--- Schockwellenaufsatz ---*/

#strict

#include CTW3

protected func ConnectID() { return(CT11); }

public func Connect(object pTower)
{
  // Wir brauchen Energie
  pTower->RequireEnergy(true);
  // per PWRL connecten
/*  var pPower = CreateObject(PWRL,0,0,-1);
  SetActionTargets(pTower, this(),pPower);*/
  return(inherited(pTower));
}

/* Callbacks */

public func IsDeepSeaResearch() { return(1); }

public func GetResearchBase() { return(CTWR); }
