/*-- Dialoge --*/

#strict 2

// Eigene Dialoge werden realisiert, indem man mit #appendto DG69 an dieses Script anhängt
// Achtung: Dadurch liegen die Funktionen aller Dialogscripte im gleichen Scope - auf eindeutige Namen ist zu achten!
// Lokale Variablen des Dialogobjektes haben ein dlg_-Prefix

// Öffentlich
local dlg_name;         // Dialog-ID
local dlg_plr;          // Ansprechender Spieler
local dlg_npc;          // Angesprochene Person - this für allgemeine Action-Objekte
local dlg_targets;      // Steuert, welche Spieler die Dialognachtichten sehen (DG69_Targets_-Konstante)
local dlg_npc_portrait; // Standard-Portraitoverride
local dlg_extra;        // Variable zum dialogeigenen Gebrauch

// Intern
local dlg_passivelisteners;
local dlg_waittime;     // Restzeit bis zum nächsten Dialog-Callback
local dlg_nextcall;     // Nächster Dialogaufruf
local dlg_activedlg;    // IntDialog-Effektnummer des aktiven Dialogeffektes (0: Kein Dialog)
local dlg_inputcall;    // Nächster Aufruf für InputCallback
local dlg_isforced;     // Wenn gesetzt, wird der Dialog nicht abgebrochen, wenn sich der Spieler bewegt

// Gegenwärtige Nachrichten, die ggf. an umliegende Clonks weitergeleitet werden
// Aufbau: [ Message, Option1, Option2, ... ]
local dlg_currmsgs;
local dlg_currmsg_speaker, dlg_currmsg_portrait;
local dlg_curroption;

static const DG69_DlgEffectWaitTime = 5,
              DG69_SpeakRadius = 250, // Px nach d2, in dessen Radius andere Clonks Nachrichten mitkriegen
              DG69_Wait_Menu = -1; // Konstante für DlgWait: Warten, bis Menü zu
              
static const DG69_Targets_Nearby  = 0,
              DG69_Targets_All     = 1,
              DG69_Targets_Private = 2;


/* Globale Funktionen zum Erzeugen/Entfernen der Dialoge */

global func SetDialog(string dialog_name, object target)
  {
  // Defaultobjekt this
  if (!target) if (!(target = this)) return 0;
  // Ggf. alten Dialog entfernen
  RemoveDialog(target);
  // Dialog erzeugen und ankleben
  var dialog = CreateObject(DG69, 5,5, -1);
  if (!dialog) return;
  if (!dialog->DlgActivate(dialog_name, target))
    { dialog->RemoveObject(); dialog=0; }
  // Der Clonk ist häufig im anderen Layer, um nicht getroffen zu werden
  // Das Dialogobjekt sollte aber dennoch immer im Hauptlayer sein
  if (dialog) dialog->SetObjectLayer(0);
  // Dialog zurückgeben
  return dialog;
  }

global func RemoveDialog(object target)
  {
  // Kein Ziel angegeben: Lokales Object
  if (!target) target=this;
  // DG69-Objekt zum Zielobjekt umwandeln; ansonsten suchen
  if (GetID(target) != DG69) target = FindObject2(Find_ID(DG69), Find_ActionTarget(target));
  if (!target) return false;
  target->RemoveObject();
  return true;
  }



/* Globale Funktion zum Starten von Dialogen */

global func StartDialog(string dialog, object player, object npc, object cmd_target)
  {
  // NPC tot?
  if (npc && !GetAlive(npc))
    {
    // Fehlernachricht
    Message("$MsgDead$", player);
    Sound("CommandFailure*", 0, player, 100, GetController(player));
    }
  // Dialogobjekt ggf. temporär erzeugen
  var dlg_target = cmd_target;
  if (!dlg_target && GetID()==DG69) dlg_target=this;
  if (!dlg_target)
    {
    dlg_target = CreateObject(DG69, 5,5, -1);
    dlg_target->ActivateTemp();
    }
  // Dialogeffekt an den Spieler
  // Timer für den Check, ob der Spieler noch spricht
  // Extra-Par1: Dialogname
  // Extra-Par2: NPC
  AddEffect("IntDialog", player, 1, DG69_DlgEffectWaitTime, dlg_target, 0, dialog, npc);
  }



/* -- Globale Funktion zum Starten von Aktionen */

global func StartAction(string action, object player_clonk)
  {
  return StartDialog(action, player_clonk);
  }


/* Initialisierung */

public func DlgActivate(string dialog_name, object target)
  {
  if (!target) return false;
  // Dialog sichern
  dlg_name = dialog_name;
  // Objekt ankleben
  if (!SetAction("Attach", target)) return false;
  // Aktualisierung an Target
  UpdateTarget(target);
  // Fertig
  return(true);
  }

