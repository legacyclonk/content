/*-- Grillen --*/

#strict

protected func Initialize()
{
  SetAction("Crickets");
  SetPhase(Random(20));
  return(1);
}

private func Crickets()
{
  if (!Random(5)) Sound("Cricket*",1);
}