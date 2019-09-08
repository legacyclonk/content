/* Schloss */

#strict

local iRefID;
local pTarget;

protected func Initialize()
{
  SetAction("None");	
}

/* Steuerung */

public func ControlDig()
{
  [$MsgTurnKey$]
  // Steckt ein Schlüssel?
  if (!Contents())  return(1, Message("$MsgNoKey$",this()), Sound("Error"));
  // Passt der Schlüssel?
  if (LocalN("iRefID", Contents()) != iRefID)  return(1, Message("$MsgWrongKey$",this()), Sound("Error"));
  if(!pTarget) return(Message("$MsgStuck$",this()));
  // Schlüssel drehen
  SetDir(1-GetDir());
  // Angeschlossenes Objekt aktivieren
  pTarget->~Activate(this());
  return(1);
}
  
public func ControlThrow(pClonk)
{
  [Schlüssel reinstecken/rausnehmen]
  if(Contents())
  {
    Exit(Contents());
    SetDir(0);
    SetAction("None");
    return(1);
  }
  // Kann der Inhalt des Clonks angenommen werden?
  var obj;
  if (obj = Contents(0, pClonk))
    if(obj->~IsKey())
      if(LocalN("iRefID", obj)) 
      {
      if(!RejectCollect(GetID(obj), obj)) Enter(this(), obj);
      if(!GetClrModulation(obj)) SetColorDw(RGB(255,255,255));
      else SetColorDw(GetClrModulation(obj));
      return(1);
      }
  return(1);
}

/* Collection */

protected func RejectCollect(idObj, pObj)
{
  // Nur Schlüssel aufnehmen
  if (!pObj->~IsKey())  return(1, Message("$MsgNoKey$",this()), Sound("Error"));
  // Nur, wenn sie einigermaßen passen
  if (LocalN("iRefID", pObj) != iRefID)  return(1, Message("$MsgWrongKey$",this()), Sound("Error"));
  // Schlüssel passt
  SetAction("Key");
  return(0);
}

public func IsSwitch() { return(true); }
