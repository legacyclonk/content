/*-- Abgebrannter Niedergang --*/

#strict

/* Initialisierung */

protected func Incineration()
  {
  // Zur Sicherheit: Eingang blockieren
  SetEntrance(0);
  // Burg aktualisieren (TransferZone des Treppenhauses darunter)
  UpdateAllCastleParts();
  }


/* Castle-Checks */

private func FindStaircase(int iOffsetX, int iOffsetY)
{
  // Anderes Treppenhaus in entsprechender Richtung suchen
  var pStaircase;
  while (pStaircase = FindObject(0, iOffsetX,iOffsetY,0,0, 0,0,0, NoContainer(), pStaircase))
    if (pStaircase->GetOCF() & OCF_Fullcon())
    	if (pStaircase->~IsStaircase())
        return(pStaircase);
  // Keins gefunden
  return(0);
}

private func CheckBack(bool fImmediateRemoval)
{
  // Nicht, wenn es noch fällt
  if (!GetContact(0, -1, 15)) // CNAT_All
    // Aber später nochmal checken (fertig runtergefallen)
    return(ScheduleCall(this(), "CheckBack", 37));
  // Anderes Bauteil im Hintergrund
  if (FindCastlePart(0, -10) || !FindStaircase(0, 16))
    if (fImmediateRemoval)
      return(RemoveObject());
    else
      // Ansonsten: Nächstes Frame nochmal checken und entfernen
      // Verzögert für hängende SolidMasks?
      ScheduleCall(this(), "CheckBack", 1, 1, true);
}

// Bei Aufschlag oder Schlossänderung prüfen, ob es überhaupt noch Sinn macht!
public func CastleChange() { return(CheckBack()); }
protected func Hit() { return(CheckBack()); }

private func FindCastlePart(int iOffsetX, int iOffsetY)
{
  // Anderes Bauteil in entsprechender Richtung suchen
  var pPart; 
  while (pPart = FindObject(0, iOffsetX,iOffsetY,0,0, OCF_Fullcon(),0,0, NoContainer(), pPart))
    if (!pPart->OnFire())
      if (pPart->~CastlePartWidth())
        return(pPart);
  // Keins gefunden
  return(0);
}
