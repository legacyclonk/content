/*-- Telegrafenmast (aufgestellt) --*/

#strict

// Hacken verursacht Schaden

protected func Damage()
{
  if(GetDamage()>100)
    if(GetCategory()&C4D_StaticBack())
      ChopDown();         
}

private func ChopDown()
{
  SetCategory(C4D_Vehicle());
  ChangeDef(TEL2);
  SetRDir(RandomX(-20,20));
  RemoveLines();
}

private func RemoveLines()
{
  var obj;
  while(obj = FindObject(CNNC, 0,0,0,0,0, "Connect", this(), 0)) RemoveObject(obj);
}

protected func Incineration() { RemoveLines(); }

/* Leitung */

public func WireOffset(& x, & y)
{
  y = -27;
}

public func MaxConnections()
{
  if(ObjectCount(CNNC, 0,0,0,0,0, "Connect", this()) >= 2) return(1);
  return();
}

public func IsWireConnectable() { return(1); }

/* Verbindungen */

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

/* Grenzmast */

public func BorderPole(dir)
{
  var x, pLine, pTIM;
  if(!dir) x = -18;
  if(dir == 1) x = 18;
  // Grenzleitung aufstellen
  pTIM = CreateObject(TIM1, x, -27, -1);
  pLine = CreateObject(WIRE, 0,0, -1);
  pLine->~Connect(this(), pTIM);
}
