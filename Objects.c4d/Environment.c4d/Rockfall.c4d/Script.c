/*-- Steinschlag --*/

#strict

protected func Initialize()
{
  // Eigene Position festlegen
  SetPosition();
  return(1);
}

/* Eventuell Steinschlag erstellen */
private func DoRocks()
{
  if (Random(150)) return(1);
  var pRock = CreateObject(RCK2, Random(LandscapeWidth()), 0, -1);
  SetXDir(25 - Random(51), pRock);
  SetYDir(20 - Random(16), pRock);
  return(1);
}
