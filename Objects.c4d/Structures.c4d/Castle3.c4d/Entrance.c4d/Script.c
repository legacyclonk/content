/*-- Niedergang --*/

#strict

local target, add_exit;

protected func ActivateEntrance(pObj)
{
	// Nicht in verfeindete SChloesser absteigen. Ansonsten Abstieg OK (auch in verfeindete Tuerme).
	if (target && target->~IsCastle() && Hostile(GetOwner(target), GetOwner(pObj))) return;
	// Abstieg OK
	SetEntrance(1);
}

public func Activate(new_target, new_add_exit)
{
  target = new_target;
  add_exit = new_add_exit;
}

protected func Collection2(pObj)
{
  if (!target) return(0);

  Enter(target, pObj);
  if (add_exit) AddCommand(pObj, "Exit");
  SetEntrance(0);
}

// Loren nicht im Niedergang ausleeren
public func NoLorryEjection(object lorry) { return true; }
