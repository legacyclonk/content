/*-- Töten --*/

#strict
#include GOAL

static fWon;

protected func Initialize()
{
  // Warten
  SetAction("Initializing");
  return(_inherited());
}

public func IsFulfilled()
{
  if(GetPlayerCount()<=1) return(1);    
  return(fWon);
}

protected func Activate(iPlayer)
{
  var iPlr, iKills, iBest; 
  for(var i = 0; i < GetPlayerCount(); i++)
  {
    iPlr = GetPlayerByIndex(i);  
    if(Kills(iPlr)>iKills) { iKills = Kills(iPlr); iBest = iPlr+1; }
  }
  var szString = "", iColor = RGB(255, 255,255);
  szString = Format("%s$TxtDesc$", szString, FragLimit);
  if(iBest) szString = Format("%s|$TxtPlr$", szString, GetPlayerName(iBest-1), iKills);
  
  // Bei Aktivierung der Regel: Scoreboard für fünf Sekunden zeigen
  DoScoreboardShow(1, iPlayer+1);
  ScheduleCall(0, "DoScoreboardShow", 37*5, 1, -1, iPlayer+1);
  
  // Nachricht ausgeben
  return(MessageWindow(szString, iPlayer));
}

private func DoScoreboardShow(a, b, c, d, e) { return(_inherited(a, b, c, d, e)); }
