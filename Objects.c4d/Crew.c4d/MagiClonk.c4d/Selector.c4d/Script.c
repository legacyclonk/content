/* Auswahlpfeil für Zauber mit Objektziel */

#strict

// Globaler Aufruf zum Erzeugen des Auswahlpfeils
global func CreateSelector(object pCommandObject, object pMenuObj, int iRadius)
{
  if(!pCommandObject) return();
  if(!pMenuObj) return();
  var pSelector = CreateObject(SLCR,10,10,GetOwner(pMenuObj));
  if (pSelector) pSelector->Create(pCommandObject, pMenuObj, iRadius);
  return(pSelector);
}

// Abbruch:
// public func Abort()
//
// Callback aus der Zielsteuerung (an pCommandObject):
// public func SelectorTarget(pTarget)      - Soll prüfen, ob Objekt ein gewolltes Ziel ist (true oder false)
//
// Callback aus der Zielsteuerung (an pMenuObj bzw. pTargetObject):
// public func OnSelectorAbort()        - Beim Abbruch
// public func OnSelectorEnter(pTarget)     - Beim OK, pTarget ist das ausgewählte Zielobjekt


local pCallbackObject;   // Befehlsziel für Callbacks
local pTargetObject;     // Besitzobjekt (an das die Sicht geklebt wird, und dessen Besitzer die Auswahl trifft) - Auch Ziel für Auswahlcallback
local iRadius;           // Radius in dem nach Objekten gesucht wird
local pArrow;            // Zeigender Pfeil
local iSelect;           // Index für Local(), dessen Wert (Objektpointer) gerade markiert wird

// Local(0 bis ?) für Aktionsziele

static const SLCR_MaxTargets=8; // Maximale Anzahl anwählbarer Ziele

/* +++++++ Initialisierung +++++++++ */

public func Create(object pCommandObject, object pMenuObj, int iRange)
  {
  // Felder kopieren
  pCallbackObject = pCommandObject;
  pTargetObject = pMenuObj;
  iRadius = iRange;

  // nach Objekten suchen, keine gefunden -> abbrechen
  // Target zurücksetzen, damit kein Mana-Rücktransport-Callback gemacht werden kann!
  if(!FindTargets()) {
    pCallbackObject = pTargetObject = 0;
    Close();
    return();
  }

  // Letztes Kommando löschen, um Probleme vorheriger Control*Single-Nachrichten zu umgehen
  ClearLastPlrCom(GetController());
  // Als Cursorobjekt auswählen, damit Tastaturkommandos gesendet werden und die Sicht stimmt
  SetCursor(GetOwner(), this(), true, true);
  SelectCrew(GetOwner(), pMenuObj, 0, 1);
  SetViewCursor(GetOwner(), pMenuObj);
  // Eine gefakte Energie/Magieleiste sieht doch viel schöner aus :)
  DoEnergy(GetEnergy(pMenuObj)*100000/GetPhysical("Energy",0,pMenuObj));
  DoMagicEnergy(GetMagicEnergy(pMenuObj)*100000/GetPhysical("Magic",0,pMenuObj));

  // erstes Ziel markieren
  SetVisibility(VIS_Owner() | VIS_God());
  SetCategory(C4D_Foreground);
  SetAction("Arrow");
  iSelect=0;
  }


/* +++++++++ Zielverwaltung ++++++++ */

protected func FindTargets()                    // Nach Zielen suchen und speichern
{
  var pTarget, i, j;
  // Bedingungen: Objekt ist nicht weiter als iRadius entfernt, es wurden nicht schon MaxTargets Ziele ausgewählt,
  //              SelectorTarget in pCallbackObject (im Zauber) gibt nicht null zurück
  while(pTarget=FindObject(0,AbsX(GetX(pTargetObject)),AbsY(GetY(pTargetObject)),-1,-1,0,0,0,0,pTarget))
  {
    if(ObjectDistance(pTargetObject,pTarget) < iRadius)
    {
      if(i >= SLCR_MaxTargets) break;
      if(pCallbackObject->~SelectorTarget(pTarget))
        Local(i++) = pTarget;
    }
    else if(ObjectDistance(pTargetObject,pTarget) > iRadius*2) break;
  }
  // restlichen Objektpointer löschen
  for(j=i; j < SLCR_MaxTargets; ++j) Local(j)=0;

  return(i);
}

protected func CheckTargets()                   // Gültigkeit von Objektpointern überprüfen
{
  var pTargets = CountTargets(), i, j;
  for(i=0; i < SLCR_MaxTargets; ++i)
    if(!Local(i) && pTargets > i)
    { j=i; i=0;
      for(; j < SLCR_MaxTargets; ++j)
        Local(j) = Local(j+1);
    }
}

