/*-- Palisade --*/

#strict

/* Verwandt mit Castleparts: Vereinfachte Version */

/* Initialisierung */

protected func Initialize()
{
  // In den Hintergrund versetzen
  SetCategory(C4D_StaticBack());
  // Bei Fertigstellung an anderen Teilen ausrichten
  AdjustPalisadePosition();
  FPalisadeChange();
}

protected func Destruction()
{
  FPalisadeChange();
}

// Burgteile auf ihre Daseinsberechtigung überprüfen
public func FPalisadeChange()
  {
  // Alle Teile benachrichtigen
  var pObj;
  while(pObj = FindObject(0, 0, 0, 0, 0, OCF_Fullcon(), 0,0, NoContainer(), pObj))
    pObj->~PalisadeChange();
  return(1);
  }

/*-- Ausrichtung von Burgbauteilen --*/

public func PalisadeTop()      { return(-14); }
public func PalisadeRange()    { return(20);  } // Maximale Ausrichtungsentfernung

public func InPalisadeRange(int iDist) { return( Abs(iDist) <= PalisadeRange()); }

// Anderes Bauteil in entsprechender Richtung suchen
public func FindPalisade(int iX, int iY, object pObj)
{
  while (pObj=FindObject(0, iX,iY,0,0, OCF_Fullcon(),0,0, NoContainer(), pObj))
    if(pObj->~PalisadeRange())
      return(pObj);
}

// this(): Auszurichtendes Teil
public func AdjustPalisadePosition()
{
  var pPart;          // Jeweils gefundenes Teil
  var iWdtH, iHgtH;   // Breite/Höhe dieses Teils div 2
  var iAdjustRange;   // PalisadeRange
  // Breite, Höhe und Oberkante des auszurichtenden Teils ermitteln
  iWdtH = GetObjWidth()/2; 
  iHgtH = GetObjHeight()/2;
  // PalisadeRange ermitteln
  iAdjustRange = PalisadeRange();
  // alte Position speichern
  var iOldX = GetX(), iOldY = GetY(), i;

  // Horizontal nach Teilen links ausrichten
  if (pPart = FindPalisade( -iWdtH - iAdjustRange))
    if (InPalisadeRange( GetX() - (i = GetX(pPart) + pPart->~GetObjWidth()/2 + iWdtH) ))
      SetPosition(i, GetY());

  // Horizontal nach Teilen rechts ausrichten
  if (pPart = FindPalisade(iWdtH + iAdjustRange))
    if (InPalisadeRange( GetX() - (i = GetX(pPart) - pPart->~GetObjWidth()/2 - iWdtH) ))
      SetPosition(i, GetY());

  // Horizontal nach gleich breiten Teilen oben ausrichten
  if (pPart = FindPalisade(0, -iHgtH - iAdjustRange))
    if (pPart->~GetObjWidth()/2 == iWdtH)
      if (InPalisadeRange(GetX() - GetX(pPart)))
        SetPosition(GetX(pPart), GetY());

  // Horizontal nach gleich breiten Teilen unten ausrichten
  if (pPart = FindPalisade(0, iHgtH + iAdjustRange))
    if (pPart->~GetObjWidth()/2 == iWdtH)
      if (InPalisadeRange(GetX() - GetX(pPart)))
        SetPosition(GetX(pPart), GetY());

  // Vertikal nach Teilen oben ausrichten
  if (pPart = FindPalisade(0, -iHgtH - iAdjustRange))
    if (InPalisadeRange( GetY() - (i = GetY(pPart) + pPart->~GetObjHeight()/2 + PalisadeTop() + iHgtH) ))
      SetPosition(GetX(), i);

  // Vertikal nach gleich hohen Teilen links ausrichten
  if (pPart = FindPalisade( -iWdtH - iAdjustRange))
    if (pPart->~GetObjHeight()/2 == iHgtH)
      if (InPalisadeRange( GetY() - GetY(pPart)))
        SetPosition(GetX(), GetY(pPart));

  // Vertikal nach gleich hohen Teilen rechts ausrichten
  if (pPart = FindPalisade(iWdtH + iAdjustRange))
    if (pPart->~GetObjHeight()/2 == iHgtH)
      if (InPalisadeRange( GetY() - GetY(pPart)))
        SetPosition(GetX(), GetY(pPart));

  // Vertikal nach Teilen unten ausrichten
  if (pPart = FindPalisade(0, iHgtH + iAdjustRange))
    if (InPalisadeRange( GetY() - (i = GetY(pPart) - pPart->~GetObjHeight()/2 - pPart->~PalisadeTop() - iHgtH) ))
      SetPosition(GetX(), i);

  // Burgteile oberhalb anpassen
  // Oben links
  while (pPart = FindPalisade(-iWdtH/2, -iHgtH - iAdjustRange, pPart))
  {
    // Dieses Teil hinter das obere
    SetCategory(C4D_StaticBack());
    SetObjectOrder(pPart, 0, 1);
  }
  // Oben rechts (meist redundant)
  while (pPart = FindPalisade(iWdtH/2, -iHgtH - iAdjustRange, pPart))
  {
    // Dieses Teil hinter das obere
    SetCategory(C4D_StaticBack());
    SetObjectOrder(pPart, 0, 1);
  }
}

// kann durch das Amulett des Gelehrten erforscht werden
public func IsJungleResearch() { return(1); }

/* Kann vom Trapper hergestellt werden */
public func IsTrapperHandcraft() { return(1); }

/* Erzwingt dass der Gebäude trotz IsTrapperHandcraft mit dem Bausatz gebaut werden kann */ 
public func IsConkitBuilding() { return(1); }

// ohne Turm keine Palisade
public func GetResearchBase() { return(WGTW); }
