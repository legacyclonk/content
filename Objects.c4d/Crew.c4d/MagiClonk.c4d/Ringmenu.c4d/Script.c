/*-- Das Ringmenü --*/

#strict

local pCallbackObject; // Befehlsziel für Callbacks
local pTargetObject;   // Besitzobjekt (an das die Sicht geklebt wird)
local iElementCount;   // Anzahl der Menüeinträge
local iScroll;         // Scrollposition (-99 - +99; 0=Eintrag gewählt)
local iSize;           // Radius
local iSymbolSize;     // Menüsymbolgröße
local iSelection;      // Ausgewähltes Element
// Locals:
// i*7+0: ID Des Menüeintrags i
// i*7+1: Text des Menüeintrags i
// i*7+2: Partikelfarbe des Menüeintrags i
// i*7+3: Menübefehl
// i*7+4: Parameter 1 für den Menübefehl
// i*7+5: Parameter 2 für den Menübefehl
// i*7+6: Symbolobjekt

private func & MnuItemID(i)     { return (Local(i*7)); }
private func & MnuItemText(i)   { return (Local(i*7+1)); }
private func & MnuItemPxClr(i)  { return (Local(i*7+2)); }
private func & MnuItemCom(i)    { return (Local(i*7+3)); }
private func & MnuItemPar1(i)   { return (Local(i*7+4)); }
private func & MnuItemPar2(i)   { return (Local(i*7+5)); }
private func & MnuItemSymbol(i) { return (Local(i*7+6)); }


/* Initialisierung */

public func Create(pCommandObject)
  {
  pCallbackObject = pCommandObject;
  SetAction("Grow");
  DoEnergy(100);
  }


/* Steuerung */

protected func CrewSelection(fDeselect, fCursor)
  {
  // Bei Crewabwahl schließen (mit Verzögerung)
  if (fDeselect) SetAction("Closing");
  return(1);
  }

protected func ControlLeft()
  {
  // Menüsteuerung links
  // Drehend: ggf. Umdrehen
  if (GetAction() eq "Turn" && GetDir() == DIR_Right())
    {
    SetDir(DIR_Left());
    SetPhase(TurnDuration() - GetPhase());
    iSelection = (iSelection+1) % iElementCount;
    return(Turning());
    }
  // Reguläre Drehung
  if (GetAction() eq "Open")
    {
    SetDir(DIR_Left());
    SetAction("Turn", pTargetObject);
    }
  // Fertig
  return(1);
  }

protected func ControlRight()
  {
  // Menüsteuerung rechts
  // Drehend: ggf. Umdrehen
  if (GetAction() eq "Turn" && GetDir() == DIR_Left())
    {
    SetDir(DIR_Right());
    SetPhase(TurnDuration() - GetPhase());
    if (!iSelection--) iSelection += iElementCount;
    return(Turning());
    }
  // Reguläre Drehung
  if (GetAction() eq "Open")
    {
    SetDir(DIR_Right());
    SetAction("Turn", pTargetObject);
    }
  // Fertig
  return(1);
  }

// Mausradsteuerung
protected func ControlWheelUp() { return (ControlLeft()); }
protected func ControlWheelDown() { return (ControlRight()); }

// Bestätigen: Werfen oder Hoch
protected func ControlUp() { return(DoEnter()); }
protected func ControlThrow() { return(DoEnter()); }

// Befehl, runter oder Graben: Abbruch
protected func ControlCommand() { return(Abort()); }
protected func ControlDigSingle() { return(Abort()); } // Muss als Single abgefangen werden, da der Befehl ansonstn an den Magier missgeleitet wird
protected func ControlDown() { return(Abort()); }

// Bestätigung
public func DoEnter()
  {
  // Element muss ausgewählt sein
  if (iScroll) return(Sound("Error",0,0,0,GetOwner()+1));
  // Zielobjekt noch gültig?
  if (!pCallbackObject) return(Close());
  // Menü schließen
  Close(1);
  // Befehl an Objekt senden (protected wegen CreateMenu-Analogie)
  ProtectedCall(pCallbackObject, MnuItemCom(iSelection), MnuItemID(iSelection), MnuItemPar1(iSelection), MnuItemPar2(iSelection));
  // Fertig; Objekt löschen
  return(RemoveObject());
  }

// Mausauswahl
public func MouseSelection(pSymbol)
  {
  // Symbol suchen
  var i=iElementCount;
  while (i--) if (MnuItemSymbol(i) == pSymbol) break;
  if (i<0) return();
  // Bestätigung
  iScroll=0; iSelection=i;
  return(DoEnter());
  }


/* Aktivität */

protected func Growing()
  {
  // Parameter setzen
  iSymbolSize = Min(100*GetPhase()/GrowDuration()+1, 100);
  iSize = GetDefSize() * Min(20 + 80*GetPhase()/GrowDuration(), 100) / 100;
  iScroll = (GetPhase() - GrowDuration()) * 200 / GrowDuration();
  // Symbole platzieren
  Update(1);
  }

