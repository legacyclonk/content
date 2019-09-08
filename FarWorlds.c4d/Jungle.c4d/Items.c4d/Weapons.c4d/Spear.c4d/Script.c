/* Speer */

#strict

/* Objekt ist ein Speer */
public func IsSpear() { return(1); }

// Objekt darf nur einmal im Inventar sein
public func CarryLimit() { return(1); }

protected func Entrance() { SetR(); }			// damit er nicht höher rauskommt als er eigentlich sollte

public func ControlThrow(pClonk)			// Bedienung: Werfen (benutzen)
{
  ObjectSetAction(pClonk,"ThrowSpear");
  return(1);
}

public func Throwing(pClonk)				
{
  // Wurfparameter berechnen
  var iX, iY, iR, iXDir, iYDir, iRDir, iDir;
  iDir = GetDir(pClonk)*2-1;

  // Speerwurf
  iX = 10*iDir;
  iY = -6;
  iR = 90+60*iDir;
  iXDir = 5*iDir;
  iYDir = -2;
  iRDir = 4*iDir;

  // Reitet? Eigengeschwindigkeit addieren
  if(GetActionTarget(0,pClonk)) if(pClonk->~IsRiding())
  {
    iXDir += GetXDir(GetActionTarget(0,pClonk)) / 10;
    iYDir += GetYDir(GetActionTarget(0,pClonk)) / 10;
  }
  // Werfen!
  Exit(this(), AbsX(iX+GetX(pClonk)), AbsY(iY+GetY(pClonk)), iR, iXDir, iYDir, iRDir);
  pClonk->~CheckArmed();
  return(1);  
}

// kann durch das Amulett des Gelehrten erforscht werden
public func IsJungleResearch() { return(1); }