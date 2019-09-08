/*-- Königsmord --*/

#strict

/* Lokale Variablen */

local iKingCount;           // Anzahl der Könige
local iKingLocals;          // Anzahl der lokalen Variablen, die für Könige reserviert sind
// Local(0..iKingLocals-1)             // Könige (Pointer)
// Local(iKingLocals..iKingLocals*3-1) // X- und Y-Startposition der Königreiche (XPos immer != 0 für Königreiche; 0 für tote Königreiche)
// Local(iKingLocals*3..oo)            // Index des Spielerteams



/* Initialisierung */

protected func Initialize()
  {
  // Immer nach oben links für einfachere Offsets
  SetPosition();
  // Das Ziel muss vor den Spielern aktiviert werden
  if (GetPlayerCount()) return (Log("$ErrActivateAfterPlrJoin$"), RemoveObject());
  // Könige suchen
  var pKing;
  while (pKing = FindObject(KING, 0,0,0,0, 0, 0,0, 0, pKing))
    // Neuer König gefunden - setzen
    King(++iKingCount) = pKing;
  // Ausreichen Könige da?
  if (iKingCount < 2) return (Log("$ErrNeedsTwoKings$"), RemoveObject());
  // Local-Listen initialisieren
  iKingLocals = iKingCount;
  // Startpositionen setzen
  var iKing=iKingLocals;
  while (iKing)
    {
    StartPosX(iKing) = Max(GetX(pKing= King(iKing)), 1);
    StartPosY(iKing) = GetY(pKing);
    --iKing;
    }
  // Prüfung starten
  SetAction("Checking");
  // Fertig
  return(1);
  }



/* Spielerbeitritt */

protected func InitializePlayer(iPlr)
  {
  // Spiel schon zu Ende?
  if (!IsGameRunning()) return(EliminatePlayer(iPlr));
  // Jeder Spieler kann sich sein Königreich aussuchen
  CreateObject(_KSL, 10,10, iPlr);
  // Das Auswahlobjekt wird alle weiteren Schritte übernehmen
  return(1);
  }

public func ChooseKingdom(iPlr, iKingdom)
  {
  // Spiel schon zu Ende?
  if (!IsGameRunning()) return(EliminatePlayer(iPlr));
  // Gewähltes Königreich prüfen
  var pKing = King(iKingdom);
  if (!pKing) return();
  // Spieler prüfen (Schon in einem Team?)
  if (PlayerTeam(iPlr)) return();
  // Spieler in dieses Königreich versetzen
  PlayerTeam(iPlr) = iKingdom;
  // Wenn der König noch nicht zugewiesen ist: Königreich übertragen
  // Beitrittsnachricht
  var szJoinMessage;
  var szKDName, idKD, szKDDesc;
  GetKingdomData(iKingdom, szKDName, idKD, szKDDesc);
  if (GetOwner(pKing)<0)
    {
    AssignKing2Plr(iKingdom, iPlr, -1);
    szJoinMessage = RandomJoinLeadMessage(GetPlayerName(iPlr), szKDName);
    }
  else
    szJoinMessage = RandomJoinMessage(GetPlayerName(iPlr), szKDName);
  PlayerMessage(iPlr, szJoinMessage);
  Log(szJoinMessage);
  // Verfeindung mit allen anderen Spielern anpassen
  for (var i,j=GetPlayerCount(); j>1; ++i)
    if (GetPlayerName(i) && i!=iPlr)
      {
      --j;
      var fHostile = PlayerTeam(i) != iKingdom;
      SetHostility(i, iPlr, fHostile, 1);
      SetHostility(iPlr, i, fHostile, 1);
      }
  // Spieler starten lassen
  LaunchPlayer(iPlr);
  // Fertig
  return(1);
  }

