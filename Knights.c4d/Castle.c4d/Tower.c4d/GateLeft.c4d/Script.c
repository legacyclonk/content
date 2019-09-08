/*-- Wachturm-Tor --*/

#strict

/* Initialisierung */

protected func Initialize()
{
  SetComDir(COMD_Up());
  return(1);
}

/* Außer Reichweite */

private func CheckRange()
{
  if (!GetActionTarget()) return();
	var iY = GetActionTarget()->GetY();
	if (GetY() < iY - 10) SetComDir(COMD_Down());
	if (GetY() > iY + 30) SetComDir(COMD_Up());
}

/* Burgteilfunktion */

private func DestructCheck()
{
  if (!GetActionTarget()) 
		return(RemoveObject());
	if (GetActionTarget()->GetID() != CPT2)
		return(RemoveObject());
}

public func CastleChange() 
{ 
	return(DestructCheck()); 
}

/* Steuerung */

public func Activate()
{
  Sound("Click");
  Sound("SteelGate1");
  if (GetComDir() == COMD_Up())
  	return(Close());
  else
  	return(Open());
}

private func Close()
{
  if (!GetActionTarget()) return(0);
	if (GetY() > GetActionTarget()->GetY() + 30) return(0);
  SetComDir(COMD_Down());
  return(1);
}

private func Open()
{
  if (!GetActionTarget()) return(0);
	if (GetY() < GetActionTarget()->GetY() - 10) return(0);
  SetComDir(COMD_Up());
  return(1);
}
