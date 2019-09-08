/*-- Eule --*/

#strict

protected func Initialize()
{
  SetAction("Owl");
  SetPhase(Random(20));
  return(1);
}

private func Owl()
{
  // Nur bei Nacht aktiv
  if (!IsNight()) return(0);
  if (!Random(30)) Sound("Owl", 1);
}