protected func CheckTarget()
{
  var pTarget = Local(iSelect);
  if (!pTarget) return(false); // Objekt gelöscht - kein Aufruf fürs SelectorTarget mehr!
  if(ObjectDistance(pTargetObject,pTarget) < iRadius)
    if(pCallbackObject->~SelectorTarget(pTarget))
      return(true);
  // nicht mehr gültig
  return(false);
}

protected func CountTargets()                   // Anzahl der Ziele
{
  for(var i,j; i < SLCR_MaxTargets; ++i) if(Local(i)) ++j;
  return(j);
}


/* ++++++++++++ Steuerung +++++++++++ */

protected func CrewSelection(bool fDeselect, bool fCursor)  // Abbruch bei Abwahl
  {
  // Bei Crewabwahl schließen (mit Verzögerung)
  if (fDeselect)
    {
    SetViewCursor(GetOwner()); // ViewCursor zurücksetzen
    SetAction("Aborting");
    }
  return(1);
  }

protected func ControlRight()         { ClearLastPlrCom(GetController()); FocusPlrView(); return(Forward()); }  // vor
protected func ControlLeft()          { ClearLastPlrCom(GetController()); FocusPlrView(); return(Backward()); } // zurück

protected func ControlThrow()         { ClearLastPlrCom(GetController()); return(DoEnter()); }  // ok
protected func ControlDig()           { ClearLastPlrCom(GetController()); return(Abort()); }    // abbrechen



protected func Forward()                    // Durchschalten: vor
{
  FindTargets();
  if(iSelect == CountTargets()-1) iSelect=0;
  else ++iSelect;
}

protected func Backward()                   // Durchschalten: zurück
{
  FindTargets();
  if(!iSelect) iSelect=CountTargets()-1;
  else --iSelect;
}

/* Bestätigen */

public func DoEnter()
  {
  // Abbruch, wenn das Zielobjekt weg ist
  if (!pTargetObject) return(Abort());
  // Backup des Zielobjektes, weil dieses Objekt ungültig wird
  // Zugriff auf die Locals könnte gefährlich werden
  var pTarget = pTargetObject;
  // Kleiner Fix, damit der Zauberer wieder ausgewählt wird
  SetCursor(GetOwner(), pTarget, 1);
  // Selber schließen - löscht das Objekt
  // Callback auf 0, damit kein AbortCall gemacht wird
  pTargetObject = 0; Close();
  // Callback ans Zielobjekt
  if (pTarget) pTarget->OnSelectorEnter(Local(iSelect)); else return();
  }

/* ++++++++++ Zerstörung/Abbruch ++++++++++ */

protected func Destruction()
  {
  // Objekt wird entfernt (irregulär): Abbrechen
  return(Abort(1));
  }

public func Close(bool fDontRemove)
  {
  // Sicht zurücksetzen - mit Pfeil, aber ohne Rahmen
  if (GetCursor(GetOwner()) == this() || !GetCursor(GetOwner()))
    {
    if (pTargetObject) SetCursor(GetOwner(), pTargetObject, 1);
    SetViewCursor(GetOwner()); // ViewCursor zurücksetzen
    }
  // Löschen
  if (!fDontRemove) RemoveObject();
  // Fertig
  return(1);
  }

public func Abort(bool fDontRemove)
  {
  // Backup des Zielobjektes, weil dieses Objekt ungültig wird
  // Zugriff auf die Locals könnte gefährlich werden
  var pTarget = pTargetObject;
  // Selber schließen - löscht das Objekt
  Close(fDontRemove);
  // Callback ans Zielobjekt
  pTargetObject = 0;
  if (pTarget) pTarget->~OnSelectorAbort(); else return();
  }


/* +++++++++++ Pseudo-Attach ++++++++++ */

protected func Attach()
{
  // Pfeil verliert selektiertes Objekt (Clonk weg)
  if(!CheckTarget()) if(!AttachTargetLost()) return();
  var pTarget = Local(iSelect);
  // Position anpassen
  SetPosition(GetX(pTarget),GetY(pTarget)-GetCon(pTarget)*GetDefHeight(GetID(pTarget))/2/100-10);
  FocusPlrView();
  // Energie anpassen
  DoEnergy(GetEnergy(pTargetObject)*100000/GetPhysical("Energy",0,pTargetObject)-GetEnergy());
  DoMagicEnergy(GetMagicEnergy(pTargetObject)*100000/GetPhysical("Magic",0,pTargetObject)-GetMagicEnergy());
}

private func FocusPlrView()
  {
  // Sicher stellen, dass der Spieler das Zielobjekt fokussiert
  return(SetPlrView(GetOwner(), Local(iSelect)));
  }

protected func AttachTargetLost()
{
  // neue Ziele suchen
  var iTargets = FindTargets();
  // keine gefunden: abbrechen
  if(!iTargets) return(false, Abort());
  // ansonsten neues Ziel festlegen
  if(iSelect >= iTargets) iSelect = iTargets-1;
  return(true);
}

//ClearLastPlrCom(GetController()); für control* - warum?
