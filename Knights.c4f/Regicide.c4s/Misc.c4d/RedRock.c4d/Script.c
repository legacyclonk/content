/*-- Groﬂer roter Stein --*/

#strict

protected func Initialize() 
{
  SetAction("Standard");
  var phase = Random(3);
  SetPhase(phase);
  SetSolidMask(phase * 32, 0, 32, 32);
}

protected func Damage() 
{
  if (GetDamage() < 25) return(0);
  CastObjects(FLNT, 4, 25, 0, -5);
  RemoveObject();
  return(1);
}
