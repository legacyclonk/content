/*-- Holz-Wachturm --*/

#strict

local exit;
local battlements;

#include CPW2 // Burgteilfunktion

/*-- Ausrichtung von Burgbauteilen --*/

public func CastlePartWidth() { return (46); }
public func CastlePartOffsetX() { return(2); }
public func CastlePartHeight() { return (106); }
public func CastlePartTop() { return (-4); }
public func CastlePartBasement() { return(); } // Kein Fundament

/* Initialisierung */

protected func Initialize()
{
  SetEntrance(1);
  SetSolidMask(96,0,48,106,0,0);
  // Niedergang
  exit = CreateObject(CSTE, 0, -3, GetOwner() );
  exit->Activate(this(), true);
  UpdateTransferZone();
  UpdateBattlements();
  UpdatePhase();
}

/* TimerCall */

private func CheckContents()
{
  while(Contents() ) Exit(Contents(), 0,-2,0, 0);
}

/* Steuerung */

protected func ControlLeft()
{
  [$TxtLeftgate$]
  if(GetAction() S= "Idle") return(SetAction("LeftClosed") );
  if(GetAction() S= "LeftClosed") return(AllOpen() );
  if(GetAction() S= "RightClosed") return(SetAction("BothClosed") );
  if(GetAction() S= "BothClosed") return(SetAction("RightClosed") );
}

protected func ControlRight()
{
  [$TxtRightgate$]
  if(GetAction() S= "Idle") return(SetAction("RightClosed"));
  if(GetAction() S= "RightClosed") return(AllOpen());
  if(GetAction() S= "LeftClosed") return(SetAction("BothClosed"));
  if(GetAction() S= "BothClosed") return(SetAction("LeftClosed"));
}

/* Status */

private func RightGateOpen()
{
  if(GetAction() S= "RightClosed") return(0);
  if(GetAction() S= "BothClosed") return(0);
  return(1);
}

private func LeftGateOpen()
{
  if(GetAction() S= "LeftClosed") return(0);
  if(GetAction() S= "BothClosed") return(0);
  return(1);
}

/* Tor-Aktionen */

private func AllOpen()
{
  SetAction("Idle");
  Sound("WoodenGate");
  UpdatePhase();
}

private func LeftClosed()
{
  Sound("WoodenGate");
  UpdatePhase();
}

private func RightClosed()
{
  Sound("WoodenGate");
  UpdatePhase();
}

private func BothClosed()
{
  Sound("WoodenGate");
  UpdatePhase();
}

private func UpdateBattlements()
{
  battlements = 1;
  if(FindCastlePart(-GetObjWidth(), 0) ) battlements = 3;
  if(FindCastlePart( GetObjWidth(), 0) )
  {
    if(battlements == 1) battlements = 2;
    else battlements = 0;
  }
}

private func UpdatePhase()
{
  var gatephase = 0;
  if(GetAction() S= "BothClosed") gatephase = 1;
  else if(GetAction() S= "LeftClosed") gatephase = 2;
  else if(GetAction() S= "RightClosed") gatephase = 3;
  
  SetSolidMask(96 + (48 * gatephase + 192 * battlements), 0, 48, 106, 0, 0);
}

public func CastleChange()
{
  UpdateBattlements();
  UpdatePhase();
}

/* Wenn der Turm mal runterfällt */

protected func Hit()
{
  UpdateTransferZone();
}

/* Zerstoerung */

protected func Incineration()
{
  if(exit) RemoveObject(exit);
}

protected func IncinerationEx() { Incineration(); }
protected func Destruction() { Incineration(); }

/* Transfer */

protected func UpdateTransferZone()
{
  SetTransferZone(-23,-20,46,72);
}

