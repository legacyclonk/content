/*-- Funke --*/

#strict

protected func Process()
{
  if (Random(10)+10 < GetActTime()) DoCon(-25);
  return(1);
}

protected func Completion()
{
  DoCon(Random(100));
  SetAction("Sparkle");
  return(1);
}

protected func Hit()
{
  return(DoCon(-50));
}

/* Globaler Funktionsaufruf */

global func Sparkle(int iAmount, int iX, int iY)
{
  CastObjects(SPRK, iAmount/3 +2, iAmount, iX, iY);
  return(1);
}
