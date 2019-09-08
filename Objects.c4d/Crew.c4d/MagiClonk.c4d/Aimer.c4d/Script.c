/*-- Zielsteuerung für gerichtete Zauber --*/
//
// Aufruf der Zielsteuerung:
// global func CreateAimer(object pCommandObject, object pMenuObj, int iStartAngle)
//
// Abbruch:
// public func Abort()
//
// Callback aus der Zielsteuerung (an pCommandObject):
// public func OnAimerEnter(int iAngle)  - Richtung gewählt
// public func OnAimerAbort(int iAngle)  - Richtungswahl abgebrochen

#strict

local pCallbackObject;   // Befehlsziel für Callbacks
local pTargetObject;     // Besitzobjekt (dessen Besitzer die Auswahl trifft)
local pAttachObj;        // Objekt, an das die Sicht geklebt wird
local iAngle;            // Eingesetellter Winkel
local pArrow;            // Zeigepfeil

static const AIMR_StepAngle = 20; // Richtungsänderung in 20°-Schritten


/* Initialisierung */

public func Create(object pCommandObject, object pMenuObj, int iStartAngle)
  {
  // Felder kopieren
  pCallbackObject = pCommandObject;
  pAttachObj = pTargetObject = pMenuObj;
  iAngle = iStartAngle;
  // Aktivität setzen
  if (Contained(pAttachObj)) pAttachObj = Contained(pAttachObj);
  SetAction("Open", pAttachObj);
  SetCategory(C4D_MouseIgnore);
  // Eine volle Energieleiste sieht doch viel schöner aus :)
  DoEnergy(100);
  // Als Cursorobjekt auswählen, damit Tastaturkommandos gesendet werden und die Sicht stimmt
  SetCursor(GetOwner(), this(), true, true);
  SelectCrew(GetOwner(), pMenuObj, 0, 1);
  SetViewCursor(GetOwner(), pMenuObj);
  // Letztes Kommando löschen, um Probleme vorheriger Control*Single-Nachrichten zu umgehen
  ClearLastPlrCom(GetController());
  // Pfeilsymbol erstellen
  if (!pArrow) pArrow=CreateObject(GetID(), 0,0, GetOwner());
  pArrow->SetArrow(this());
  pArrow->UpdateAngle(iAngle);
  // Damit der Pfeil beim Tod des Zielenden nicht bleibt
  AddEffect("IntAimerStayAlive",pCallbackObject,1,0,0,GetID(),this);
  // Anfangswinkel übertragen
  pTargetObject->~AimingAngle(iAngle);
  UpdateAttachment();
  return(1);
  }

func FxIntAimerStayAliveStart(object pTarget, int iEffectNumber, bool fTemp, object pAimer)
{
 if(fTemp)
  return;
 EffectVar(0, pTarget, iEffectNumber)=pAimer;
}

func FxIntAimerStayAliveTimer(object pTarget, int iEffectNumber)
{
 return 1;
}

func FxIntAimerStayAliveStop(object pTarget, int iEffectNumber, int iReason, bool fTemp)
{
 if(fTemp)
  return;
 if(iReason == 3 || iReason == 4)
  if(EffectVar(0, pTarget, iEffectNumber))
   EffectVar(0, pTarget, iEffectNumber)->~Abort();
}

/* Gibt den aktuellen Winkel des Aimer/Pfeil-Paares */

public func GetCurrentAngle()
{
  if(pArrow)
    return(pArrow->GetR() );
  return(GetR() );
}
  

/* Attachment: Eingang in Gebäude verhindern: Dann halt am Gebäude attachen */

protected func RejectEntrance(object pInObj)
  {
  if (pArrow)
    {
    SetActionTargets(pAttachObj = pInObj);
    UpdateAttachment();
    }
  return(true);
  }

private func UpdateAttachment()
  {
  // Immer an der Objektmitte des Zielobjektes kleben
  if (pArrow)
    {
    var x,y;
    SetVertex(0, 0, x=GetVertex(0, 0, pAttachObj), 0, 2);
    SetVertex(0, 1, y=GetVertex(0, 1, pAttachObj), 0, 2);
    }
  }

/* Zielpfeil */

public func SetArrow(object pAimer)
  {
  // Initialisierung Pfeil
  SetVisibility(VIS_Owner() | VIS_God());
  SetCategory(C4D_Foreground | C4D_MouseIgnore);
  SetAction("Arrow", pAimer);
  SetActionData(2);
  return(true);
  }

public func UpdateAngle(int iAngle)
  {
  // Pfeil ausrichten
  return(SetR(iAngle));
  }

protected func AttachTargetLost()
  {
  // Pfeil verliert Aimer: Pfeil löschen
  return(RemoveObject());
  }


/* Zerstörung/Abbruch */

protected func Destruction()
  {
  // Pfeilobjekt: OK; nix tun
  if (!pArrow) return();
  // Pfeil entfernen
  RemoveObject(pArrow);
  // Objekt wird entfernt (irregulär): Abbrechen
  return(Abort(1));
  }

