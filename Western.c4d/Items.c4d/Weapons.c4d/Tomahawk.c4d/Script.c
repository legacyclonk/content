/*-- Tomahawk --*/

#strict

/* Ist eine Handwaffe */
public func IsFightWeapon()
{
  return(1);
}

/* Kampfaktion */
public func GetFightAction()
{
  return("AxeFight");
}

/* Schlagaktion */
public func GetStrikeAction()
{
  return("AxeStrike");
}

/* Kampfgeschwindigkeit */
public func GetStrikeTime()
{
  return(3);
}

/* Kampfschaden */
public func GetStrikeDamage()
{
  return(12);
}

/* Schlag */
public func StrikeSound()
{
  Sound("AxeHit*");
}

/* Aufschlag */
protected func Hit()
{
  Sound("MetalHit*");
}

/* Werfen */

public func ControlThrow(pClonk)            // Bedienung: Werfen (benutzen)
{
  if(pClonk->~IsRiding()) return(ObjectSetAction(pClonk,"RideThrow"));
  else return(ObjectSetAction(pClonk,"ThrowTomahawk"));
}

public func Throwing(pClonk)                
{
  // Wurfparameter berechnen
  var iX, iY, iR, iXDir, iYDir, iRDir, iDir;
  iDir = GetDir(pClonk)*2-1;

  // Speerwurf
  iX = 10*iDir;
  iY = -6;
  iR = 90*iDir;
  iXDir = 5*iDir;
  iYDir = -2;
  iRDir = 50*iDir;

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

// Kein Ambosprodukt
public func IsAnvilProduct() { return(0); }

/* Kann von Indianern erforscht werden */
public func IsIndianResearch() { return(1); }

/* Kann vom Indianer in Handarbeit hergestellt werden */
public func IsIndianHandcraft() { return(1); }
