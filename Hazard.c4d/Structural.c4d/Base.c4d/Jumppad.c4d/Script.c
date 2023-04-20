/*-- Jumppad --*/

#strict

local top;

// Einstellung

public func Set(int str, int a, int a2)
{
	SetR(a2);
  if(!top) 
	{
		top = CreateObject(JPTP, 0,0, GetOwner());
		top -> SetAction("Attach", this());
	}
  top -> SetR(a);
  top -> SetStr(str);
}

// Initialisierung

protected func Initialize()
{
  SetAction("Float");
}

/* Serialisierung */

public func Serialize(array& extra)
{
	if (GetR())
		extra[GetLength(extra)] = Format("Set(%d,%d,%d)",top->LocalN("strength"),top->GetR(),GetR());
	else
		extra[GetLength(extra)] = Format("Set(%d,%d)",top->LocalN("strength"),top->GetR());
}
