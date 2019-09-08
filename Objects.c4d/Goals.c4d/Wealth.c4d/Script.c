/*-- Geldpunkte --*/

#strict
#include GOAL

// Punktzahl
local TargetWealth;

protected func Initialize()
{
  // Anderes Objekt vorhanden? Selber dazuzählen
  var pGoal;
  if (pGoal = FindObject(GetID()))
  { 
    LocalN("TargetWealth", pGoal) += 100;
    return(RemoveObject()); 
  }
  // Punktzahl setzen
  TargetWealth = 100;
  return(inherited());
}
    
private func AllPlayersHaveValue(iValue)
{
  var iPlayers,i;
  for (iPlayers = GetPlayerCount(); iPlayers; i++)
    if (GetPlayerName(i))
    {
      iPlayers--;
      if (GetWealth(i) < iValue) return(0);
    }
  return(1);
}
    
public func IsFulfilled()
{
  if (!TargetWealth) TargetWealth = 100;
  return(AllPlayersHaveValue(TargetWealth));
}
    
public func IsFulfilledforPlr(iPlr) 
{
  return (GetWealth(iPlr) >= TargetWealth);
}

protected func Activate(iPlayer)
{
  if (IsFulfilled()) 
    return(MessageWindow(Format("$MsgGoalFulfilled$", TargetWealth), iPlayer));
  MessageWindow(Format("$MsgGoalUnfulfilled$", GetWealth(iPlayer), TargetWealth), iPlayer);
  return(1);
}