protected func ControlTransfer(object pObj, int tx, int ty)
{
  // Aufstieg zur Plattform
  if(Inside(tx - GetX(), -24, 24) && Inside(ty - GetY(), -24, 24) )
    if(Contained(pObj) || GetY(pObj) > (GetY() + 10) )
      return(HandleTransferPlatform(pObj, tx, ty) );

  // Abstieg von der Plattform
  if(!Contained(pObj) )
    if(Inside(GetX(pObj) - GetX(), -27, 27) && Inside(GetY(pObj) - GetY(), -30, 0) )
      if(Inside(tx - GetX(), -24, 24) && Inside(ty - GetY(), 15, 53) )
        return(HandleTransferDescend(pObj, tx, ty) );

  // Ausgang rechts
  if(Inside(tx - GetX(), 20, 28) && Inside(ty - GetY(), 0, 53) )
    return(HandleTransferGateRight(pObj, tx, ty) );

  // Ausgang links
  if(Inside(tx - GetX(), -28, -20) && Inside(ty - GetY(), 0, 53) )
    return(HandleTransferGateLeft(pObj, tx, ty) );

  // Rausklettern rechts
  if(Inside(tx - GetX(), 20, 28) && Inside(ty - GetY(), -30, -1) )
    AddCommand(pObj, "MoveTo", 0, GetX() + 22, GetY() - 15);

  // Rausklettern links
  if(Inside(tx - GetX(), -28, -20) && Inside(ty - GetY(), -30, -1) )
    AddCommand(pObj, "MoveTo", 0, GetX() - 22, GetY() - 15);

  // Transfer nicht möglich
  return(0);
}

private func HandleTransferPlatform(object pObj, int tx, int ty)
{
  // Clonk ist auf der Plattform: erledigt
  if(!Contained(pObj) )
    if(Inside(GetX(pObj) - GetX(), -24, 24) && Inside(GetY(pObj) - GetY(), -50, 0) )
      return(0);

  // Clonk ist im Turm: aufsteigen
  if(Contained(pObj) == this() )
    return(Exit(pObj, 0, -2) );

  // Clonk soll den Turm betreten
  return(AddCommand(pObj, "Enter", this()) );
}
 
private func HandleTransferDescend(object pObj, int tx, int ty)
{
  // Clonk ist im Turm: warten
  if(Contained(pObj) == this() ) return(1);

  // Clonk soll noch in die Mitte der Plattform laufen
  if(!Inside(GetX(pObj) - GetX(), -20, 20) && !Inside(GetY(pObj) - GetY(), -20, 0) )
    return(AddCommand(pObj, "MoveTo", 0, GetX(), GetY() - 10) );

  // Clonk in den Turm versetzen (geschummelt: Tastaturclonks können dies nicht :)
  Enter(this(), pObj);
  
  // Clonk soll den Turm verlassen
  return(AddCommand(pObj, "Exit") );
}
 
private func HandleTransferGateRight(object pObj, int tx, int ty)
{
  // Tor ist offen: Turm loslassen
  if(RightGateOpen() )
    if(GetAction(pObj) S= "Push" && GetActionTarget(0, pObj) == this() )
      return(AddCommand(pObj, "UnGrab", this()) );
    else
      // Tor ist offen und losgelassen: erledigt
      return(0);

  // Clonk soll den Turm anfassen
  if(GetAction(pObj) ne "Push" || GetActionTarget(0, pObj) != this() )
    return(AddCommand(pObj, "Grab", this()) );
    
  return(ControlRight(pObj) );
}

private func HandleTransferGateLeft(object pObj, int tx, int ty)
{
  // Tor ist offen: Turm loslassen
  if(LeftGateOpen())
    if(GetAction(pObj) S= "Push" && GetActionTarget(0, pObj) == this() )
      return(AddCommand(pObj, "UnGrab", this()) );
    else
      // Tor ist offen und Clonk fasst es nicht an: erledigt    
      return(0);
  
  // Clonk soll den Turm anfassen
  if(GetAction(pObj) ne "Push" || GetActionTarget(0, pObj) != this() )
    return(AddCommand(pObj, "Grab", this()) );

  // Tor öffnen
  return(ControlLeft(pObj) );
}

// Loren nicht im Turm ausleeren
public func NoLorryEjection(object lorry) { return true; }
