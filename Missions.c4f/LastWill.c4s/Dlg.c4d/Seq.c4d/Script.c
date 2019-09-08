/*-- Sequenzen --*/

#strict 2

// Eigene Sequenzen werden realisiert, indem man mit #appendto SQ69 an dieses Script anhängt
// Achtung: Dadurch liegen die Funktionen aller Sequenzen im gleichen Scope - auf eindeutige Namen ist zu achten!
// Lokale Variablen des Sequenzobjektes haben ein seq_-Prefix

// Öffentlich
local seq_name;         // Dialog-ID
local seq_players;      // Spieler, die die Sequenz verfolgen
local seq_clonks;       // Clonks, die an der Sequenz Teilnehmen
local seq_extra;        // Variable zum Sequenzeigenen Gebrauch

// Intern
local seq_counter;      // Sequenzzähler
local seq_waitfordlg;   // Wenn gesetzt, läuft der Zähler erst weiter, wenn die Mehrzahl der zuhörenden Spieler ihr Menü geschlossen hat
local seq_views;        // Sequenz-Sichtobjekte für alle Spieler
local seq_wantskip;     // true für alle Spieler, die die Sequenz überspringen wollen


/* Globale Funktion zum Starten einer Sequenz */

global func StartSequence(string sequence_name, audience)
  {
  // Sequenzobjekt erzeugen und starten
  var seq = CreateObject(SQ69, -GetX(), -GetY(), NO_OWNER);
  if (!seq->SeqStart(sequence_name, audience)) return false;
  return seq;
  }
  
global func FindSequence(string name)
  {
  // Sequenz mit Namen suchen
  var sequences = FindObjects(Find_ID(SQ69));
  for (var seq in sequences)
    if (!name || LocalN("seq_name", seq) == name) return seq;
  return 0;
  }


/* Sequenzstart */

public func SeqStart(string sequence_name, audience)
  {
  var i;
  // Reset
  SeqClear();
  // Daten sichern
  seq_name = sequence_name;
  // Initialisierung
  seq_players = seq_clonks = seq_views = [];
  // Zuhörer ermitteln: Default sind alle Clonks und Spieler
  if (!audience)
    {
    for (i=0; i<GetPlayerCount(C4PT_User); ++i)
      AddAudiencePlayer(GetPlayerByIndex(i, C4PT_User), true);
    }
  else if (GetType(audience) == C4V_Array)
    {
    // Zuhörer ist eine Liste; entweder von Spielern oder von Objekten (oder gemischt)
    for (var aud_item in audience)
      if (GetType(aud_item) == C4V_C4Object)
        {
        // Zuhörer Einzelobjekt des Spielers
        AddAudienceObject(aud_item);
        AddAudiencePlayer(GetController(aud_item));
        }
      else
        {
        // Zuhörer Spieler mit allen Clonks
        AddAudiencePlayer(aud_item, true);
        }
    }
  else if (GetType(audience) == C4V_C4Object)
    {
    // Zuhörer ist nur ein Clonk und der zugehörige Spieler
    AddAudienceObject(audience);
    AddAudiencePlayer(GetController(audience));
    }
  // Keine Spieler? Dann hat es keinen Sinn
  if (!GetLength(seq_players)) { SeqStop(); return false; }
  // Timer starten
  AddEffect("IntTimer", this, 1, 10, this);
  // Start-Call
  if (seq_name) ProtectedCall(this, Format("Seq%sStart", seq_name));
  // Und los!
  return true;
  }
  
public func AddAudiencePlayer(int plr, bool add_crew)
  {
  // Spieler schon enthalten?
  if (GetIndexOf(plr, seq_players) > -1) return false;
  // Ansonsten hinzufügen und besten Cursor merken!
  var i = GetLength(seq_players);
  seq_players[i] = plr;
  var seq_view_trg = GetViewCursor(plr);
  if (!seq_view_trg) seq_view_trg = GetCursor(plr);
  if (!seq_view_trg) seq_view_trg = GetCrew(plr, 0);
  // ggf. Crew zu Zuhörern hinzufügen
  var obj, j=0;
  while (obj = GetCrew(plr, j++)) AddAudienceObject(obj);
  // Sichtobjekt erschaffen
  seq_views[i] = CreateObject(SV69, 0,0, plr);
  seq_views[i]->InitView(seq_view_trg, this);
  return true;
  }

