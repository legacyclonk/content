/*-- Pumpe --*/

#strict

#appendto PUMP

protected func Activate(iPlr)
{
  if(GetID()!=NOPC) return(_inherited(iPlr));
  MessageWindow(GetDesc(), iPlr);
  return(1);
}

private func ReadyToPump()
{
  // Zuflussrohr suchen
  var pSourcePipe = FindObject(SPIP,0,0,0,0,0,"Connect",this());
  // Kein Zuflussrohr da: nicht pumpen
  if (!pSourcePipe) return(0);
  // Zufluss-Leitungsbausatz suchen
  var pPumpSource = GetActionTarget(1, pSourcePipe);
  // Zuflussrohr ist gar nicht oder nicht an einen Leitungsbausatz angeschlossen: strange?!
  if (!pPumpSource || (GetID(pPumpSource)!=LNKT)) return(1);
  // Abflussrohr suchen
  var pDrainPipe = FindObject(DPIP,0,0,0,0,0,"Connect",this());
  // Kein Abflussrohr da: pumpen
  if (!pDrainPipe) return(1);
  // Abfluss-Zielobjekt suchen
  var pPumpTarget = GetActionTarget(1, pDrainPipe);
  // Abflussrohr ist gar nicht oder an einen Leitungsbausatz angeschlossen: pumpen
  if(FindObject(NOPC)) if(pPumpTarget && (GetID(pPumpTarget)==LNKT)) if(GBackSolid(GetX(pPumpTarget)-GetX(), GetY(pPumpTarget)-GetY())) return(0);
  if (!pPumpTarget || (GetID(pPumpTarget)==LNKT)) return(1);
  // Kapazität für gepumptes Material im Zielobjekt prüfen
  if (!CheckCapacity(pPumpTarget, 12, pPumpSource->GetMaterial(0,1)))
    // Keine ausreichende Kapazität: nicht pumpen
    return(0);
  // Ansonsten: pumpen
  return(1);
}

private func CheckCapacity(pTarget, iAmount, iMaterial) { return(_inherited(pTarget, iAmount, iMaterial)); }
