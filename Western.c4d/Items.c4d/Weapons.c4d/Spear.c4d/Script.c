/*-- Speer --*/

#strict 2

local fThrown;

public func ControlThrow(pClonk)  // Bedienung: Werfen (benutzen)
{
  if(pClonk->~IsRiding()) return(ObjectSetAction(pClonk,"RideThrow"));
  else return(ObjectSetAction(pClonk,"ThrowSpear"));
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

  // Reitet? Fluggeschwindigkeit addieren
  if(GetActionTarget(0,pClonk))
    if(pClonk->~IsRiding()) {
      iXDir += GetXDir(GetActionTarget(0,pClonk),1);
      iYDir += GetYDir(GetActionTarget(0,pClonk),1);
    }

  // Wurfmasse setzen
  SetMass(25);
  fThrown = 1;

  // Werfen!
  Exit(this(), AbsX(iX+GetX(pClonk)), AbsY(iY+GetY(pClonk)), iR, iXDir, iYDir, iRDir);
  pClonk->~CheckArmed();
  return(1);  
}

protected func Hit()
{
  ResetMass();
  return(1);
}

protected func Entrance()
{
  SetR(); // damit er nicht höher rauskommt als er eigentlich sollte
  ResetMass();
  return(1);
}

public func ResetMass()
{
  if(!fThrown) return(0);
  SetMass(GetMass(0,GetID()));
  return(1);
}

/* Dieses Objekt ist ein Speer */
public func IsSpear() { return(1); }

/* Das Objekt darf nur einmal im Inventar sein */
public func CarryLimit() { return(1); }

/* Kann von Indianern erforscht werden */
public func IsIndianResearch() { return(1); }

/* Kann vom Indianer in Handarbeit hergestellt werden */
public func IsIndianHandcraft() { return(1); }