protected func Turning()
  {
  // Parameter setzen
  iScroll = (GetPhase() * 100 / TurnDuration()) * (1-GetDir()*2);
  // Symbole platzieren
  Update();
  }

protected func TurningDone()
  {
  // Parameter setzen
  var iSelOld=iSelection;
  iSelection = (iSelection-1+GetDir()*2)%iElementCount;
  if (iSelection<0) iSelection += iElementCount;
  Update();
  }

protected func Opened()
  {
  iScroll=0;
  iSymbolSize=100; 
  iSize=GetDefSize();
  Update(1);
  }

protected func Update(fUpdCon)
  {
  // Alle Menüsymbole neu platzieren
  var iIndex = iElementCount;
  while (iIndex--)
    {
    // Symbolwinkel ermitteln
    var iAngle = ((iIndex-iSelection)*100 + iScroll)*36 / (iElementCount*10);
    // Symbolposition ermitteln
    var iX = Sin(iAngle, iSize), iY = -Cos(iAngle, iSize);
    // Symbol vorhanden?
    var pSymbol;
    if (pSymbol = MnuItemSymbol(iIndex))
      {
      // Verschieben
      // An alter Stelle einen Effekt erzeugen
      var pEffect = CreateObject(MSPK, GetX(pSymbol)-GetX(), GetY(pSymbol)-GetY(), GetOwner());
      SetClrModulation(MnuItemPxClr(iIndex), pEffect);
      SetVisibility(VIS_Owner(), pEffect);
      // Größe setzen (VOR dem Verschieben der Vertices!)
      if (fUpdCon) SetCon(iSymbolSize, pSymbol);
      // An neue Stelle schieben
      SetVertex(0, 0, -iX, pSymbol); SetVertex(0, 1, -iY, pSymbol);
      }
    else
      // Neu erzeugen
      if (MnuItemSymbol(iIndex) = pSymbol = CreateObject(MnuItemID(iIndex), iX, iY, GetOwner()))
        {
        SetVisibility(VIS_Owner(), pSymbol);
        ObjectSetAction(pSymbol, "Attach", this());
        // Größe setzen (VOR dem Verschieben der Vertices!)
        if (fUpdCon) SetCon(iSymbolSize, pSymbol);
        SetVertex(0, 0, -iX, pSymbol); SetVertex(0, 1, -iY, pSymbol);
        }
    }
  // Text des ausgewählten Elements
  if (iScroll)
    PlayerMessage(GetOwner(), "", this());
  else
    PlayerMessage(GetOwner(), Format("@%s", MnuItemText(iSelection)), this());
  }

/* Hilfsfunktionen */

private func TurnDuration() { return(10); }
private func GrowDuration() { return(10); }
private func GetDefSize() { return(Max(15 * iElementCount, 60)); }


/* Menüaufbau */

public func AddItem(idItem, szText, szCommand, par1, par2, dwParticleColor)
  {
  // Elementindex ermitteln
  var iIndex = iElementCount++;
  // Parameter setzen
  MnuItemID(iIndex) = idItem;
  MnuItemText(iIndex) = szText;
  MnuItemPxClr(iIndex) = dwParticleColor;
  MnuItemCom(iIndex) = szCommand;
  MnuItemPar1(iIndex) = par1;
  MnuItemPar2(iIndex) = par2;
  MnuItemSymbol(iIndex) = 0; // Wird dynamisch erzeugt
  // Fertig; Index zurückgeben
  return(iIndex);
  }

// Setzt den aktuellen Menüeintrag auf das Element mit der ID idSel
public func SelectID(idSel) { var i=iElementCount; while (i--) if (MnuItemID(i)==idSel) return(1, iSelection=i); }

public func Show(pMenuObj)
  {
  // Nicht wenn leer...
  if (!iElementCount) return(0, RemoveObject());
  // Felder setzen
  pTargetObject = pMenuObj;
  // Alle Symbole zeigen
  var iIndex=iElementCount, pSymbol;
  while (iIndex--) if (pSymbol = MnuItemSymbol(iIndex)) SetVisibility(VIS_Owner(), pSymbol);
  // Wachsen
  SetAction("Grow", pTargetObject);
  // Auswählen
  SetCursor(GetOwner(), this());
  }

global func CreateRingMenu(pCommandObject)
  {
  var pMenu = CreateObject(RNMU,0,0,GetOwner(pCommandObject));
  pMenu->Create(pCommandObject);
  return (pMenu);
  }


/* Menü schließen */

protected func AttachTargetLost() { return(Close()); }

public func Close(fDontRemove)
  {
  // Alle Symbole löschen
  var i=iElementCount, pSymbol;
  while (i--) if (pSymbol = MnuItemSymbol(i)) RemoveObject(pSymbol);
  // Sicht zurücksetzen
  if (GetCursor(GetOwner()) == this()) SetCursor(GetOwner(), pTargetObject);
  // Löschen
  if (!fDontRemove) RemoveObject();
  // Fertig
  return(1);
  }

public func Abort()
  {
  // Abbruch: Menü schließen
  return(Close());
  }