public func LaunchPlayer(iPlr, fRelaunch)
  {
  // Startposition ermitteln
  var iKingdom = PlayerTeam(iPlr);
  if (!iKingdom) return();
  var sx, sy; GetKingdomPos(iKingdom, sx, sy);
  // Vorerst einfach die gesamte Spielercrew plazieren
  var iCrew, pCrew;
  while (pCrew = GetCrew(iPlr, iCrew++))
    {
    DoEnergy(10000, pCrew);
    while (DoMagicEnergy(1, pCrew)) ;
    if (Contained(pCrew)) if (GetID(pCrew) != KING) Exit(pCrew);
    SetPosition(sx + Random(21)-10, sy - Random(20) - 10, pCrew);
    SetDir(Random(2), pCrew);
    }
  // Flagge gibts gratis
  var pFlagClonk = GetCursor(iPlr); if (!pFlagClonk) pFlagClonk = GetHiRank(iPlr);
  if (pFlagClonk) CreateContents(FLAG, pFlagClonk);
  // OK, fertig
  return(1);
  }

private func AssignKing2Plr(iKingdom, iPlr, iFromPlayer)
  {
  // Keine Besitzeränderung?
  if (iPlr == iFromPlayer) return(1);
  // Kein Besitzer: Königreich annehmen
  var obj;
  var iKDX, iKDY, iKDWdt, iKDHgt;
  if (iFromPlayer<0)
    // Nur Objekte im Bereich des Königreiches
    if (!GetKingdomExtents(iKingdom, iKDX, iKDY, iKDWdt, iKDHgt)) return();
  // Ansonsten: Alle Objekte mit entsprechendem Besitzer
  while (obj = FindObjectOwner(0, iFromPlayer, iKDX, iKDY, iKDWdt, iKDHgt, 0, 0,0, obj))
    if (~GetOCF(obj) & OCF_Living())
      SetOwner(iPlr, obj);
  // Kein Spieler: König übernehmen
  var pKing;
  pKing = King(iKingdom);
  if (!pKing) return();
  if (GetOwner(pKing)>=0)
    {
    // Königsbesitzer vorhanden: Info löschen
    var pDelKing = CreateObject(GetID(pKing), 0,0, GetOwner(pKing));
    GrabObjectInfo(pKing, pDelKing);
    RemoveObject(pDelKing);
    }
  // König in die eigene Crew
  SetOwner(iPlr, pKing); MakeCrewMember(pKing, iPlr);
  SetOwner(iPlr, pKing); 
  // König auswählen
  UnselectCrew(iPlr); 
  SetCursor(iPlr, pKing);
  SelectCrew(iPlr, pKing, 1);
  // Fertig
  return(1);
  }



/* Spieleraustritt */

protected func RemovePlayer(iPlr)
  {
  // Spiel schon zu Ende? Keine weiteren Checks nötig (normalerweise sollte iPlrTeam==0 sein; aber es könnten auch in der Endphase noch Spieler aus dem Gewinnerteam austreten!)
  if (!IsGameRunning()) return(1);
  // Spielerteam zugewiesen? (Schlägt fehl, falls der Spieler in EliminateTeam() eliminiert wird!)
  var iPlrTeam = PlayerTeam(iPlr);
  if (!iPlrTeam) return(1);
  // Team löschen
  PlayerTeam(iPlr) = 0;
  // Hat das Team noch einen König?
  var pKing = King(iPlrTeam);
  if (!pKing) return(1);
  // Gehört der König diesem Spieler?
  if (GetOwner(pKing) == iPlr)
    {
    // Existiert noch ein anderer Spieler in diesem Team?
    var iOtherTeamPlr = FindPlayerOfTeam(iPlrTeam);
    if (iOtherTeamPlr < 0)
      // Anderer Spieler nicht gefunden: Team eliminieren
      EliminateTeam(iPlrTeam);
    else
      // Anderer Spieler gefunden: König(reich) an diesen Spieler übergeben
      AssignKing2Plr(iPlrTeam, iOtherTeamPlr, iPlr);
    return(1);
    }
  // OK, eine unterstützende Kraft fällt weg
  return(1);
  }

