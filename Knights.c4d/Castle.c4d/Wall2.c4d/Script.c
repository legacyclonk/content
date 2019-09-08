/*-- Wand --*/

// Stellt die Grundfunktionalität für alle Burgbauteile bereit

#strict

local basement;

/* Initialisierung */

protected func Initialize()
{
  // In den Hintergrund versetzen
  SetCategory(C4D_StaticBack());
  // Bei Fertigstellung an anderen Teilen ausrichten
  CastlePartAdjust();
  // Objekte im Umkreis ihre Überprüfungen durchführen lassen
  CastlePartInitialize();
  // Fertig
  return(1);
}

protected func Construction()
{
  // Erstellen des Fundaments
  CastlePartConstruction();
  return(1);
}

protected func Destruction()
{
  // Benachrichtigung benachbarter Teile + Entfernen des Fundaments
  CastlePartDestruction();
  return(1);
}

protected func Incineration()
{
  // Benachrichtigung benachbarter Teile
  CastlePartIncinerate();
  return(1);
}

/* Ausrichtung von Burgbauteilen */

// Burgteildaten
public func CastlePartWidth()    { return (80);  }
public func CastlePartHeight()   { return (74);  }
public func CastlePartTop()      { return (-13); }
public func CastlePartBasement() { return(BSC3); }

protected func CastlePartRange() { return(15);   } // Maximale Ausrichtungsentfernung

public func InCastlePartRange(int iDist)
{ 
	return(Abs(iDist) <= CastlePartRange()); 
}

// Anderes Bauteil in entsprechender Richtung suchen
public func FindCastlePart(int iX, int iY, object pObj)
{
  while (pObj = FindObject(0, iX,iY,0,0, OCF_Fullcon(),0,0, NoContainer(), pObj))
    if (pObj->~CastlePartWidth())
      return(pObj);
  // Keins gefunden
  return(0);
}

// Wird von allem Burgenbauteilen aufgerufen.
// this(): Auszurichtendes Teil
public func CastlePartAdjust()
{
  var pPart;          // Jeweils gefundenes Teil
  var iWdtH, iHgtH;   // Breite/Höhe dieses Teils div 2
  var iAdjustRange;   // CastlePartRange
  // Breite, Höhe und Oberkante des auszurichtenden Teils ermitteln
  iWdtH = CastlePartWidth()/2; 
  iHgtH = CastlePartHeight()/2;
  // CastlePartRange ermitteln
  iAdjustRange = CastlePartRange();
  // alte Position speichern
  var iOldX = GetX(), iOldY = GetY(), i;

  // Horizontal nach gleich breiten Teilen oben ausrichten
  if (pPart = FindCastlePart(0, -iHgtH - iAdjustRange))
    if (pPart->~CastlePartWidth()/2 == iWdtH)
      if (InCastlePartRange(GetX() - GetX(pPart)))
        SetPosition(GetX(pPart), GetY());

  // Horizontal nach gleich breiten Teilen unten ausrichten
  if (pPart = FindCastlePart(0, iHgtH + iAdjustRange))
    if (pPart->~CastlePartWidth()/2 == iWdtH)
      if (InCastlePartRange(GetX() - GetX(pPart)))
        SetPosition(GetX(pPart), GetY());

  // Horizontal nach Teilen links ausrichten
  if (pPart = FindCastlePart( -iWdtH - iAdjustRange))
    if (InCastlePartRange( GetX() - (i = GetX(pPart) + pPart->~CastlePartWidth()/2 + iWdtH) ))
      SetPosition(i, GetY());

  // Horizontal nach Teilen rechts ausrichten
  if (pPart = FindCastlePart(iWdtH + iAdjustRange))
    if (InCastlePartRange( GetX() - (i = GetX(pPart) - pPart->~CastlePartWidth()/2 - iWdtH) ))
      SetPosition(i + pPart->~CastlePartOffsetX(), GetY());

  // Vertikal nach Teilen oben ausrichten
  if (pPart = FindCastlePart(0, -iHgtH - iAdjustRange))
    if (InCastlePartRange( GetY() - (i = GetY(pPart) + pPart->~CastlePartHeight()/2 + CastlePartTop() + iHgtH) ))
      SetPosition(GetX(), i);

  // Vertikal nach gleich hohen Teilen links ausrichten
  if (pPart = FindCastlePart( -iWdtH - iAdjustRange))
    if (pPart->~CastlePartHeight()/2 == iHgtH)
      if (InCastlePartRange( GetY() - GetY(pPart)))
        SetPosition(GetX(), GetY(pPart));

  // Vertikal nach gleich hohen Teilen rechts ausrichten
  if (pPart = FindCastlePart(iWdtH + iAdjustRange))
    if (pPart->~CastlePartHeight()/2 == iHgtH)
      if (InCastlePartRange( GetY() - GetY(pPart)))
        SetPosition(GetX(), GetY(pPart));

  // Vertikal nach Teilen unten ausrichten
  if (pPart = FindCastlePart(0, iHgtH + iAdjustRange))
    if (InCastlePartRange( GetY() - (i = GetY(pPart) - pPart->~CastlePartHeight()/2 - pPart->~CastlePartTop() - iHgtH) ))
      SetPosition(GetX(), i);

  // Burgteile oberhalb anpassen
  // Oben links
  while (pPart = FindCastlePart(-iWdtH/2, -iHgtH - iAdjustRange, pPart))
  {
    // Fundament gegebenenfalls entfernen
    pPart->~RecheckBasement();
    // Dieses Teil hinter das obere
    SetCategory(C4D_StaticBack());
    SetObjectOrder(pPart, 0, 1);
  }
  // Oben rechts (meist redundant)
  while (pPart = FindCastlePart(iWdtH/2, -iHgtH - iAdjustRange, pPart))
  {
    // Fundament gegebenenfalls entfernen
    pPart->~RecheckBasement();
    // Dieses Teil hinter das obere
    SetCategory(C4D_StaticBack());
    SetObjectOrder(pPart, 0, 1);
  }

  // Objekt mit Fundament wurde verschoben?
  if (basement) 
  	if((iOldX -= GetX()) || (iOldY -= GetY()))
	    // Dann das Fundament und daraufliegende Objekte nachschieben
	    MoveBasement(basement, -iOldX, -iOldY);

  // Fertig
  return(1);
}

