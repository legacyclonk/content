/*-- Toilette --*/

#strict

func Initialize()
{
  SetAction("Open");  
  SetDir(Random(2));
}

/* Serialisierung */

public func Serialize(array& extra)
{
	if (GetDir())
		extra[GetLength(extra)] = "SetDir(1)";
}
