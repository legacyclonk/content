/* Königsmord - Königreichauswahl */

#strict

local pRule; // Regel, die das Menü steuert
// Locals: Zuordnung Menüindex -> Königsindex

/* Initialisierung */

public func Initialize()
  {
  // Königsmord-Regel suchen
  pRule = FindObject(_KTK);
  if (!pRule) return(RemoveObject());
  // Königreich direkt nach Team bestimmen
  var iKingdom = iKingdom = pRule->_KTK::GetIndexedKingdom(GetPlayerTeam(GetOwner()) - 1);
  var szKDName, idKD, szKDDesc;
  if (!pRule->_KTK::GetKingdomData(iKingdom, szKDName, idKD, szKDDesc)) return(RemoveObject());
  MnuChooseKingdom(idKD, iKingdom);
    
  /*// Alle Clonks des Besitzers einfangen
  var iCrew, pCrew;
  while (pCrew = GetCrew(GetOwner(), iCrew++)) Enter(this(), pCrew);
  // Menü(s) öffnen
  Timer();*/
  // Fertig
  return(1);
  }


/* Timer */

/*protected func Timer()
  {
  // Regel noch da?
  if (!pRule) return(RemoveObject());
  // Menü für alle Clonks von der Regel erstellen lassen (könnte z.B. durch LAufzeitbeitritt geschlossen worden sein
  var i, pClonk;
  while (pClonk = Contents(i++))
    if (!GetMenu(pClonk))
      OpenMenu(pClonk);
  }*/


/* Menüsteuerung */

/*
private func OpenMenu(pClonk)
  {
  // Keine Clonkangabe: Aufruf für alle enthaltenen Clonks
  if (!pClonk)
    {
    var j, pClonk2;
    //while (pClonk2 = Contents(j++)) if (!OpenMenu(pClonk2)) return();
    return(1);
    }
  // Vorheriges Menü schließen
  if (GetMenu(pClonk)) CloseMenu(pClonk);
  // Menü öffnen
  CreateMenu(_KTK, pClonk, this(), 0, "$MnuKingdomChoice$", 0, 1);
  // Einträge hinzufügen
  var i, k, pKing, iKingdom;
  while (iKingdom = pRule->_KTK::GetIndexedKingdom(i++))
    {
    var szKDName, idKD, szKDDesc;
    if (!pRule->_KTK::GetKingdomData(iKingdom, szKDName, idKD, szKDDesc)) continue;
    AddMenuItem(szKDName, "MnuChooseKingdom", idKD, pClonk, 0, iKingdom, szKDDesc);
    Local(k++) = iKingdom;
    }
  // Kein Königreich zur Auswahl? Objekt mit allen Clonks löschen (Spieler eliminiert)
  if (!k)
    {
    Log("$MsgNoKingdoms$");
    EliminatePlayer(GetOwner());
    RemoveObject();
    return();
    }
  // Menüauswahl
  OnMenuSelection(GetMenuSelection(pClonk), pClonk);
  // OK, Erfolg
  return(1);
  }*/

/*protected func MenuQueryCancel() { return(); } // Kein Menüabbruch */

/*protected func OnMenuSelection(iMenuIndex, pClonk)
  {
  // Regel noch da?
  if (!pRule) return(RemoveObject());
  // Position des ausgewählten Königreiches ermitteln (Bei Fehler (Königreich tot) Menüs neu erstellen)
  var kx,ky;
  if (!pRule->_KTK::GetKingdomPos(Local(iMenuIndex), kx, ky)) return(OpenMenu());
  // Sichtposition danach setzen
  SetPosition(kx, ky); SetPlrView(GetOwner(), pClonk);
  // Fertig
  return(1);
  }*/

protected func MnuChooseKingdom(idKD, iKingdom)
  {
  // Sicherheit
  if (!pRule) return(RemoveObject());
  // Tärää - Königreich ausgewählt!
  // Alle Menüs schließen
  /*var i, pClonk;
  while (pClonk = Contents(i++)) CloseMenu(pClonk);*/
  // Benachrichtigung an den Königsmord
  if (!pRule->_KTK::ChooseKingdom(GetOwner(), iKingdom)) return();
  // Erfolg: Wir sind überflüssig :(
  return(RemoveObject());
  }


/* Steuerung */

/*protected func ContainedThrow(pClonk) { [Burgauswahl] return(OpenMenu(pClonk)); } // Menü ablehnen
protected func RejectCollect() { return(1); } // Gegenstände ablehnen */
