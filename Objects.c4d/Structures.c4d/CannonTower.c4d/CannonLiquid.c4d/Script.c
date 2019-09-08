/*--- Flüssigkeitsgeschütz ---*/

#strict

protected func ConnectID() { return(LICA); }
protected func GetAlternativeType() { return(ConnectID()); }

/* Wird vom Turm als Kanone erkannt */

public func IsCannon() { return(1); }

/* An Turm anschrauben */

public func Connect(object pTower)
{
  pTower->ConnectCannonDef(this(), ConnectID() );
}

/* Aufschlaggeräusch */

protected func Hit()
{
  Sound("RockHit*");
}
 
/* Forschung */  

public func GetResearchBase() { return(CTW0); }
