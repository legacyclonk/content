/*--- Schwert ---*/

#strict

/* Ist eine Handwaffe */
public func IsFightWeapon()
{
  return(1);
}

/* Kampfaktion */
public func GetFightAction()
{
  return("SwordFight");
}

/* Schlagaktion */
public func GetStrikeAction()
{
  if (Random(2))
    return("SwordThrust");
  else
    return("SwordStrike");
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
  Sound("SwordHit*");
}

/* Aufschlag */
protected func Hit()
{
  Sound("SwordHit*");
  return(1);
}

/* Kann geschmiedet werden */
public func IsAnvilProduct() { return(1); }


/* Aufwertungszauberkombo: Mit Schwert wird der Clonk zum Ritter */
public func GetRevaluationCombo(object pClonk) { return(KNIG); }
