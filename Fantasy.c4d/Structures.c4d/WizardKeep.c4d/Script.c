/*-- Zauberkastell --*/

#strict

#include WTWR

local entr;

// Hat ein größeres Fundament als der Zauberturm
public func BasementID()
{
  return(BAS4);
}

/* Initialisierung */

protected func Initialize()
{
  _inherited();
  entr = CreateObject(CSTE, 17, -29, GetOwner() );
  entr->Activate(this(), false);
  UpdateTransferZone();
}

/* Niedergang bei Zerstoerung oder Brand loeschen */

protected func Destruction()
{
  if(entr) entr->RemoveObject();
  return(_inherited() );
}

protected func Incineration()
{
  if(entr) entr->RemoveObject();
  return(_inherited() );
}

protected func IncinerationEx()
{
  if(entr) entr->RemoveObject();
  return(_inherited() );
}

/* Aufstieg via Zauber-Menu */

/*public func SelectSpell(object caller) Nicht mehr nötig, da nun über das Kontextmenü erreichbar...
{
  inherited(caller);
  if(caller->Contained() == this() )
    AddMenuItem("$TxtAscend$", "Ascend", MASC, caller, 0, caller);
}*/

/* Aufstieg */

public func Ascend(caller, object menu_caller)
{
  if(menu_caller) caller = menu_caller;
  Exit(caller, entr->GetX() - GetX(), entr->GetY() - GetY() + entr->GetObjHeight() / 2);
}

/* Kontextmenü */

protected func ContextAscend(object pClonk)
{
  [$TxtAscend$|Image=MASC|Condition=IsComplete]
  pClonk->ContainedCall("Ascend", this());
}

private func IsComplete()
{
  return (GetCon() >= 100); 
}

/* Transfer */

protected func UpdateTransferZone()
{
  SetTransferZone(-4, -38, 37, 82);
}

protected func ControlTransfer(object obj, int x, int y)
{
  if(Hostile(GetOwner(), obj->GetOwner()) ) return(0);

  // Abstieg
  if(Inside(obj->GetX(), GetX() - 4, GetX() + 41) && Inside(obj->GetY(), GetY() - 43, GetY() - 25) )
    if(y > GetY() - 25)
      return(HandleTransferDescend(obj, x, y) );

  // Aufstieg
  if(Inside(x, GetX() - 4, GetX() + 41) && Inside(y, GetY() - 43, GetY() - 25) )
    return(HandleTransferAscend(obj, x, y) );

  return(0);
}

private func HandleTransferAscend(object obj, int x, int y)
{
  // Schon am Ziel?
  if(Inside(obj->GetX(), GetX() - 4, GetX() + 41) && Inside(obj->GetY(), GetY() - 43, GetY() - 25) && obj->Contained() != this() )
    return(0);

  // Kastell betreten
  AddCommand(obj, "Call", this(), 0,0,0,0, "Ascend");
  if(obj->Contained() != this() )
    AddCommand(obj, "Enter", this() );

  return(1);
}

private func HandleTransferDescend(object obj, int x, int y)
{
  if(!Inside(obj->GetX(), GetX() - 4, GetX() + 41) || !Inside(obj->GetY(), GetY() - 43, GetY() - 25) || obj->Contained() == this() )
    return(0);

  return(AddCommand(obj, "Enter", entr) );
}

