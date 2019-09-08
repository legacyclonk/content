/*-- Turm mit Zugbrücke (links) --*/

#strict

#include CPT2 // Burgteil - Treppenhaus - Turm

/* Zugbrücke erzeugen */

private func InitializeTower()
{
	CreateObject(CPBL, -50, +35, GetOwner());
}

/* Transfer */

public func UpdateTransferZone()
{
  // Normale Transferzone
  var x = -19; var w = 34;
  // Zugbrücke geschlossen: breitere Transferzone
  if (FindDrawbridgeUp())
  	{ x = -25; w = 40; }
  // Mit darüberliegendem Niedergang
  if (FindStaircaseAbove())
    if (GetOCF() & OCF_Fullcon())
      return(SetTransferZone(x, -47, w, 61));
  // Ohne darüberliegenden Niedergang (kein Auf- oder Abstieg möglich)
  return(SetTransferZone(x, 10, w, 4));
}

/* Torsteuerung */

protected func CtxControlUp(object pClonk)
{
  // Kontextfunktion: Nur Kontrolle ermöglichen, wenn der Clonk auch tatsächlich das Tor anfasst
  // Zur Abwärtskompatibilität mit Szenarienscripten, die Tore öffnen, wird dies in eine eigene Funktion ausgelagert
  if (GetProcedure(pClonk) eq "PUSH")
    if (GetActionTarget(0, pClonk) == this())
      return (ControlUp(pClonk));
}

protected func CtxControlDownSingle(object pClonk)
{
  if (GetProcedure(pClonk) eq "PUSH")
    if (GetActionTarget(0, pClonk) == this())
      return (ControlDownSingle(pClonk));
}

protected func ControlUp()
{
  [$TxtDrawbridgeup$]
  var pBridge;
  if (pBridge = FindDrawbridge())
  	return(pBridge->ControlUp());
  return(0);
}
  
protected func ControlDownSingle()
{
  [$TxtDrawbridgedown$]
  var pBridge;
  if (pBridge = FindDrawbridge())
  	return(pBridge->ControlDown());
  return(0);
}
    
/* Forschung */

public func GetResearchBase() { return(CPT2); }
  