private func UpdateTarget(object target)
  {
  // Größe
  var wdt = GetDefWidth(GetID(target));
  var hgt = GetDefHeight(GetID(target));
  SetShape(wdt/-2, hgt/-2, wdt, hgt);
  // Name für Mausklick
  SetName(Format("$TxtName$", GetName(target)));
  return true;
  }

public func ActivateTemp()
  {
  // Temporär: Nur für einen Dialog aktivieren, und danach löschen
  SetCategory(C4D_MouseIgnore|C4D_StaticBack);
  return(SetAction("Temp"));
  }


/* Zur Sicherheit: Bei Spielstands-Laden das Shape aktualisieren */

protected func UpdateTransferZone()
  {
  UpdateTarget(GetActionTarget());
  }


/* Entfernen, wenn das Dialogziel weg ist */

protected func AttachTargetLost() { return(RemoveObject()); }


/* Ansprechen über Linksklick */

protected func MouseSelection(int by_player)
  {
  // Ignorieren für Nicht-Dialoge
  if (GetAction() != "Attach") return;
  // Ziel verloren?
  if (!GetActionTarget()) { RemoveObject(); return; }
  // Spieler muss seinen Clonk lenken
  var clonk = GetCursor(by_player);
  if (!clonk) return;
  if (clonk->~IsNotReady()) return;
  // Clonk zum Ziel laufen lassen und ansprechen
  SetCommand(clonk, "Call", this, 0,0, 0, "ActivateEntrance");
  AddCommand(clonk, "MoveTo", this);
  }


/* Ansprechen über 'springen' (Oder Callback aus den Commands) */

protected func ActivateEntrance(object speaker)
  {
  // Ignorieren für Nicht-Dialoge
  if (GetAction() != "Attach") return;
  // Ziel verloren?
  if (!GetActionTarget()) { RemoveObject(); return; }
  // Eingangsbefehl stoppen
  SetCommand(speaker, "None");
  // Nah genug dran?
  if (ObjectDistance(speaker) >= 20) return;
  // Kein Dialog?
  if (!GetChar(dlg_name)) return;
  // Nur Spieler
  if (!GetAlive(speaker)) return;
  if (~GetOCF(speaker) & OCF_CrewMember) return;
  if (speaker->~IsNotReady()) return;
  // Nicht, wenn aktiv (NPC redet schon)
  if (IsActive()) return;
  // Spieler redet schon mit jemand anderem (aktiv oder passiv): Den anderen Dialog abbrechen
  // Wenn der Clonk im Dialog "gefesselt" ist, sollte er ~IsNotReady sein
  var speak_eff;
  if (speak_eff = GetEffect("IntDialog*", speaker))
    RemoveEffect(0, speaker, speak_eff);
  // Dialog starten!
  return StartDialog(dlg_name, speaker, GetActionTarget(), this);
  }

protected func FxIntDialogStart(object target, int fx_num, int temp, string dialog, object npc)
  {
  // Nur reguläre Aufrufe
  if (temp) return FX_OK;
  // Spieler und Dialogindex lokal sichern
  dlg_activedlg = fx_num;
  dlg_plr = target;
  dlg_name = dialog;
  dlg_npc = npc;
  dlg_inputcall = 0;
  if (!dlg_npc) dlg_npc = this; // Ziellose Action
  // Passiv-Dialogeffekte an umliegende Clonks
  dlg_passivelisteners = FindDlgMsgTargets();
  for (var surr_clonk in dlg_passivelisteners)
    if (surr_clonk) AddEffect("IntDialogPassive", surr_clonk, 1, DG69_DlgEffectWaitTime, this);
  // Spieler vorbereiten
  StopClonkEx(dlg_plr);
  // Dialog starten
  ObjectCall(this, Format("Dlg%sStart", dlg_name));
  // OK; Erfolg
  return FX_OK;
  }

