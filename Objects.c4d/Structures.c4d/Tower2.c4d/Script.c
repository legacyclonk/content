/*-- Wachturm --*/

#strict

#include BAS3
#include DOOR

local exit, gate_left, gate_right;

/* Initialisierung */

protected func Initialize() {
  // Tore erzeugen
  gate_left  = CreateObject(GATL, -14, +10, GetOwner());
  ObjectSetAction(gate_left,  "GateLeft",  this());
  gate_right = CreateObject(GATR, +15, +10, GetOwner());
  ObjectSetAction(gate_right, "GateRight", this());
  // Ausgang erzeugen
  exit = CreateObject(CSTE, 0, -20, GetOwner());
  exit->Activate(this(), true);
  // Transferzone setzen
  UpdateTransferZone();
  return(1);
}

/* Kontext */ 

protected func Incineration()
{
  if (exit)       RemoveObject(exit);
  if (gate_left)  RemoveObject(gate_left);
  if (gate_right) RemoveObject(gate_right);
}

protected func IncinerationEx() { Incineration(); }
protected func Destruction() { Incineration(); _inherited(); }
 
public func ContextGateLeft(caller) {
  [$TxtLeftgate$|Image=GATL|Condition=IsFullCon]
  SetCommand(caller, "Grab", this());
  AppendCommand(caller, "Call", this(), 0, 0, 0, 0, "ControlLeft");  
  return(1); 
}
  
public func ContextGateRight(caller) {
  [$TxtRightgate$|Image=GATL|Condition=IsFullCon]
  SetCommand(caller, "Grab", this());
  AppendCommand(caller, "Call", this(), 0, 0, 0, 0, "ControlRight");  
  return(1);
}
  
private func IsFullCon() { return(GetCon() >= 100); }

/* Steuerung */

protected func ControlLeft(caller) {
  [$TxtLeftgate$]
  var gate = FindObject(0, 0, 0, 0, 0, 0, "GateLeft", this());
  if (!gate) return(0);
  gate->Activate();
  return(1);
}
  
protected func ControlRight(caller) {
  [$TxtRightgate$]
  var gate = FindObject(0, 0, 0, 0, 0, 0, "GateRight", this());
  if (!gate) return(0);
  gate->Activate();
  return(1);
}
 
/* Inhalt auf die Plattform versetzen */

protected func DoorClosed() {
  while (Contents()) Exit(Contents(), 0, -20, 0, 0);  
}

/* Wenn der Turm mal runterfällt */

protected func Hit() {
  UpdateTransferZone();
}

/* Transfer */

protected func UpdateTransferZone() {
  SetTransferZone(-23, -40, 46, 72);
}

protected func ControlTransfer(object pObj, int tx, int ty)
{
  // Aufstieg zur Plattform
  if(Inside(tx - GetX(), -24, 24) && Inside(ty - GetY(), -50, -24) )
    if(GetY(pObj) > GetY() - 24)
      return(HandleTransferPlatform(pObj, tx, ty) );
  
  // Abstieg von der Plattform    
  if(Inside(GetX(pObj) - GetX(), -24, 24) && Inside(GetY(pObj) - GetY(), -50, -24) )
    if(Inside(tx - GetX(), -24, 24) && Inside(ty - GetY(), -24, 32) )
      return(HandleTransferDescend(pObj, tx, ty) );
      
  // Ausgang rechts
  if(Inside(tx - GetX(), 20, 28) && Inside(ty - GetY(), -24, 32) )
    return(HandleTransferGateRight(pObj, tx, ty) );
  
  // Ausgang links
  if(Inside(tx - GetX(), -28, 20) && Inside(ty - GetY(), -24, 32) )
    return(HandleTransferGateLeft(pObj, tx, ty) );

  return(0);
}

private func HandleTransferPlatform(object pObj, int tx, int ty)
{
  // Clonk ist auf der Plattform: erledigt
  if(Inside(GetX(pObj) - GetX(), -24, 24) && Inside(GetY(pObj) - GetY(), -40, -24) )
    return(0);

  // Clonk ist im Turm: warten
  if(Contained(pObj) == this() )
    return(1);

  // Clonk soll den Turm betreten
  return(AddCommand(pObj,"Enter",this()) );
}
  
private func HandleTransferDescend(object pObj, int tx, int ty)
{
  // Clonk ist im Turm: warten
  if(Contained(pObj) == this() )
    return(1);
  // Clonk in den Turm versetzen (geschummelt: Tastaturclonks können dies nicht :)
  Enter(this(),pObj);
  // Clonk soll den Turm verlassen
  return(AddCommand(pObj,"Exit") );
}
 
private func HandleTransferGateRight(object pObj, int tx, int ty)
{
  // Tor suchen
  if (!gate_right) return(0);
  
  // Tor ist offen: Turm loslassen
  if(GetY(gate_right) - GetY() < 0)
    if(GetAction(pObj) S= "Push" && GetActionTarget(0,pObj) == this() )
      return(AddCommand(pObj, "UnGrab", this()) );
       
  // Tor ist offen: Erledigt
  if(GetY(gate_right) - GetY() < 0) return(0);
  
  // Clonk soll den Turm anfassen
  if((GetAction(pObj) ne "Push") || (GetActionTarget(0, pObj) != this()) )
    return(AddCommand(pObj, "Grab", this()) );
    
  // Tor bewegt sich: warten
  if (GetComDir(gate_right) == COMD_Up()) return(1);

  // Tor öffnen
  return(ControlRight(pObj));
}

private func HandleTransferGateLeft(object pObj, int tx, int ty)
{
  // Tor suchen
  if (!gate_left) return(0);
 
  // Tor ist offen: Turm loslassen
  if(GetY(gate_left) - GetY() < 0)
    if(GetAction(pObj) S= "Push" && GetActionTarget(0, pObj) == this() )
      return(AddCommand(pObj, "UnGrab", this()) );
      
  // Tor ist offen: Erledigt
  if(GetY(gate_left) - GetY() < 0) return(0);
  
  // Clonk soll den Turm anfassen
  if( (GetAction(pObj) ne "Push") || (GetActionTarget(0, pObj) != this()) )
    return(AddCommand(pObj, "Grab", this()) );
  
  // Tor bewegt sich: warten
  if(GetComDir(gate_left) == COMD_Up() ) return(1);
  
  // Tor öffnen
  return(ControlLeft(pObj) );
}

// Loren nicht im Turm ausleeren
public func NoLorryEjection(object lorry) { return true; }
