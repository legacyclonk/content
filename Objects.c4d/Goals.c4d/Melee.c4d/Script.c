/*-- Melee --*/

#strict
#include GOAL

// Teamwork-Melee

// Anfangszahl Spieler
local StartPlayerCount;

protected func InitializePlayer(int iNewPlr, int x, int y, object base, int iTeam)
{
  // NET2-Team: Bei gegebenem Team nicht manuell verfeinden
  if (iTeam) return();
  // Verfeindung: Alle Spieler verfeindet
  var i,j;
  for(var i = 0; i < GetPlayerCount(); ++ i)
  {
    var plr = GetPlayerByIndex(i);
    if(plr == iNewPlr) continue;
    
    SetHostility(iNewPlr, plr, true, true);
    SetHostility(plr, iNewPlr, true, true);
  }
}

private func CheckTeamHostile(int iPlr1, int iPlr2)
{
  // Spieler sind verfeindet, wenn sie in unterschiedlichen Teams sind,
  //  oder in keinen Teams und tatsächlich verfeindet
  var iTeam1 = GetPlayerTeam(iPlr1);
  if (iTeam1 != GetPlayerTeam(iPlr2)) return true;
  if (iTeam1) return false;
  return(Hostile(iPlr1, iPlr2));
}

public func IsFulfilled()
{
  // Anfangszahl Spieler bestimmen
  if(!StartPlayerCount)
  {
    if( (StartPlayerCount = GetPlayerCount()) < 2)
    {
      Log("$MsgTooFewPlayers$");
      return(RemoveObject() );
    }
  }
  
  // Wenn Teams.txt-Teams noch gewählt werden müssen, kann das Ziel nicht erfüllt sein
  if (GetPlayerTeam(GetPlayerByIndex()) == -1) return();

  // Alle Spieler durchgehen
  for(var i = 0; i < GetPlayerCount(); ++ i)
  {
    var plr = GetPlayerByIndex(i);
    // Mit den anderen Spielern vergleichen
    for(var j = i + 1; j < GetPlayerCount(); ++ j)
    {
      var plr2cmp = GetPlayerByIndex(j); 
      // Noch verfeindete Spieler da? Nicht erfüllt.
      if(CheckTeamHostile(plr, plr2cmp) ) return(0);
    }
  }
  
  // Alle verbündet, also erfüllt
  return(1);
}

protected func Activate(iPlr)
{
  // Verfeindete Spieler zählen
  var hostile_count;
  for(var i = 0; i < GetPlayerCount(); ++ i)
  {
    var plr = GetPlayerByIndex(i);
    if(plr == iPlr) continue;
    
    if(Hostile(iPlr, plr) ) hostile_count ++;
  }
  
  // Ausgeben
  if(!hostile_count)
    MessageWindow("$MsgGoalFulfilled$", iPlr);
  else
    MessageWindow(Format("$MsgGoalUnfulfilled$", hostile_count), iPlr);
}

