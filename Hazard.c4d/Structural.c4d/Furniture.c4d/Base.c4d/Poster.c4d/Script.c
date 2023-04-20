/*-- Poster --*/

#strict

protected func Initialize()
{
  SetAction("Poster");
}

public func SetRandom()
{
  // sind nur 15 Poster, nich 16
  // wenn Poster hinzugefüht werden, muss der Code hier verändert werden. :(
  var y, x = Random(4);
  if(x == 3) y = Random(3);
  else       y = Random(4);
  Set(x,y);
}

public func Set(int x, int y)
{
  SetPhase(x);
  SetDir(y);
}

/* Serialisierung */

public func Serialize(array& extra)
{
	extra[GetLength(extra)] = Format("Set(%d,%d)",GetPhase(),GetDir());
}
