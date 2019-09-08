/*--- Abflussrohr ---*/

#strict

protected func Initialize()
{
  SetAction("Connect");  
  SetVertex(0,0,GetX()); SetVertex(0,1,GetY());
  SetVertex(1,0,GetX()); SetVertex(1,1,GetY());
  SetPosition(0, 0, this());
}

private func Transfer() // Abflussrohr führt zur Zeit keine Prüfung von AcceptTransfer beim Zielobjekt durch...
{
  var i;
  // Angeschlossene Objekte ermitteln
  var pPumpSource = GetActionTarget(0);
  var pPumpTarget = GetActionTarget(1);
  // Kein Quellobjekt
  if (!pPumpSource) return(0);
  // Quellobjekt hat nicht genug Energie
  if (!EnergyCheck(1000, pPumpSource)) return(0);
  // Zielobjekt vorhanden
  if (pPumpTarget)
  {
    // Materialtransfer
    for (i = 0; i < 3; i++)  
      ObjectInsertMaterial(ObjectExtractLiquid(pPumpSource), pPumpTarget);
  }
  // Kein Zielobjekt vorhanden
  else
  {
    // Materialtransfer
    for (i = 0; i < 3; i++)    
      InsertMaterial(ObjectExtractLiquid(pPumpSource), GetVertex(50,0), GetVertex(50,1));
  }
}

public func LineBreak(bool fNoMsg)
{
  Sound("LineBreak");
  if (!fNoMsg) BreakMessage();
}
  
public func BreakMessage()
{
  // Angeschlossene Objekte ermitteln
  var pPumpSource = GetActionTarget(0);
  var pPumpTarget = GetActionTarget(1);
  // Meldung bei Leitungsbausatz am einen oder anderen Ende ausgeben
  if (GetID(pPumpSource) == LNKT) 
    Message("$TxtLinebroke$", pPumpSource);
  else
    Message("$TxtLinebroke$", pPumpTarget);
}