public func AddAudienceObject(object obj)
  {
  // Nur wenn nicht schon drin
  if (GetIndexOf(obj, seq_clonks) > -1) return false;
  // Clonk deaktivieren und aufnehmen
  SetCrewEnabled(false, obj);
  SetInvincible(obj);
  SetCommand(obj, "None");
  SetComDir(COMD_Stop, obj);
  seq_clonks[GetLength(seq_clonks)] = obj;
  return true;
  }


/* Sequenztimer */

protected func FxIntTimerTimer(object obj, int num)
  {
  // Wartet noch auf Dialog?
  if (seq_waitfordlg)
    {
    if (SeqIsInDlg()) return FX_OK;
    seq_waitfordlg = false;
    }
  // Nächstes Frame
  if (seq_name) ProtectedCall(this, Format("Seq%s%d", seq_name, ++seq_counter));
  return FX_OK;
  }


/* Hilfsfunktionen */

private func SeqIsInDlg()
  {
  // Ist im Dialog, wenn die Hälfte oder mehr der Spieler noch im Dialog ist
  var in_cnt=0,  total_cnt=GetLength(seq_players);
  for (var plr in seq_players)
    if (IsPlrInMenu(plr)) ++in_cnt;
  return in_cnt >= (total_cnt+1)/2;
  }

private func IsPlrInMenu(int plr)
  {
  var cursor = GetCursor(GetController(plr));
  if (!cursor)
    {
    for (var check in seq_clonks)
      if (GetController(check) == plr)
        { cursor = check; break; }
    if (!cursor) return false;
    }
  // Enginemenü oder Dialog
  if (GetMenu(cursor) == DG69) return true;
  // Ringmenü
  if (cursor->~IsMenu()) return true;
  // Input-Dlg
  if (TestMessageBoard(plr, true)) return true;
  // Kein Menü
  return false;
  }
  
private func SeqDing()
  {
  for (var plr in seq_players) Sound("Ding", true, 0, 100, plr+1);
  return true;
  }

private func SeqDlgMsg(string msg, object speaker, string portrait)
  {
  return SeqMsg(msg, speaker, portrait, true);
  }

private func SeqGetSpeaker(int idx)
  {
  // Normalerweise den indizierten Clonk aus dem Array
  // Falls ein Clonk weg ist (Spieler hat das Spiel verlassen), zurückfallen auf den nächsten existierenden Clonk
  var check, n=GetLength(seq_clonks);
  for (var i=0; i<n; ++i)
    if (check = seq_clonks[(i + idx)%n])
      return check;
  return 0;
  }

private func SeqMsg(string msg, object speaker, string portrait, bool is_dlg_message)
  {
  // Default: Sprecher aus den teilnehmenden Clonks
  if (!speaker) if (!(speaker = SeqGetSpeaker())) return false;
  for (var obj in seq_views) if (obj) MsgBox(obj, msg, speaker, portrait, !is_dlg_message);
  return true;
  }

private func SeqSetCommand(string cmd, object trg, int x, int y, object trg2)
  {
  // Command an alle teilnehmenden Clonks
  for (var obj in seq_clonks) if (obj) SetCommand(obj, cmd, trg, x, y, trg2);
  return true;
  }
  
private func SeqGetPlayerCount() { return GetLength(seq_players); }

private func SeqSetView(object trg)
  {
  for (var obj in seq_views) if (obj) obj->SetAttach(trg);
  return true;
  }

private func SeqResetView()
  {
  // Sicht zurück auf die Clonks
  for (var obj in seq_views) if (obj) obj->ResetAttach();
  return true;
  }

private func SeqStopClonks()
  {
  for (var obj in seq_clonks) if (obj)
    {
    SetCommand(obj, "None");
    SetComDir(COMD_Stop, obj);
    }
  return true;
  }

