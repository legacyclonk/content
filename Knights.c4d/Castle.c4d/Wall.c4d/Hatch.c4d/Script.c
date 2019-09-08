/*-- Bodenklappe --*/

#strict

/*
 * Prueft, ob die Bodenklappe an der angegeben Stelle baubar ist
 * Gibt bei Misserfolg eine Fehlermeldung, ansonsten das
 * Fundament, in das die Bodenklappe einzubauen ist.
 */

private func CheckPlacement(int x, int y)
{
  // Nur vor Burgteile
  var castle = FindCastlePart(x, y - 10);
  if(!castle) return("$ErrNoCastle$");
  
  // Nur vor Waende
  if(GetID(castle) != CPW1 && GetID(castle) != CPW2)
    return("$ErrOnlyWall$");
  
  // Nur in Fundamente
  var basement = LocalN("basement", castle);
  if(!basement) return("$ErrOnlyBasement$");
  
  // Nur, wenn noch keine Bodenklappe vorhanden ist
  var hatch_id = basement->~HatchID();
  if(!hatch_id) return("$ErrDupHatch$");
  
  return(basement);
}

/* Callback vom Bausatz */

public func RejectConstruction(int x, int y, object clonk)
{
  var err = CheckPlacement(GetX(clonk) + x, GetY(clonk) + y);
  if(GetType(err) == C4V_String() )
  {
    Message(err, clonk);
    Sound("Error", 0, clonk);
    return(1);
  }
  else
  {
    return(0);
  }
}

/* Erschaffung */

protected func Construction()
{
  // Position pruefen
  var err = CheckPlacement(AbsX(GetX()), AbsY(GetY()) );
  if(GetType(err) == C4V_String() ) return(RemoveObject() );
  
  // Sofort vollstaendig aufbauen
  SetCon(100);
  
  // In die Mitte des Fundamentes setzen
  SetPosition(GetX(err), GetY(err) - 8);
  
  // Fundament zum loch-fundament machen
  err->ChangeToHatch(this() );
  
  // Freigraben, davor aber Solidmask entfernen
  Open();
  DigFreeRect(GetX()-8, GetY(), 16, 12);
  Close();
}

protected func Initialize()
{
  SetAction("Closed");
}

/* Steueung */

protected func ControlUp(clonk)
{
  [$TxtOpen$]
  if (GetAction() S= "Closed") SetAction("Opening");
  ClearScheduleCall(this(), "DelayedClose");
}

protected func ControlDownSingle(clonk)
{
  [$TxtClose$]
  if (GetAction() S= "Open") SetAction("Closing");
  ClearScheduleCall(this(), "DelayedClose");
}

protected func ControlThrow(clonk)
{
  [$TxtOpen$]
  if (GetAction() S= "Closed") SetAction("Opening");
  ClearScheduleCall(this(), "DelayedClose");
}

protected func ControlDigSingle(clonk)
{
  [$TxtClose$]
  if (GetAction() S= "Open") SetAction("Closing");
  ClearScheduleCall(this(), "DelayedClose");
}

protected func ControlDigDouble(clonk)
{
  [$TxtCloseDelay$]
  // Etwaige andere Verzögerung abbrechen
  ClearScheduleCall(this(), "DelayedClose");  
  // Verzögert schließen
  SoundClick();
  ScheduleCall(this(), "DelayedClose", 80);
}

protected func ControlThrowDouble(clonk)
{
  [$TxtCloseDelay$]
  // Etwaige andere Verzögerung abbrechen
  ClearScheduleCall(this(), "DelayedClose");  
  // Erst öffnen
  if ((GetAction() S= "Closed") || (GetAction() S= "Closing"))
    SetAction("Opening");
  // Dann verzögert schließen
  ScheduleCall(this(), "SoundClick", 15);
  ScheduleCall(this(), "DelayedClose", 80);
}

protected func DelayedClose()
{
  if (GetAction() S= "Open") SetAction("Closing");
}

private func Close()
{
  SetSolidMask(8, 16, 16, 8, 8, 16);
}

private func Open()
{
  SetSolidMask();
}

/* Sounds */

private func SoundClose()
{
  Sound("HatchClose");
}

private func SoundOpen()
{
  Sound("HatchOpen");
}

protected func SoundClick()
{
  Sound("Click");  
}
