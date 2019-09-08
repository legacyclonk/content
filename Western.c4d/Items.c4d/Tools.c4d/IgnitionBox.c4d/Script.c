/*--- Fernzünder ---*/

#strict

protected func Hit ()
{
  Sound("WoodHit*");
}

protected func Activate (pCaller)
{
  // Zünddraht bereits verbunden: Sprengung
  var wire, obj;
  if (wire = FindObject(PIWI,0,0,0,0,0,"Connect",this()))
  {
    if(obj = GetActionTarget(0, wire))
    {
      SetOwner(GetOwner(pCaller), obj); // Besitzer passend setzen
      obj-> ~Ignition(pCaller);
    }
    return();
  }

  // Dynamitkiste suchen
  var box;
  if (! ( box = FindObject(DYNB,-30,-30,60,60, 0, 0,0, NoContainer()) ) )
    if (FindContents(DYNB, pCaller))
      return(Message("$AufdenBoden$", pCaller));

  // Dynamitkiste suchen
  if (! ( box = FindObject(DYNB,-30,-30,60,60, 0, 0,0, NoContainer()) ) )
    return(Message("$KeineKiste$", pCaller));

  // Neuen Draht erzeugen
  CreateObject(PIWI)-> ~Connect(box, this());
  Sound("Connect");
  Message("$Angeschlossen$", pCaller);

  return(1);
}

// Soll von nun an leichter bedienbar sein.
protected func ContextConnect(object pCaller)
{
  [$MnuConIGNB$|Image=IGNB|Condition=CanConnect]
  InventoryConnect(pCaller);
}

protected func ContextDeConnect(object pCaller)
{
  [$MnuDeConIGNB$|Image=IGNB|Condition=IsConnected]
  DeConnect(pCaller);
}

public func CanConnect()
{
  if(FindObject(PIWI,0,0,0,0,0,"Connect",this()))
    return(0);
  if(!Contained())
    return(0);
  if(FindContents(DYNB,Contained()))
    return(1);
  return(0);
}

public func IsConnected()
{
  if(FindObject(PIWI,0,0,0,0,0,"Connect",this()))
    return(1);
  return(0);
}

public func InventoryConnect(object pCaller)
{
  // Dynamitkiste suchen
  var pBox;
  if(FindContents(DYNB, pCaller))
    pBox = FindContents(DYNB, pCaller);
  else
    return(Message("$NoBoxFound$", pCaller));

  // Wenn die Leitung schon mal da ist
  if (FindObject(PIWI,0,0,0,0,0,"Connect",this()))
    return(Message("$WireAlreadyThere$", pCaller));

  // Neuen Draht erzeugen
  CreateObject(PIWI)->~Connect(pBox, this());
  Sound("Connect");
  Message("$Angeschlossen$", pCaller);

  return(1);
}

public func DeConnect(object pCaller)
{
  var pWire;
  // Die Leitung reißt
  if(pWire = FindObject(PIWI,0,0,0,0,0,"Connect",this()))
    return(pWire->~LineTooLong());
  return(Message("$NoLeitung$",pCaller));
}

// Kann in der Waffenschmiede hergestellt werden
public func IsArmoryProduct() { return(1); }
