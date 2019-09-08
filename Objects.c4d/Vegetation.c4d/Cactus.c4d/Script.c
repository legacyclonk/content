/*-- Kaktus --*/

#strict

public func Life()
{
  if (!Random(20000))
    PlaceVegetation(GetID(this()), -200, -200, 400, 400, 10);
  return(1);
}
