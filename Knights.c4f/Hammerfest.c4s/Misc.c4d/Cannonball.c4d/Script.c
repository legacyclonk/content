/*--- Kanonelkugel ---*/

#strict

local fActivated;

public func ExplodeSize() { return(25); }

protected func Hit()
{   
  Sound("RockHit*");
  if(!fActivated) return();
  CastObjects(_BLP, RandomX(20,30), 70, 0, 0); 
  Explode(ExplodeSize());
  return(1);
}

protected func Departure(pObj)
{
  if(GetID(pObj)==CCAN) fActivated = 1; 
}

protected func Entrance()
{
  fActivated = 0; 
}

public func IsAnvilProduct() { return(1); }