private func SeqTM(v1, v2) { if (GetLength(seq_players) > 1) return v2; else return v1; }


/* Sequenz überspringen */

public func OpenSkipMenu(int plr)
  {
  // Überspringen erlaubt?
  if (!seq_name) return false;
  var skip;
  if (!(skip = ProtectedCall(this, Format("Seq%sAllowSkip", seq_name)))) return false;
  if (GetType(skip) != C4V_String) skip = "$TxtSkip$";
  var i = GetIndexOf(plr, seq_players);
  if (i == -1) return false;
  var view_obj = seq_views[i];
  if (!view_obj) return false;
  if (!CreateMenu(SV69, view_obj, this, 0, skip, 0, C4MN_Style_Context, false, SV69)) return false;
  AddMenuItem(skip, Format("QuerySkipSequence(%d)", plr), SV69, view_obj, 0, 0, "$TxtSkip$");
  return true;
  }

protected func QuerySkipSequence(int plr)
  {
  var i = GetIndexOf(plr, seq_players);
  if (i == -1) return false;
  seq_wantskip[i] = 1;
  if (!CheckSkipSequence())
    {
    // Noch keine Mehrheit: Nur Wunsch loggen
    Log("$MsgWantSkip$", GetPlayerName(plr));
    }
  return true;
  }

protected func CheckSkipSequence()
  {
  // Überspringen, wenn mehr als die Hälfte dafür ist
  var nwant = 0, n = GetLength(seq_players);
  for (var b in seq_wantskip) nwant += b;
  if (nwant > n/2)
    {
    Log("$MsgSeqSkipped$");
    ProtectedCall(this, Format("Seq%sSkip", seq_name));
    return true;
    }
  return false;
  }


/* Sequenzende */

public func SeqStop(bool ding)
  {
  if (ding) SeqDing();
  if (seq_name) ProtectedCall(this, Format("Seq%sStop", seq_name));
  return RemoveObject();
  }

protected func Destruction()
  {
  SeqClear();
  }

private func SeqClear()
  {
  // Clonks aus Sequenz freigeben
  if (seq_clonks) for (obj in seq_clonks) if (obj) 
    {
    SetCrewEnabled(true, obj);
    DoBreath(10000, obj);
    SetVincible(obj);
    }
  // Sichtobjekte löschen
  var obj;
  if (seq_views) for (obj in seq_views) if (obj) RemoveObject(obj);
  // Keine Verfolgung mehr
  seq_players = seq_clonks = seq_views = seq_wantskip = [];
  seq_name = 0;
  seq_counter = 0;
  seq_waitfordlg = false;
  }
  
  
/* Spieler während Sequenz eliminiert? Dann raus mit ihm! */

protected func RemovePlayer(int plr)
  {
  var i = GetIndexOf(plr, seq_players);
  if (i == -1) return false;
  var n = GetLength(seq_players) - 1;
  if (!n) { SeqStop(); return false; }
  seq_players[i] = seq_players[n];
  seq_views[i] = seq_views[n];
  SetLength(seq_players, n);
  SetLength(seq_views, n);
  // Ergibt das Verlassen des Spielers eine Mehrheit fürs Überspringen der Sequenz?
  CheckSkipSequence();
  return false;
  }
  
  
/* Unverwundbarkeit */

global func SetInvincible(object obj)
  {
  if (!obj) if (!(obj = this)) return false;
  return AddEffect("IntInvincibility", obj, 200, 0, obj);
  }
  
global func SetVincible(object obj)
  {
  if (!obj) if (!(obj = this)) return false;
  return RemoveEffect("IntInvincibility", obj);
  }

global func FxIntInvincibilityDamage(object obj, int iEff, int iDmg, int iCause, int iCausePlr) { }

global func FxIntInvincibilityEffect(string szNewEffectName)
  {
  // nicht anbrennen lassen
  if (WildcardMatch(szNewEffectName,"*Fire*")) return FX_Effect_Deny;
  // gleichen Effekt ablehnen
  if (szNewEffectName == "IntInvincibility") return FX_Effect_Deny;
  } 
  