protected func FxIntDialogStop(object target, int fx_num, int reason, bool temp)
  {
  // Nur reguläre Aufrufe
  if (temp) return FX_OK;
  // Dialog-Callback
  if (GetChar(dlg_name)) ObjectCall(this, Format("Dlg%sStop", dlg_name));
  // Dialog frei
  AbortMessageBoard(this, GetController(dlg_plr));
  dlg_plr = 0; dlg_npc = 0;
  dlg_activedlg = 0;
  dlg_currmsgs = 0;
  dlg_currmsg_speaker = 0;
  dlg_currmsg_portrait = 0;
  dlg_curroption = 0;
  // Zuhörereffekte stoppen
  var fx_listener_target;
  if (dlg_passivelisteners) for (var surr_clonk in dlg_passivelisteners) if(surr_clonk) 
    if (fx_listener_target = GetEffect("IntDialogPassive", surr_clonk, 0, 4))
      if (fx_listener_target == this)
        RemoveEffect("IntDialogPassive", surr_clonk);
  dlg_passivelisteners = 0;
  // Temporäres Dialogobjekt? Dann entfernen
  if (IsTempDlg())
    {
    if (reason!=FX_Call_RemoveClear) RemoveObject(); /* else wird schon entfernt */
    }  
  return FX_OK;
  }
  
private func IsForcedDialog()
  {
  // Forcierter Dialog?
  if (dlg_isforced) return true;
  // Auch bei deaktiviertem Spieler oder Aktion
  if (dlg_npc == this) return true;
  if (!GetCrewEnabled(dlg_plr)) return true;
  return false;
  }

protected func FxIntDialogTimer(object target, int fx_num, int time)
  {
  // "normaler" Dialog?
  if (!IsForcedDialog())
    {
    // Prüfen, ob der NPC noch lebt und da ist
    if (!dlg_npc || !GetAlive(dlg_npc)) { StopDialog(); return FX_OK; }
    // Prüfen, ob der Clonk inzwischen weggelaufen ist
    if (ObjectDistance(target) > 50)
      {
      if (!ObjectCall(this, Format("Dlg%sRunaway", dlg_name)));
        // Kein Runaway-Aufruf: Standard nehmen
        DlgRunaway();
      StopDialog();
      return FX_OK;
      }
    }
  // Dialog wartet auf Schließen des Menüs?
  if (dlg_waittime == DG69_Wait_Menu)
    {
    if (!IsPlrInMenu()) dlg_waittime = 0;
    }
  else
    // Dialog wartet auf Timer
    dlg_waittime = Max(dlg_waittime - DG69_DlgEffectWaitTime);
  var callfn;
  if (!dlg_waittime)
    {
    if (GetChar(dlg_nextcall))
      {
      callfn = dlg_nextcall; dlg_nextcall=0;
      Call(Format("Dlg%s", callfn));
      }
    else
      StopDialog();
    }
  return FX_OK;
  }

private func IsPlrInMenu()
  {
  var cursor = GetCursor(GetController(dlg_plr));
  if (!cursor) if (!(cursor = dlg_plr)) return false;
  // Enginemenü oder Dialog
  if (GetMenu(cursor) == DG69) return true;
  // Ringmenü
  if (cursor->~IsMenu()) return true;
  // Input-Dlg
  if (TestMessageBoard(GetController(dlg_plr), true)) return true;
  // Kein Menü
  return false;
  }

private func StopDialog(bool ding)
  {
  // Dialog abbrechen: Effekt entfernen
  if (!dlg_plr) return false;
  if (ding) Sound("Ding", false, dlg_plr, 100, dlg_plr->GetController());
  return RemoveEffect(0, dlg_plr, dlg_activedlg);
  }

private func Find_PlayerType(int plrtype) {
	var p = [C4FO_Or];
	for(var i = -1; i < GetPlayerCount(); i++)
		if(GetPlayerType(GetPlayerByIndex(i)) == plrtype)
			p[GetLength(p)] = Find_Owner(GetPlayerByIndex(i));
	return(p);
}

private func FindDlgMsgTargets()
  {
  // Keine Ziele
  if (dlg_targets == DG69_Targets_Private) return [];
  // Alle Spielerclonks
  if (dlg_targets == DG69_Targets_All)
    {
    return FindObjects(Find_PlayerType(C4PT_User), Find_Func("DG69_IsMsgTarget"), Find_Exclude(dlg_plr));
    }
  else
    {
    // Nur naheliegende
    return FindObjects(Find_PlayerType(C4PT_User), Find_Func("DG69_IsMsgTarget"), Find_Exclude(dlg_plr), Find_Distance(DG69_SpeakRadius));
    }
  }

global func DG69_IsMsgTarget()
  {
  // Muss Cursor sein
  if (GetCursor(GetController()) != this) return false;
  // Darf sonst nirgends zuhören/sprechen
  if (GetEffect("IntDialog*", this)) return false;
  // OK
  return true;
  }


/* Status */

public func IsActive() { return !!dlg_activedlg; }
public func IsTempDlg() { return (GetAction() == "Temp"); }


/* Standard-Callbacks */

private func DlgRunaway()
  {
  Message("$MsgRunaway$", dlg_npc);
  }


/* Hilfsfunktionen */

