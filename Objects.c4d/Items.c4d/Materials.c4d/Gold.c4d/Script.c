/*-- Gold --*/

#strict

/* Aufschlag */

protected func Hit()
{
  Sound("RockHit*");
  return(1);
}

/* Für Goldmine */

public func Sale(plr) {
  var obj;
  if (obj = FindObject (GLDM, 50 - GetX (), 49 - GetY ()))
    obj -> GLDM::PlayerHasSold(plr, this ());
}

public func Purchase(int plr, object buyobj) {
  var obj;
  if (obj = FindObject (GLDM, 50 - GetX (), 49 - GetY ()))
    obj -> GLDM::PlayerHasBought(plr, this ());
}

func IsAlchemContainer() { return(true); }
func AlchemProcessTime() { return(180); }