protected func EliminateTeam(iKingdom)
  {
  // Königreich schon weg?
  if (!StartPosX(iKingdom)) return();
  // Versager-Nachricht
  var szKDName, idKD, szKDDesc;
  if (!GetKingdomData(iKingdom, szKDName, idKD, szKDDesc)) return();
  Log(RandomEliminationMsg(szKDName));
  // Alle Spieler aus dem Königreich vernichten
  for (var i,j=GetPlayerCount(); j; ++i) if (GetPlayerName(i))
    {
    --j;
    if (PlayerTeam(i) == iKingdom)
      {
      PlayerTeam(i) = 0;
      EliminatePlayer(i);
      }
    }
  // Der König könnte noch leben, wenn zum Beispiel der Thron erobert wurde
  var pKing = King(iKingdom);
  if (GetAlive(pKing)) Kill(pKing);
  // Ein Königreich weniger
  King(iKingdom) = StartPosX(iKingdom) = StartPosY(iKingdom) = 0;
  --iKingCount;
  // Fertig
  return(1);
  }

public func PlayerForfeit(iPlr, iByPlayer)
  {
  // Ein Spieler hat das Leben seines Königreiches verwirkt
  // Gültiger Spieler?
  if (iPlr<0) return();
  // Königreich zugewiesen?
  var iKingdom = PlayerTeam(iPlr);
  // Ansonsten haftet der Spieler persönlich :)
  if (!iKingdom) return(EliminatePlayer(iPlr));
  // Königreich vernichten
  return(EliminateTeam(iKingdom));
  }

public func PlayerEliminated(iPlr)
  {
  // Spiel schon zu Ende?
  if (!IsGameRunning()) return();
  // Gehört der Spieler einem Königreich an?
  var iKingdom = PlayerTeam(iPlr);
  if (!iKingdom) return();
  // Ist er selber nicht der König?
  var pKing = King(iKingdom);
  if (!pKing) return();
  if (GetOwner(pKing) == iPlr) return();
  // Dann kann ihn der König wieder zum Leben erwecken
  // Thron suchen
  var pThrone = FindObjectOwner(THRN, GetOwner(pKing));
  if (!pThrone) return();
  // Relaunch im Thron ermöglichen
  return(pThrone->PlayerEliminated(iPlr));
  }




/* Spielziel */

protected func Checking()
  {
  // Timer: Prüfen, ob ein Königreich eliminiert ist
  var i=iKingLocals, pKing;
  while (i--)
    if (pKing = Local(i))
      if (!GetAlive(pKing))
        // König tot: Team eliminieren
        EliminateTeam(i+1);
      else; // spart einen Block :)
    else if (Local(iKingLocals + i*2))
      // Kein König mehr da, aber dennoch ein Königreich zugewiesen (XPos != 0): Team auch eliminieren
      EliminateTeam(i+1);
  // Ende?
  if (iKingCount <= 1) EndGame();
  }

private func EndGame()
  {
  // Nur, wenn noch laufend
  if (!IsGameRunning()) return();
  // Sieger ermitteln
  var iWinner = FindWinner();
  // Siegernachricht
  Sound("Trumpet", 1);
  var szWinMsg, szKDName, idKD, szKDDesc;
  if (iWinner) if (GetKingdomData(iWinner, szKDName, idKD, szKDDesc)) szWinMsg = RandomWinningMsg(szKDName);
  // Anonyme Nachricht
  if (!szWinMsg) szWinMsg = RandomWinningMsg2();
  // Nachricht ausgeben
  Message("@%s", 0, szWinMsg);
  Log(szWinMsg);
  // Alle Spieler eliminieren, die nicht im Team sind (z.B. noch kein Königreich gewählt)
  for (var i,j=GetPlayerCount(); j; ++i) if (GetPlayerName(i))
    {
    --j;
    if (PlayerTeam(i) != iWinner)
      {
      PlayerTeam(i) = 0;
      EliminatePlayer(i);
      }
    }
  // Neue Endaktivität
  SetAction("Ending");
  // Fertig
  return(1);
  }

private func FindWinner()
  {
  // Einen Gewinner in allen Königreichen suchen
  var i = iKingLocals, pKing;
  while (i)
    if (pKing = King(i--))
      // König vorhanden und lebt: Gewinner gefunden!
      if (GetAlive(pKing)) return(i+1);
  // Kein Gewinner
  return();     
  }

protected func EndingDone()
  {
  // Ende Gelände
  GameOver();
  return(RemoveObject());
  }



/* Königreichverwaltung */