private func DlgWait(string next_func, int wait)
  {
  if (!wait && IsPlrInMenu()) wait = DG69_Wait_Menu;
  else if (!wait) wait = 5;
  dlg_waittime = wait;
  dlg_nextcall = next_func;
  }
  
private func DlgWaitStop(bool ding, int wait)
  {
  if (ding)
    return DlgWait("StopDialogDing", wait);
  else
    return DlgWait("StopDialog", wait);
  }
  
public func DlgStopDialogDing() { return StopDialog(true); }
public func DlgStopDialog() { return StopDialog(); }

private func DlgMsg(string message, object speaker, string portrait)
  {
  // Portrait-Fallback
  if (!portrait && speaker==dlg_npc) portrait = dlg_npc_portrait;
  // Daten sichern
  dlg_curroption = 0;
  if (message)
    {
    dlg_currmsgs = [message];
    dlg_currmsg_speaker = speaker;
    dlg_currmsg_portrait = portrait;
    }
  else
    {
    dlg_currmsgs = 0;
    dlg_currmsg_speaker = 0;
    dlg_currmsg_portrait = 0;
    return true;
    }
  // Nachricht zum Spieler
  return MsgBox(dlg_plr, message, speaker, portrait);
  }

private func SetMsgPortrait(string new_portrait) { dlg_npc_portrait = new_portrait; }

private func DlgAddOption(icon, string option, string command, param, int extra, xPar)
  {
  // Icon auflösen
  if (!icon)
    // Standardicon
    icon = ROCK;
  else if (GetType(icon) == C4V_C4Object)
    {
    // Icon nach Objekt in Objektfarbe
    extra |= C4MN_Add_ImgObject;
    icon = GetID(xPar = icon);
    }
  // Ansonsten bleibt das angegebene Icon
  // Option sichern für Zuhörer
  dlg_currmsgs[GetLength(dlg_currmsgs)] = option;
  // Optionen an den echten Dialogspieler
  return MsgBoxAddOption(dlg_plr, icon, option, command, param, extra, xPar);
  }
  
private func DlgAddText(string text)
  {
  // Sichern für Zuhörer
  dlg_currmsgs[GetLength(dlg_currmsgs)] = text;
  // Nachricht zum Spieler
  return (MsgBoxAddText(dlg_plr, text)); 
  }
  
private func DlgQueryInput(string text, bool uppercase, string call_fn)
  {
  // Spieler soll anhalten, damit er nicht in sein Verderben läuft
  StopClonkEx(dlg_plr);
  // MsgBoard-Query, um einen String abzufragen
  dlg_inputcall = call_fn;
  CallMessageBoard(this, uppercase, text, GetController(dlg_plr));
  return true;
  }
  
protected func InputCallback(string answer, int for_plr)
  {
  // MsgBoard-Query-Antwort ist eingetroffen!
  var inputcall = dlg_inputcall;
  if (inputcall)
    {
    dlg_inputcall = 0;
    Call(inputcall, answer, for_plr);
    }
  return true;
  }

protected func OnMenuSelection(int idx, object obj)
  {
  // Menüauswahl sichern für Zuhörer
  dlg_curroption = idx;
  }

private func DlgSetForced(bool to_val) { dlg_isforced = to_val; return true; }


/* Passiv-Zuhören bei den Dialogen */

protected func FxIntDialogPassiveStart(object target, int fx_num, int temp)
  {
  // Nur reguläre Aufrufe
  if (temp) return FX_OK;
  // Text aktualisieren
  DlgUpdatePassiveText(target);
  return FX_OK;
  }

protected func FxIntDialogPassiveStop(object target, int fx_num, int reason, bool temp)
  {
  // Nur reguläre Aufrufe
  if (temp) return FX_OK;
  // Text aktualisieren
  CustomMessage("", 0, GetController(target));
  return FX_OK;
  }
  
protected func FxIntDialogPassiveTimer(object target, int fx_num, int time)
  {
  // Text aktualisieren
  DlgUpdatePassiveText(target);
  return FX_OK;
  }

private func DlgUpdatePassiveText(object target)
  {
  var i, color, msg;
  if (!dlg_currmsgs || !GetLength(dlg_currmsgs))
    {
    // zZ keine Passiv-Nachricht
    CustomMessage("", 0, GetController(target));
    }
  else
    {
    // Passiv-Nachricht zusammenbauen (thx M&M-Adventure)
    for(var part in dlg_currmsgs)
      {
      if (i++)
        {
        if(i%2) color = RGB(200,200,200); else color = RGB(255,255,255);
        if (i-1 == dlg_curroption) color = RGB(255,0,0);
        msg = Format("%s|<c %x>%s</c>", msg, color, part);
        }
      else msg = part;
      }
    // Nachricht zeigen
    MsgBox(target, msg, dlg_currmsg_speaker, dlg_currmsg_portrait, true);
    }
  }


