/*-- Säbel --*/

#strict

/* Ist eine Handwaffe */
public func IsFightWeapon()
{
  return(1);
}

/* Kampfaktion */
public func GetFightAction()
{
  return("SaberFight");
}

/* Schlagaktion */
public func GetStrikeAction()
{
  if (Random(2))
    return("SaberThrust");
  else
    return("SaberStrike");
}

/* Kampfgeschwindigkeit */
public func GetStrikeTime()
{
  return(2);
}

/* Kampfschaden */
public func GetStrikeDamage()
{
  return(8);
}

/* Schlag */
public func StrikeSound()
{
  Sound("SaberHit*");
}

/* Aufschlag */
protected func Hit()
{
  Sound("SaberHit*");
  return(1);
}

/* Kann geschmiedet werden */
public func IsAnvilProduct() { return(0); }

// Kann in der Waffenschmiede hergestellt werden
public func IsArmoryProduct() { return(1); }

/* Aufwertungszauberkombo: Mit Säbel wird der Clonk zum Kavalleristen */
public func GetRevaluationCombo(object pClonk) { return(CVRM); }