private func & King(iIndex) { return(Local(iIndex - 1)); }
private func & StartPosX(iTeam) { return(Local(iKingLocals + iTeam*2 - 2)); }
private func & StartPosY(iTeam) { return(Local(iKingLocals + iTeam*2 - 1)); }
private func & PlayerTeam(iPlr) { return (Local(iKingLocals*3 + iPlr)); }

public func GetIndexedKingdom(iIndex)
  {
  // Nur zugewiesene Könige zählen
  var i=0;
  while (i < iKingLocals) if (Local(i++)) if (!iIndex--) return(i);
  // Index außerhalb der Grenzen: 0 (Kein Königreich)
  return();
  }

public func GetKingdomData(iKingdom, &rszKDName, &ridKD, &rszKDDesc)
  {
  // König suchen
  var pKing = King(iKingdom);
  if (!pKing) return();
  // Namen setzen
  rszKDName = pKing->~GetKingdomName();
  if (!GetChar(rszKDName)) rszKDName = Format("$MsgUnnamed$ #%d", iKingdom);
  // ID fürs Symbol setzen
  ridKD = GetID(pKing);
  // Desc setzen
  rszKDDesc = "$MsgAKingdom$";
  // Fertig
  return(1);
  }

public func GetKingdomPos(iKingdom, &riX, &riY)
  {
  // Königreich zugewiesen?
  if (!King(iKingdom)) return();
  // Position setzen
  riX = StartPosX(iKingdom);
  riY = StartPosY(iKingdom);
  // Fertig
  return(1);
  }

private func FindPlayerOfTeam(iKingdom, iIndex)
  {
  // Alle Spieler durchgehen
  for (var i,j=GetPlayerCount(); j; ++i)
    if (GetPlayerName(i))
      {
      --j;
      // Spieler gehört dem Team an?
      if (PlayerTeam(i) == iKingdom)
        // Ergebnis gefunden!
        if (!iIndex--) return(i);
      }
  // Kein Ergebnis
  return(-1);
  }



/* Hilfsfunktionen */

private func LooksLikeKingdom(iX, iY)
  {
	// Außerhalb der Landschaftsgrenzen?
	if (iX<0 || iX>=LandscapeWidth() || iY<0 || iY>=LandscapeHeight()) return();
  // Zur Sicherheit, damit nicht an 0/0 gesucht wird...
  if (!iX && !iY) ++iY;
  // Königreich == Burg
  var obj;
  while (obj = FindObject(0, iX,iY,0,0, 0, 0,0, NoContainer(), obj))
    if (obj->~CastlePartWidth()) return(1);
  // Kein Burgteil - Kein Königreich :(
  return();
  }

private func GetKingdomExtents(iKingdom, &rX, &rY, &rWdt, &rHgt)
  {
  // Sicherheit
  if (!King(iKingdom)) return();
  // Szenarienspezifisch?
  if (GameCall("KingdomExtents", King(iKingdom)))
    {
    rX = GameCall("KingdomExtentsX", King(iKingdom));
    rY = GameCall("KingdomExtentsY", King(iKingdom));
    rWdt = GameCall("KingdomExtentsWdt", King(iKingdom));
    rHgt = GameCall("KingdomExtentsHgt", King(iKingdom));
    return(1);
    }
  // Startposition suchen
  var sx = StartPosX(iKingdom), sy = StartPosY(iKingdom);
  // Von dort nach links nach einem Ende des Königreiches suchen
  rX=sx; while (LooksLikeKingdom(rX, sy)) rX -= 40;
  // Suche nach oben
  rY=sy; while (LooksLikeKingdom(sx, rY)) rY -= 40;
  // Suche nach rechts
  rWdt = sx-rX; while (LooksLikeKingdom(rX+rWdt, sy)) rWdt += 40;
  // Suche nach unten
  rHgt = sy-rY; while (LooksLikeKingdom(sx, rY+rHgt)) rHgt += 40;
  // Fertig; Königreich bestimmt
  return(1);
  }

