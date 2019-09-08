/*-- Axt --*/

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

/* Kann geschmiedet werden */
public func IsAnvilProduct() { return(1); }
