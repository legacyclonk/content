/*--- Draht ---*/

#strict

/* Locals */

// 0,1: Drahtfarben

local Warn,
      dummy1, dummy2;   // Verbindungspunkte

protected func Initialize()
{
  // Local 0 und 1 sind die Lininenfarben
  Local(0)=22; 
  Local(1)=22;  
  // Die ersten beiden Vertices auf aktuelle Position legen
  SetVertexXY(0,GetX(), GetY());
  SetVertexXY(1,GetX(), GetY());
}

public func Connect(pObj1, pObj2)
{
  if(!Connectable(pObj1)) return();
  if(!Connectable(pObj2)) return();

  // Wenn nötig Verbindungspunkte erstellen
  Connector(pObj1, 1);
  Connector(pObj2, 2);

  SetAction("Connect",dummy1,dummy2);

  pObj1->~WireConnected(this());
  pObj2->~WireConnected(this());
  return(1); 
}

private func Connecting()
{
  // Draht zu lang
  if(ObjectDistance(GetActionTarget(),GetActionTarget(1))>150)
    return(LineTooLong());
  // Draht zu lang: warnen
  if(ObjectDistance(GetActionTarget(),GetActionTarget(1))>120)
    return(WarnLineTooLong());
  // Meldungs-Flag zurücksetzen
  Warn = false;
  // Kann nur dafür vorgesehene Objekte verbinden
  if(!Connectable(GetActionTarget(1, GetActionTarget(0)))
    || !Connectable(GetActionTarget(1, GetActionTarget(1))))
    {
    Sound("LineBreak");
    return(RemoveObject());
    }
  return(1);
}
  
protected func LineBreak()
{
  Sound("LineBreak");
}

private func LineTooLong()
{
  Sound("LineBreak");
  RemoveObject();
} 

private func WarnLineTooLong()
{
  // Meldung wurde bereits ausgegeben
  if(Warn) return(1);
  // Meldung bei Drahtrolle am einen oder anderen Ende ausgeben
  var pWarn;
  if(GetID(GetActionTarget(1, GetActionTarget()))==WIRR) pWarn = GetActionTarget();
  if(GetID(GetActionTarget(1, GetActionTarget(1)))==WIRR) pWarn = GetActionTarget(1);
  if(pWarn) Message("$MsgMaxLen$", pWarn);
  // Meldung ausgegeben: merken
  Warn = true;
  return(1);
}

// Verbindungspunkte
private func Connector(object pConnect, int ConnectNo)
{
  // Nummer
  if(ConnectNo == 1)
    {
    // Verbindungspunkt noch nicht vorhanden
    if(!dummy1)
      dummy1 = CreateObject(CNNC, 0,0, -1);
    // Neu verbinden
    dummy1->Connect(this(), pConnect);
    }
  // Nummer
  if(ConnectNo == 2)
    {
    // Verbindungspunkt noch nicht vorhanden
    if(!dummy2)
      dummy2 = CreateObject(CNNC, 0,0, -1);
    // Neu verbinden
    dummy2->Connect(this(), pConnect);
    }
}

// Prüft, ob man an einem Objekt eine Leitung anbringen kann
private func Connectable(object pObj)
{
  if(pObj->~IsWireConnectable()) return(1);
  if(pObj->GetID() == TIM1) return(1);
}
