/*-- Telegrafenstation --*/

#strict
#include BAS5
#include DOOR

local temp1, temp2;

/* Eingangssteuerung */

private func OpeningDoor() { Sound("GateOpen"); }

private func ClosingDoor() { Sound("GateClose"); }

/* Funktionen */

protected func ContextSend(object pClonk)
{
  [$SendMsg$|Image=TEL1]
  pClonk->ContainedCall("ContainedUp", this());
}

protected func ContainedUp(object pClonk)
{
  [$SendMsg$]
  CreateMenu(GetID(), pClonk, this(), 0, "$SendMsg$", 2);
  var pLine, i = 1;
  while(pLine = FindObject(CNNC, 0,0,0,0,0, "Connect", this(), 0, pLine))
    AddMenuItem(Format("$TestWire$", i++), Format("TestWire(%d, %d)", ObjectNumber(pClonk), ObjectNumber(GetActionTarget(0,pLine))), WIRE, pClonk, 0, pClonk, "$TestWireDesc$");
  if(Ready2Supply())
    AddMenuItem("$MnCallSupp$", "CallSupply", TSCS, pClonk, 0, pClonk);
}

protected func ContainedDown()
{
  [$CancelSending$]
  if(!ActIdle() && !WildcardMatch(GetAction(), "*Door*")) return(SetAction("Idle"));
}

protected func TestWire(int pClonkNo, int pLineNo)
{
  temp1 = pClonkNo;
  temp2 = pLineNo;
  // Nur wenn Energie da ist
  SetAction("TestWire");
}

protected func Testing()
{
  var pClonk = Object(temp1);
  var pLine = Object(temp2);
  temp1 = 0;
  temp2 = 0;
  // Clonk / Leitung weg
  if(!pClonk || !pLine) return();
  // Alte Funken entfernen
  var oldSpark;
  if(oldSpark = FindObjectOwner(TGTS, GetOwner(pClonk))) RemoveObject(oldSpark);
  // Funken erzeugen
  var Spark = CreateObject(TGTS, 27, -21, GetOwner(pClonk));
  // Startpunkt ermitteln
  var Start = GetActionTarget(0, pLine);
  if(GetActionTarget(1, GetActionTarget(1, pLine)) == this()) Start = GetActionTarget(1, pLine);
  // Losschicken
  Spark->Set(pLine, Start);
}

protected func CallSupply(id dummy, object pClonk)
{
  temp1 = pClonk;
  // Nur wenn Energie da ist
  SetAction("CallSupply");
}

protected func Supply()
{
  if(!temp1) return();
  var pClonk = temp1;
  temp1 = 0;
  // Hat der Spieler genug Geld?
  if(GetWealth(GetOwner(pClonk)) < 20)
    {
    PlayerMessage(GetOwner(pClonk), "$MsgNoMoney$", pClonk);
    Sound("Error", 1, 0,0, GetOwner(pClonk));
    return();
    }
  // Geld abziehen, bestellen und Ger‰usch
  DoWealth(GetOwner(pClonk), -20);
  HomebaseProduce(GetOwner(pClonk));
  return(Sound("Uncash", 1, 0,0, GetOwner(pClonk)));
}

// Bereit zum Nachschubliefern
public func Ready2Supply()
{
  // Regel aktiviert
  var sRule;
  if(!(sRule=FindObject(TSCS))) return();
  // Bin ich mit einem Randpfosten verbunden?
  return(IsConnectedTo(LocalN("pPole1", sRule)) || IsConnectedTo(LocalN("pPole2", sRule)));
}

/* Leitungssteuerung */

public func MaxConnections()
{
  if(ObjectCount(CNNC, 0,0,0,0,0, "Connect", this()) >= 2) return(1);
  return();
}

public func WireOffset(& x, & y)
{
  x = 27;
  y = -21;
}

/* Status */

// Verbunden mit...
public func IsConnectedTo(object pTarget, object pIgnore)
{
  // Seltsam aber: bin ich das Ziel?
  if(pTarget == this()) return(1);
  // Alle angeschlossenen Leitungen durchgehen
  var pConnect, pEnd;
  while(pConnect = FindObject(CNNC, 0,0,0,0,0, "Connect", this(), 0, pConnect))
    {
    // Endpunkt suchen
    pEnd = GetWireEnd(pConnect, pTarget);
    // Endpunkt = Ziel?
    if(pEnd == pTarget) return(1);
    // Endpunkt = ingoriertes Ziel?
    if(pEnd == pIgnore) continue;
    // Endpunkt sagen, dass er weiter schauen soll und schauen, ob er erfolgreich war
    if(pEnd->~IsConnectedTo(pTarget, this())) return(1);
    }
  // Ich bin scheinbar nicht verbunden
  return();
}

private func GetWireEnd(object pConnect, object pTarget)
{
  var pEnd, pLine, pLineEnd;
  // Ein Ende muss es geben
  while(!pEnd)
    {
    // Leitung speichern
    pLine = GetActionTarget(0, pConnect);
    // Anderes Ende suchen
    pLineEnd = GetActionTarget(0, pLine);
    if(pLineEnd == pConnect) pLineEnd = GetActionTarget(1, pLine);
    // Objekt dort anschauen
    pLineEnd = GetActionTarget(1, pLineEnd);
    // Objekt gefunden
    pEnd = pLineEnd;
    }
  return(pEnd);
}

// Telegrafenleitungen sind hier anschlieﬂbar
public func IsWireConnectable() { return(1); }