private func MoveBasement(object pBasement, int iByX, int iByY)
{
  // Fundament verschieben
  SetPosition(GetX(pBasement) + iByX, GetY(pBasement) + iByY, pBasement);
  // Verschiebung nicht nach oben? Keine (ernstzunehmende) Gefahr
  if (iByY >= 0) return(1);
  // Ansonsten: Objekte aus dem Fundament holen
  var iWdt = CastlePartWidth();
  var iX = GetX(pBasement) - GetX() - iWdt/2;
  var iY = GetY(pBasement) - GetY() - 25;
  var pObj;
  while (pObj = FindObject(0, iX, iY, iWdt, 32, 0, 0, 0, NoContainer(), pObj))
    if (!(GetCategory(pObj) | C4D_StaticBack() | C4D_Structure()))
      if(pObj != pBasement)
        SetPosition(GetX(pObj) + iByX, GetY(pObj) + iByY, pObj);
  // Fertig
  return(1);
}

public func RecheckBasement()
{
  // Fundament vorhanden?
  if (!basement) return(1);
  // Prüfen, ob links und rechts ein Burgteil unterhalb existiert
  var iWdt4 = CastlePartWidth()/4;
  var iY = CastlePartHeight()/2 + CastlePartRange();
  // Unterhalb rechts prüfen
  if (FindCastlePart(iWdt4, iY))
    // Unterhalb links prüfen
    if (FindCastlePart(-iWdt4, iY))
      // OK; das Fundament ist unnötig
      RemoveObject(basement);
  // Fertig
  return(1);
}

/* Überprüfungen bei Veränderungen in der Burg */

// Burgteile auf ihre Daseinsberechtigung überprüfen (Brustwehren inmitten der Burg, etc)
public func FCastleChange()
{
  // Alle Teile benachrichtigen
  var pObj;
  while (pObj = FindObject(0, 0, 0, 0, 0, OCF_Fullcon(), 0,0, NoContainer(), pObj))
    pObj->~CastleChange();
  return(1);
}


public func CastlePartInitialize() 
{ 
	return(FCastleChange()); 
}

public func CastlePartConstruction()
{
  // Fundament erstellen?
  var idBasement = CastlePartBasement();
  if (!idBasement) return(1);
  // nötig?
  if(FindCastlePart(0, CastlePartRange())) return(1);
  // OK, Fundament erstellen und speichern
  basement = CreateObject(idBasement, 0, 8, GetOwner());
  // Da das Burgteil statisch ist, sollte sich auch das Fundament nicht bewegen
  basement->Consolidate();
  return(1);
}

// Der Aufruf von CastleChange muss verzögert erfolgen, damit das Teil zum Aufruf auch wirklich weg ist
public func CastlePartDestruction()
{
  // Fundament?
  if (basement) 
  	RemoveObject(basement);
  // Globaler Temporäreffekt, wenn nicht schon vorhanden
  if (!GetEffect("IntCPW2CastleChange"))
  	AddEffect("IntCPW2CastleChange", 0, 1, 2, 0, CPW2);
  return(1);
}

protected func FxIntCPW2CastleChangeStop()
{
  // Alle BurgTeile benachrichtigen
  var pObj;
  while (pObj = FindObject(0, 0, 0, 0, 0, OCF_Fullcon(), 0,0, NoContainer(), pObj))
    pObj->~CastleChange();
  // Fertig
  return(1);
}

// Wird von verbrannten Burgenbauteilen bei Incineration aufgerufen.
// Muss seltsamerweise ebenso verzögert aufgerufen werden
public func CastlePartIncinerate(object pPart)
{
  return(UpdateAllCastleParts());
}

global func UpdateAllCastleParts()
  {
  // Globaler Temporäreffekt, wenn nicht schon vorhanden
  if (!GetEffect("IntCPW2CastleChange")) 
  	AddEffect("IntCPW2CastleChange", 0, 1, 2, 0, CPW2);
  return(1);
  }
  
// Sucht ein Burgteil an angegebener Position
global func FindCastlePart(int x, int y)
{
  // Verhindern, dass x und y 0 sind, sonst wuerde die ganze Karte abgesucht
  if (x == 0) x = 1;
  if (y == 0) y = 1;
  
  var obj;
  while (obj = FindObject(0, x, y, 0,0, 0,0,0, NoContainer(), obj))
    if (obj->~CastlePartWidth())
      return(obj);
  return(0);
}