public func Close(bool fDontRemove, object oldTarget)
  {
  // Sicherheitseffekt entfernen
  if(oldTarget)
   if(GetEffect("IntAimerStayAlive", oldTarget))
    RemoveEffect("IntAimerStayAlive", oldTarget);
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
  var pTarget = pCallbackObject;
  pCallbackObject = 0;
  // Selber schließen - löscht das Objekt
  Close(fDontRemove, pTarget);
  // Callback ans Zielobjekt
  if (pTarget) pTarget->~OnAimerAbort(); else return();
  }


/* Steuerung */

protected func CrewSelection(bool fDeselect, bool fCursor)
  {
  // Bei Crewabwahl schließen (mit Verzögerung)
  if (fDeselect)
    {
    SetViewCursor(GetOwner()); // ViewCursor zurücksetzen
    SetAction("Aborting");
    }
  return(1);
  }

protected func ControlLeft()          { ClearLastPlrCom(GetController()); return(DoDirControl(DIR_Left)); }
protected func ControlRight()         { ClearLastPlrCom(GetController()); return(DoDirControl(DIR_Right)); }
protected func ControlUp()            { ClearLastPlrCom(GetController()); if(!GetPlrJumpAndRunControl(GetController())) return(DoUpDownControl(-AIMR_StepAngle)); }
protected func ControlDown()          { ClearLastPlrCom(GetController()); if(!GetPlrJumpAndRunControl(GetController())) return(DoUpDownControl(AIMR_StepAngle)); }
protected func ControlThrow()         { ClearLastPlrCom(GetController()); return(DoEnter()); }
protected func ControlDig()           { ClearLastPlrCom(GetController()); return(Abort()); }
protected func ControlSpecial()       { if (pCallbackObject) pCallbackObject->~OnAimerSpecial(); }

protected func ControlUpdate(object pByObject, int comdir)
{
  AimUpdate(this(), comdir, 1, "ControlConf");
}

protected func ControlConf(int conf)
{
  DoUpDownControl(6 * conf);
}

protected func ControlCommand(string szCommand, object pTarget, int tx, int ty)
  {
  // Bewegungskommandos als Auswahlrichtung interpretrieren - alles andere bricht den Aimer ab
  if (szCommand ne "MoveTo") return(Abort());
  // Zielbewegung in Koordinaten umwandeln
  if (pTarget) { tx=GetX(pTarget); ty=GetY(pTarget); }
  iAngle = Angle(GetX(), GetY(), tx, ty);
  if (iAngle > 180) iAngle -= 360;
  var iDir = GetDir(pTargetObject);
  if ((iAngle > 0 && !iDir) || (iAngle < 0 && iDir))
    pTargetObject->SetDir(1-iDir);
  pArrow->UpdateAngle(iAngle);
  pTargetObject->~AimingAngle(iAngle);
  DoEnter();
  return(true);
  }

private func DoDirControl(int iDir)
  {
  // Abbruch, wenn das Zielobjekt weg ist
  if (!pTargetObject) return(Abort());
  // Richtungsänderung?
  if (iDir != GetDir(pTargetObject))
    {
    iAngle=-iAngle;
    pTargetObject->SetDir(iDir);
    pArrow->UpdateAngle(iAngle);
    pTargetObject->~AimingAngle(iAngle);
    }
  else
    // Tastendruck in Zielrichtung: Zaubern!
    DoEnter();
  return(1);
  }

private func DoUpDownControl(int iByChange)
  {
  // Abbruch, wenn das Zielobjekt weg ist
  if (!pTargetObject) return(Abort());
  // Zielrichtung ändern
  iAngle = BoundBy(Abs(iAngle) + iByChange, 0, 180);
  if (!pTargetObject->GetDir()) iAngle *= -1;
  pArrow->UpdateAngle(iAngle);
  pTargetObject->~AimingAngle(iAngle);
  return(1);
  }


/* Auswahl */

public func DoEnter()
  {
  // Abbruch, wenn das Zielobjekt weg ist
  if (!pTargetObject) return(Abort());
  // Backup des Zielobjektes, weil dieses Objekt ungültig wird
  // Zugriff auf die Locals könnte gefährlich werden
  var pTarget = pCallbackObject;
  // Selber schließen - löscht das Objekt
  // Callback auf 0, damit kein AbortCall gemacht wird
  pCallbackObject = 0; Close(false, pTarget);
  // Callback ans Zielobjekt
  if (pTarget) pTarget->OnAimerEnter(iAngle); else return();
  }


// Blindaufrufe
public func OnAimerEnter(id idSpell) {}
public func OnAimerAbort()           {}
public func OnAimerSpecial()         {}

// Globaler Aufruf
global func CreateAimer(object pCommandObject, object pMenuObj, int iStartAngle)
  {
  var pAimer = CreateObject(AIMR,GetX(pMenuObj)-GetX(),GetY(pMenuObj)-GetY(),GetOwner(pMenuObj));
  if (pAimer) pAimer->Create(pCommandObject, pMenuObj, iStartAngle);
  return (pAimer);
  }