private func RandomEliminationMsg(szTeamName)
  {
  var iMsg = Random($MsgEliminatedCnt$);
  if (!iMsg--) return(Format("$MsgEliminated0$", szTeamName));
  if (!iMsg--) return(Format("$MsgEliminated1$", szTeamName));
  if (!iMsg--) return(Format("$MsgEliminated2$", szTeamName));
  if (!iMsg--) return(Format("$MsgEliminated3$", szTeamName));
  if (!iMsg--) return(Format("$MsgEliminated4$", szTeamName));
  if (!iMsg--) return(Format("$MsgEliminated5$", szTeamName));
  if (!iMsg--) return(Format("$MsgEliminated6$", szTeamName));
  if (!iMsg--) return(Format("$MsgEliminated7$", szTeamName));
  if (!iMsg  ) return(Format("$MsgEliminated8$", szTeamName));
               return(Format("$MsgEliminated9$", szTeamName));
  }

private func RandomWinningMsg(szTeamName)
  {
  var iMsg = Random($MsgWonCnt$);
  if (!iMsg--) return(Format("$MsgWon0$", szTeamName));
  if (!iMsg--) return(Format("$MsgWon1$", szTeamName));
  if (!iMsg--) return(Format("$MsgWon2$", szTeamName));
  if (!iMsg--) return(Format("$MsgWon3$", szTeamName));
  if (!iMsg--) return(Format("$MsgWon4$", szTeamName));
  if (!iMsg--) return(Format("$MsgWon5$", szTeamName));
  if (!iMsg--) return(Format("$MsgWon6$", szTeamName));
  if (!iMsg--) return(Format("$MsgWon7$", szTeamName));
  if (!iMsg  ) return(Format("$MsgWon8$", szTeamName));
               return(Format("$MsgWon9$", szTeamName));
  }

private func RandomWinningMsg2()
  {
  var iMsg = Random($MsgWonACnt$);
  if (!iMsg--) return("$MsgWonA0$");
  if (!iMsg--) return("$MsgWonA1$");
  if (!iMsg--) return("$MsgWonA2$");
  if (!iMsg--) return("$MsgWonA3$");
  if (!iMsg--) return("$MsgWonA4$");
  if (!iMsg--) return("$MsgWonA5$");
  if (!iMsg--) return("$MsgWonA6$");
  if (!iMsg--) return("$MsgWonA7$");
  if (!iMsg  ) return("$MsgWonA8$");
               return("$MsgWonA9$");
  }

private func RandomJoinLeadMessage(szPlayerName, szTeamName)
  {
  var iMsg = Random($MsgKingCnt$);
  if (!iMsg--) return(Format("$MsgKing0$", szPlayerName, szTeamName));
  if (!iMsg--) return(Format("$MsgKing1$", szPlayerName, szTeamName));
  if (!iMsg--) return(Format("$MsgKing2$", szPlayerName, szTeamName));
  if (!iMsg--) return(Format("$MsgKing3$", szPlayerName, szTeamName));
  if (!iMsg--) return(Format("$MsgKing4$", szPlayerName, szTeamName));
  if (!iMsg--) return(Format("$MsgKing5$", szPlayerName, szTeamName));
  if (!iMsg--) return(Format("$MsgKing6$", szPlayerName, szTeamName));
  if (!iMsg--) return(Format("$MsgKing7$", szPlayerName, szTeamName));
  if (!iMsg--) return(Format("$MsgKing8$", szPlayerName, szTeamName));
               return(Format("$MsgKing9$", szPlayerName, szTeamName));
  }

private func RandomJoinMessage(szPlayerName, szTeamName)
  {
  var iMsg = Random($MsgTeamJoinCnt$);
  if (!iMsg--) return(Format("$MsgTeamJoin0$", szTeamName, szPlayerName));
  if (!iMsg--) return(Format("$MsgTeamJoin1$", szTeamName, szPlayerName));
  if (!iMsg--) return(Format("$MsgTeamJoin2$", szTeamName, szPlayerName));
  if (!iMsg--) return(Format("$MsgTeamJoin3$", szTeamName, szPlayerName));
  if (!iMsg--) return(Format("$MsgTeamJoin4$", szTeamName, szPlayerName));
  if (!iMsg--) return(Format("$MsgTeamJoin5$", szTeamName, szPlayerName));
  if (!iMsg--) return(Format("$MsgTeamJoin6$", szTeamName, szPlayerName));
  if (!iMsg--) return(Format("$MsgTeamJoin7$", szTeamName, szPlayerName));
  if (!iMsg--) return(Format("$MsgTeamJoin8$", szTeamName, szPlayerName));
               return(Format("$MsgTeamJoin9$", szTeamName, szPlayerName));
  }