/* Dialogstil-Nachrichten */

global func MsgBoxGetTargetByPlayer(object pPlayer)
  {
  var pMsgTarget = GetCursor(GetController(pPlayer));
  if (!pMsgTarget) pMsgTarget = pPlayer;
  return pMsgTarget;
  }

global func MsgBox(object pPlayer, string sMessage, object pSpeaker, string szPortrait, bool fAsMessage)
  {
  // Defaults
  if (!pPlayer) if (!(pPlayer = this)) return false;
  if (!pSpeaker) if (!(pSpeaker = this)) return false;
  if (!sMessage) return false;
  if (!szPortrait) if (!(szPortrait = pSpeaker->~GetMsgPortrait())) szPortrait = "1";
  var pMsgTarget = MsgBoxGetTargetByPlayer(pPlayer);
  // Portrait ermitteln
  var c,i,p2="";
  if (GetChar(szPortrait) == GetChar("@"))
    {
    // @Portrait: Aus dieser Definition holen
    while (c=GetChar(szPortrait, ++i)) p2 = Format("%s%c", p2, c);
    szPortrait = Format("Portrait:%i::%x::%s", DG69, GetColorDw(pSpeaker), p2);
    }
  else
    {
    // Ansonsten direkt vom Sprecher
    szPortrait = Format("Portrait:%i::%x::%s", GetID(pSpeaker), GetColorDw(pSpeaker), szPortrait);
    }
  var iSpeakerNameLen = 0;
  while (GetChar(GetName(pSpeaker), iSpeakerNameLen)) ++iSpeakerNameLen;
  // Farbe ermitteln
  var dwClr = pSpeaker->~GetDlgMsgColor();
  if (!dwClr) dwClr = GetColorDw(pSpeaker);
  if (!dwClr) dwClr = 255;
  // Wie ausgeben?
  if (fAsMessage)
    {
    // Nachricht als reguläre Message; Spieler nicht stoppen
    CustomMessage(Format("<c %x>%s:</c> %s", dwClr, GetName(pSpeaker), sMessage), 0, GetController(pPlayer), 90,70, 0xffffff, MD69, szPortrait);
    }
  else
    {
    // Nachricht als Menü
    // Spieler soll anhalten, damit er nicht ins Verderben läuft
    StopClonkEx(pPlayer);
    if (!CreateMenu(DG69, pMsgTarget, this, 0, "", 0, C4MN_Style_Dialog, false, DG69)) return false;
    AddMenuItem(szPortrait, "", NONE, pMsgTarget, 0, 0, "", C4MN_Add_ImgTextSpec, 0, 0);
    AddMenuItem(Format("<c %x>%s:</c> %s", dwClr, GetName(pSpeaker), sMessage), "", NONE, pMsgTarget, 0, 0, "", C4MN_Add_ForceNoDesc, 0, 0);
    SetMenuDecoration(MD69, pMsgTarget);
    SetMenuTextProgress(iSpeakerNameLen+1, pMsgTarget);
    }
  return true;
  }
  
global func MsgBoxAddText(object pPlayer, string sText)
  {
  var pMsgTarget = MsgBoxGetTargetByPlayer(pPlayer);
  return (AddMenuItem(sText, "", NONE, pMsgTarget, 0, 0, 0, C4MN_Add_ForceNoDesc));
  }
  
global func MsgBoxAddOption(object pPlayer, id idIcon, string sOption, string sCommand, vParam, int extra, xPar)
  {
  // Optionen
  extra |= C4MN_Add_ForceNoDesc;
  var pMsgTarget = MsgBoxGetTargetByPlayer(pPlayer);
  return (AddMenuItem(sOption, sCommand, idIcon, pMsgTarget, 0, vParam, 0, extra, xPar));
  }
  
  
/* Hilfsfunktionen */

// Clonk anhalten
global func StopClonk(object target)
  {
  if (!target) if (!this) return false;
  SetComDir(COMD_Stop, target);
  SetCommand(target, "None");
  Call("~StopAiming");
  return true;
  }
  
// Clonk für Dialog vorbereiten: Anhalten, Menüs schließen, etc.
global func StopClonkEx(object target, bool keep_action)
  {
  if (!target) if (!this) return;
  if (!keep_action) if (target) ObjectSetAction(target, "Walk"); else SetAction("Walk");
  StopClonk(target);
  CloseMenu(target);
  }
  