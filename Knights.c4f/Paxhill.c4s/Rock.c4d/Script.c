/*-- Groﬂer Stein --*/

#strict 2

protected func Initialize() 
{
  SetAction("Standard");
  var phase = Random(6);
  SetPhase(phase);
  SetSolidMask(phase * 32, 0, 32, 32);
}

protected func Damage() 
{
  if (GetDamage() < 50) return false;
  CastObjects(ROCK, 4, 15, 0, -5);
  RemoveObject();
  return true;
}
