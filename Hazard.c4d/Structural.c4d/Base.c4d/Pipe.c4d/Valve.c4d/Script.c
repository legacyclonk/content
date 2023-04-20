/*-- Ventil --*/

#strict

local open;

protected func Initialize() {
  open = false;
  SetAction("Exist");
}

protected func ControlDig() {
  open = !open;
  UpdatePipesystem();
}

public func Status() { return(open); }

/* Serialisierung */

public func Serialize(array& extra)
{
	if (open)
		extra[GetLength(extra)] = "ControlDig()";
}
