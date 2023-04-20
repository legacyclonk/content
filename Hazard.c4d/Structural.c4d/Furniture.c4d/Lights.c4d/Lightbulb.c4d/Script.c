/*-- Glühbirne - Grundobjekt --*/

#strict

local bulb;

public func Initialize() {
  bulb = CreateObject(BULB);
  bulb->Init(this(),50); 
  return(1);
}

public func Set(int Length) {
  bulb->Set(Length);
}

public func Destruction() {
  if(bulb)
    RemoveObject(bulb);
}

/* Serialisierung */

public func Serialize(array& extra)
{
	_inherited(extra);
	extra[GetLength(extra)] = Format("Set(%d)", bulb->LocalN("length"));
}