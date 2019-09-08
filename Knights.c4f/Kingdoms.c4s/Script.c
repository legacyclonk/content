/* Königreiche */
// by Sven2, 2003

#strict

/* Szenarieninitialisierung */

protected func Initialize()
  {
  // Sicherstellen, dass das Spielziel da ist
  if (!ObjectCount(_KTK)) CreateObject(_KTK,0,0,-1);
  // Zusätzlich Melee
  if (!ObjectCount(MELE)) CreateObject(MELE);
  // Himmelsparallaxität
  SetSkyParallax(0,20,10);
  // Script starten
  ScriptGo(1);
  // Fertig
  return(1);
  }

/* Spielerinitialisierung */

protected func InitializePlayer(iPlr)
  {
  // Pläne geben
  (CPPL)->SetKnowledge(iPlr);
  (WPPL)->SetKnowledge(iPlr);
  // Kein Thron!
  SetPlrKnowledge(iPlr, THRN, 1);
  // Fertig
  return(1);
  }


/* Spielerrelaunch */

public func RelaunchPlayer(iPlr)
  {
  var pRule = FindObject(_KTK);
  if (!pRule) return();
  if (!pRule->PlayerEliminated(iPlr)) return();
  Log("%s $MsgEliminated$",GetPlayerName(iPlr));
  PlayerMessage(iPlr, "$MsgEliminatedInfo$");
  Sound("Eliminated");
  return(1);
  }


/* Einzelspielercheck */

/*protected func Script50()
  {
  // Spielziel noch da? (man weiß ja nie)
  var pRule = FindObject(_KTK); if (!pRule)  return();
  if (pRule->IsFulfilled()) return(ScriptGo());
  // Nach kurzer Zeit: Prüfen, ob alle Spieler beigetreten sind
  if (pRule->IsUnjoinedPlayer())
    {
    Message("$MsgUnjoinedPlayers$:|%s", 0, pRule->GetUnjoinedPlayers());
    return(goto(40));
    }
  // Spiel 1 vs x?
  var Q;
  if (pRule->IsSinglePlayerVsTeam(Q))
    // Spielstart mit Einzelspieler gegen ein Team
    DoPlrAdvantage(pRule->GetSinglePlayer());
  else
   // Regulärer Spielstart
    Message("$MsgGameGo$");
  // Script starten
  return(1);
  }*/

private func DoPlrAdvantage(iPlr)
  {
  // Einzelspieler kriegen einen kleinen Bonus
  // +300 Clunker
  SetWealth(iPlr, GetWealth(iPlr)+350);
  // Vierfaches Heimatbasismaterial
  var id, i;
  while (id = GetHomebaseMaterial(iPlr, 0, i++))
    DoHomebaseMaterial(iPlr, id, GetHomebaseMaterial(iPlr, id)*3);
  // Nachricht
  Message("$MsgGameGo2$", 0, GetPlayerName(iPlr));
  // Fertig
  return(1);
  }



/* Spielablauf: Mehr Blitze in der Mitte */

protected func Script70()
  {
  // Durchschnittlich einen Zusatzblitz alle 5 Sekunden erzeugen
  // (Ohne Bildschirmerhellung)
  if (!Random(2))
    LaunchLightning(LandscapeWidth()/4 + Random(LandscapeWidth()/2), 0, -20, 41, +5, 15);
  return(goto(60));
  }


/* Königreiche - Ausdehnungen */

public func KingdomExtents(pKing) { return(1); }
public func KingdomExtentsX(pKing) { if (GetX(pKing) < LandscapeWidth()/2) return(0); else return(2130); }
public func KingdomExtentsY(pKing) { return(0); }
public func KingdomExtentsWdt(pKing) { return(470); }
public func KingdomExtentsHgt(pKing) { return(700); }
