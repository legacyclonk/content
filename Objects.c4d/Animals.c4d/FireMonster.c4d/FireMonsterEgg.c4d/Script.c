/*-- Feuermonsterei --*/

#strict

/* TimerCall */

private func Breed()
{
  if (!Random(2000)) DoBreak();
  return(1);
}
  
/* Einwirkungen */  

protected func Hit()
{
  if (Random(2)) return(1);
  return(DoBreak());
}

protected func Damage()
{
  return(DoBreak());
}

/* Aktionen */
  
private func DoBreak()
{
  if (!ActIdle()) return(0);
  if (GBackSolid()) return(0);
  Sound("EggBreak");
  SetAction("Break");
  var pMonster = CreateConstruction(FMNS,0,0,-1,40);
  pMonster->Birth();
  return(1);
}
  
private func Destroy()
{
  RemoveObject();
  return(1);
}