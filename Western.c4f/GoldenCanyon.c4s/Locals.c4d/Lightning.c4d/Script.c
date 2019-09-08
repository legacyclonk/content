/*-- Gewitter --*/

#strict

protected func Initialize()
{
  //Position festlegen
  SetPosition();
  return(1);
}

private func Thunder()
{
  //es blitzt und donnert
  if (Random(20)) return(1);
  LaunchLightning(Random(LandscapeWidth()), 0, -20, 41, +5, 15,1);
  return(1);
}