/* Status */

private func IsGameRunning() { return(GetAction() eq "Checking"); }

public func IsFulfilled()
  {
  // Nur ein König kann überleben!
  return (iKingCount < 2);
  }

protected func Activate(iPlr)
  {
  var iTeam = PlayerTeam(iPlr), szTeamMsg;
  var szKDName, idKD, szKDDesc;
  if (iTeam) if (GetKingdomData(iTeam, szKDName, idKD, szKDDesc)) szTeamMsg = Format("$MsgTeamInfo$", szKDName);
  if (!szTeamMsg) szTeamMsg = "$MsgNoTeam$";
  var szKingdomMsg;
  if (iTeam)
    // Selber im Team
    if (iKingCount>1)
      if (iKingCount == 2)
        // Nur ein weiteres Team
        szKingdomMsg = "$MsgOneOtherTeams$";
      else
        // Mehr weitere Teams
        szKingdomMsg = Format("$MsgOtherTeams$", iKingCount-1);
    else
      // Keine weiteren Teams
      szKingdomMsg = "$MsgNoOtherTeams$";
  else
    // Kein eigenes Team
    if (iKingCount)
      if (iKingCount == 1)
        // Nur ein weiteres Team
        szKingdomMsg = "$MsgOneTeam$";
      else
        // Es gibt andere Teams
        szKingdomMsg = Format("$MsgTeams$", iKingCount);
    else
      // Es gibt keine Königreiche
      szKingdomMsg = "$MsgNoTeams$";
  // Nachricht zusammensetzen und ausgeben
  return(MessageWindow(Format("%s|%s|%s", GetDesc(), szTeamMsg, szKingdomMsg), iPlr));
  }

public func IsUnjoinedPlayer()
  {
  // Zurückgeben, ob noch Spieler ohne Team vorhanden sind
  for (var i,j=GetPlayerCount(); j; i++)
    if (GetPlayerName(i))
      {
      // Noch in der Teamauswahl?
      if (FindObjectOwner(_KSL, i)) return(1);
      --j;
      }
  // Alle OK
  return();
  }


public func GetUnjoinedPlayers()
  {
  // Namensstring aus nicht beigetretenen Spielern zusammensetzen
  var sResult, iNumPlrs, sName;
  for (var i,j=GetPlayerCount(); j; i++)
    if (sName = GetPlayerName(i))
      {
      // Noch in der Teamauswahl?
      if (FindObjectOwner(_KSL, i))
        if (iNumPlrs++)
          sResult = Format("%s, %s", sResult, sName);
        else
          sResult = sName;
      --j;
      }  
  // Fertig, Stirng zurückgeben
  return (sResult);
  }

public func IsSinglePlayerVsTeam(&riSinglePlrKingdom)
  {
  // Gibt es ein Einzelkämpferkönigreich und ein Mehrspielerkönigreich?
  var fSinglePlrKingdom, fMultiPlrKingdom;
  // Alle Spieler auf ihre Königreiche anrechnen (Var())
  for (var i,j=GetPlayerCount(); j; i++)
    if (GetPlayerName(i))
      {
      ++Var(PlayerTeam(i));
      --j;
      }
  // Königreiche überprüfen
  var iTeam=iKingLocals+1;
  while (--iTeam)
    if (Var(iTeam) && StartPosX(iTeam))
      if (Var(iTeam) == 1)
        {
        fSinglePlrKingdom = 1;
        riSinglePlrKingdom = iTeam;
        }
      else
        fMultiPlrKingdom = 1;
  // Bedingung nur erfüllt, wenn beide Arten von Königreichen existieren
  return(fSinglePlrKingdom && fMultiPlrKingdom);
  }

public func GetSinglePlayer()
  {
  var iSingleTeam;
  // Einzelspielerkönigreich vorhanden?
  if (!IsSinglePlayerVsTeam(iSingleTeam)) return(-1);
  // Königsspieler zurückgeben
  return(GetOwner(King(iSingleTeam)));
  }
