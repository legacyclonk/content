/*-- Verbindungspunkt --*/

#strict

// Dies ist nur ein kleines Hilfsobjekt, zur richtigen Positionierung des Drahtes

public func Connect(pLine, pConnect)
{
  SetAction("Connect", pLine, pConnect);

  var x,y;
  GetActionTarget(1)->~WireOffset(x,y);
  SetPosition(GetX(GetActionTarget(1))+x, GetY(GetActionTarget(1))+y);
}

public func Timer()
{
  // Verschwinden
  if(!GetActionTarget() || !GetActionTarget(1))
    return(RemoveObject());
  // Versetzen
  var x,y;
  GetActionTarget(1)->~WireOffset(x,y);
  if(GetX() != x || GetY() != y)
    SetPosition(GetX(GetActionTarget(1))+x, GetY(GetActionTarget(1))+y);
}

public func IsWireConnectable() { return(1); }
