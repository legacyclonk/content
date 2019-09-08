/*-- KillTheCaptain --*/

#strict

private func Execute()
{
  // Alle Spieler überprüfen
  var iPlrCnt, iPlr = 0;
  for (iPlrCnt = GetPlayerCount(); iPlrCnt; iPlr++)
    if (GetPlayerName(iPlr))
      {
      iPlrCnt--;
      if (GetPlayerTeam(iPlr) != -1) if (!GetCaptain(iPlr)) 
        EliminatePlayer(iPlr);  
      }
}

protected func Activate(iByPlayer)
{
  MessageWindow(GetDesc(), iByPlayer);
  return(1);
}