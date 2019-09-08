/*-- Siedlungspunkte --*/

#strict
#include GOAL

// Punktzahl
local TargetScore;

protected func Initialize()
{
  // Anderes Objekt vorhanden? Selber dazuzählen
  var pGoal;
  if (pGoal = FindObject(GetID()))
  { 
    LocalN("TargetScore", pGoal) += 100;
    return(RemoveObject()); 
  }
  // Punktzahl setzen
  TargetScore = 100;
  return(inherited());
}
    
private func AllPlayersHaveValue(iValue)
{
  var iPlayers,i;
  for (iPlayers = GetPlayerCount(); iPlayers; i++)
    if (GetPlayerName(i))
    {
      iPlayers--;
      if (GetPlrValueGain(i) < iValue) return(0);
    }
  return(1);
}
    
public func IsFulfilled()
{
  if (!TargetScore) TargetScore = 100;
  return(AllPlayersHaveValue(TargetScore));
}

public func IsFulfilledforPlr(iPlr) 
{
  return (GetPlrValueGain(iPlr) >= TargetScore);
}

protected func Activate(iPlayer)
{
  if (IsFulfilled()) 
    return(MessageWindow(Format("$MsgGoalFulfilled$", TargetScore), iPlayer));
  MessageWindow(Format("$MsgGoalUnfulfilled$", GetPlrValueGain(iPlayer), TargetScore), iPlayer);
  return(1);
}

