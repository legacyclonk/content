/*-- Leitungsbausatz --*/

#strict

protected func Hit()
{
  Sound("RockHit*");
}

protected func Activate(clonk)
{
  [$TxtConnectline$]
  // Objekt zum Line-Verlegen finden.
  var obj = FindObject(0, 1,0,0,0, OCF_LineConstruct(), 0,0,0, obj);
  // Keins da? Auch gut.
  if(!obj) return(Message("$TxtNoNewLine$", this()) );
  // Leitung
  var pLine = FindObject(0, 0,0,0,0, 0, "Connect", this() );
  SetComDir(COMD_Stop(), clonk);

  // Aktuelle Leitung anschließen
  if(pLine)
  {
    // Zielobjekt == Quellobject?
    if(obj == GetActionTarget(0, pLine) || obj == GetActionTarget(1, pLine) )
    {
      // Leitung wieder entfernen
      Message("$TxtLineRemoval$", this(), GetName(pLine) );
      RemoveObject(pLine);
      Sound("Connect");
      return(1);
    }
    else
    {
      // Line an pFrom anschließen
      if(!ConnectLine(pLine, obj) ) return(1);
    }
  }
  // Neue Leitung
  else
  {
    // LineType abfragen
    var linetype = obj->~LineConnectType();
    // Nicht vorhanden -> Standard-LineType verwenden
    if(!linetype) linetype = DefaultLineType(obj);
    // Kein möglicher Leitungstyp? :(
    if(!linetype)
    {
      Sound("Error");
      return(Message("$TxtNoNewLine$", this()) );
    }

    // Line erzeugen
    pLine = CreateLine(linetype, GetOwner(), obj, this() );
  }

  // Engineinternes Verhalten blockieren
  return(1);
}

private func DefaultLineType(obj)
{
  // LineConnect-Wert abfragen
  var line_accept = GetDefCoreVal("LineConnect", "DefCore", GetID(obj) );

  // Pumpe
  if (line_accept & C4D_LiquidPump && line_accept & C4D_LiquidInput)
  {
    var nothing = true;
    for (var pipe in FindObjects(Find_ID(SPIP), Find_ActionTarget(obj), Find_Action("Connect")))
    {
      // Nur das erste Actiontarget zaehlt.
      if (pipe->GetActionTarget(0) == obj)
        nothing = false;
    }
    if (nothing) return(SPIP);
  }
  if (line_accept & C4D_LiquidPump && line_accept & C4D_LiquidOutput)
  {
    nothing = true;
    for (pipe in FindObjects(Find_ID(DPIP), Find_ActionTarget(obj), Find_Action("Connect")))
    {
      if (pipe->GetActionTarget(0) == obj)
        nothing = false;
    }
    if (nothing) return(DPIP);
  }

  // Strom
  if(line_accept & C4D_PowerOutput)
    return(PWRL);

  // Gar nix, dann werden manche Leitungen eventuell doppelt.
  if(line_accept & C4D_LiquidPump)
    return(SPIP);
  return(0);
}

private func CreateLine(linetype, owner, from, to)
{
  var line = CreateObject(linetype, 0, 0, owner);
  ObjectSetAction(line, "Connect", from, to);
  Message("$TxtNewLine$", this(), GetName(line) );
  Sound("Connect");
  return(line);
}

/* Verbinden zweier Gebäude mit einer Leitung */
private func ConnectLine(line, to)
{
  /* Leitungstyp der Leitung und Anschlussmöglichkeit am Ziel finden */
  var line_type = GetDefCoreVal("Line", "DefCore", GetID(line) );
  var line_accept = GetDefCoreVal("LineConnect", "DefCore", GetID(to) );
  
  // Targets ausmachen. Es ist nicht definiert, welches Target das aktuell
  // verbundene Objekt und welches das Linekit ist.
  var t1 = GetActionTarget(0, line);
  var t2 = to;
  if(t1 == this() ) { t2 = GetActionTarget(1, line); t1 = to; }
  
  var ok = false;
  // Energieleitung?
  if (line_type == C4D_LinePower)
  {
    // Geht ok, wenn das Ziel Strom benötigt oder welchen hergibt
    if (t1 == to && line_accept & C4D_PowerOutput)
      ok = true;
    if (t2 == to && line_accept & C4D_PowerInput)
      ok = true;
  }
  // Zufluss?
  else if(line_type == C4D_LineSource)
  {
    // Geht ok, wenn das Ziel Flüssigkeiten benötigt oder hergibt
    if (t1 == to && line_accept & C4D_LiquidPump && line_accept & C4D_LiquidInput)
    {
      ok = true;
      // Jede Pumpe kann nur eine Zuflussleitung auf einmal haben.
      for (var pipe in FindObjects(Find_ID(SPIP), Find_ActionTarget(to), Find_Action("Connect")))
      {
        if (pipe->GetActionTarget(0) == to)
          ok = false;
      }
    }
    if (t2 == to && line_accept & C4D_LiquidOutput)
      ok = true;
  }
  // Abfluss?
  else if(line_type == C4D_LineDrain)
  {
    // Geht ok, wenn das Ziel Flüssigkeit hergibt oder benötigt
    if (t1 == to && line_accept & C4D_LiquidPump && line_accept & C4D_LiquidOutput)
      ok = true;
    if (t2 == to && line_accept & C4D_LiquidInput)
      ok = true;
  }

  // Kann die Leitung hier angeschlossen werden?
  if(!ok)
  {
    // Nö, also tschüss
    Sound("Error");
    Message("$TxtNoConnectType$", this(), GetName(line), GetName(to) );
    return(0);
  }
  
  // Verbinden
  ObjectSetAction(line, "Connect", t1, t2);

  Message("$TxtConnect$", Contained(), GetName(line), GetName(to) );
  Sound("Connect");
  RemoveObject();
  return(1);
}

/* Produkteigenschaften */

public func IsAnvilProduct() { return(1); }
