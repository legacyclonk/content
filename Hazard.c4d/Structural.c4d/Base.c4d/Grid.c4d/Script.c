/* Lüftungsgitter */

#strict 2

protected func Initialize()
{
  SetAction("Grid");
}

protected func Damage()
{
  if (GetAction() == "Put") return(0);
  if (GetDamage() < 30) return(0);
  // Crack open
  while (Contents()) Exit(Contents());
  Sound("CrateCrack");
  Sound("Blast2");
  SetAction("Put");
  return(1);
}

/* Serialisierung */

public func Serialize(array& extra)
{
	if (GetAction() == "Put")
		extra[GetLength(extra)] = "SetAction(\"Put\")";
}
