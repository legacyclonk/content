/*--- Drahtrolle ---*/

#strict

protected func Hit() {
  Sound("WoodHit*");
}

public func Activate(pByObj)
{
  [$Connect$|Image=WIRE]
  var pConnect, pCheck, pLine;
  // Anschlusspunkt suchen
  while(pCheck = FindObject(0,1, 0,0,0,0,0,0, NoContainer(), pCheck))
    if(pCheck->~IsWireConnectable())
      if(! pCheck->~MaxConnections())
        if(! pConnect)
          pConnect = pCheck;

  // Kein Anschlusspunkt gefunden
  if(!pConnect) return(Message("$MsgCantConnect$", pByObj));

  // Draht wird bereits verlegt: verbinden
  if(pLine = FindConnectedWire(this()))
    {
    // Draht anschlieﬂen
    if(! ReconnectWire(pLine, pConnect))
      // Es ist ein Fehler aufgetreten
      return(Message("$MsgCantConnect$", pByObj));
    else
      return(RemoveObject());
    }
  // Draht wird noch nicht verlegt: neuen Draht erzeugen
  pLine = CreateObject(WIRE, 0,0, GetOwner());
  pLine->Connect(pConnect, this());

  Sound("Connect");
  return(1);
}

private func FindConnectedWire(pObj)
{
  // Leitung finden und zur¸ckgeben
  return(GetActionTarget(0, FindObject(CNNC, 0,0,0,0,0, "Connect", pObj)));
}

private func ReconnectWire(pLine, pConnect)
{
  // Keine Kurzschl¸sse
  if(GetActionTarget(1, GetActionTarget(0, pLine)) == pConnect || GetActionTarget(1, GetActionTarget(1, pLine)) == pConnect)
    return();

  // Draht weitergeben
  if(GetActionTarget(1, GetActionTarget(0, pLine)) == this())
    {
    pLine->Connect(pConnect, GetActionTarget(1, GetActionTarget(1, pLine)));
    return(Sound("Connect"));
    }
  if(GetActionTarget(1, GetActionTarget(1, pLine)) == this())
    {
    pLine->Connect(GetActionTarget(1, GetActionTarget(0, pLine)), pConnect);
    return(Sound("Connect"));
    }
  // Unerwarteter Fehler
  return(0);
}

// Kann auf dem Amboﬂ hergestellt werden
public func IsAnvilProduct() { return(1); }
// Kann Telegrafendr‰hte verbinden
public func IsWireConnectable() { return(1); }
