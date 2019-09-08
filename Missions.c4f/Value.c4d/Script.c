/*-- Siedlungspunkte --*/

/* Umgebaut, so dass es mit den Team Siedlungspunkten zusammenarbeitet */

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
  var iPlayers, i, iFactor = 1;
  var fUseTeamAccount = (FindObject2(Find_ID(TACC)) && !GameCall("TACC_BlockTeamExchange", TACC_TypeScore));
  for (iPlayers = GetPlayerCount(); iPlayers; i++)
    if (GetPlayerName(i))
    {
      iPlayers--;
      if(fUseTeamAccount) iFactor = FindObjectOwner(ACNT, i)->GetAlliancePlayerCount(1);
      if (GetPlrValueGain(i) < ApplyFactor(iValue, iFactor)) return(0);
    }
  return(1);
}

private func ApplyFactor(iValue, iFactor)
{
  // Alleine muss der Spieler 100% schaffen, zu zweit 150% zu dritt 175% usw.
  iFactor = 2000 - 1000/(2**(iFactor-1));
  return iValue * iFactor / 1000;
}
    
public func IsFulfilled()
{
  if (!TargetScore) TargetScore = 100;
  return(AllPlayersHaveValue(TargetScore));
}

public func IsFulfilledforPlr(iPlr) 
{
  var iFactor = 1;
  if(FindObject2(Find_ID(TACC)) && !GameCall("TACC_BlockTeamExchange", TACC_TypeScore))
    iFactor = FindObjectOwner(ACNT, iPlr)->GetAlliancePlayerCount(1);
  return (GetPlrValueGain(iPlr) >= ApplyFactor(TargetScore, iFactor));
}

protected func Activate(iPlayer)
{
  var iFactor = 1;
  if(FindObject2(Find_ID(TACC)) && !GameCall("TACC_BlockTeamExchange", TACC_TypeScore))
    iFactor = FindObjectOwner(ACNT, iPlayer)->GetAlliancePlayerCount(1);
  if (IsFulfilled()) 
    return(MessageWindow(Format("$MsgGoalFulfilled$", ApplyFactor(TargetScore, iFactor)), iPlayer));
  MessageWindow(Format("$MsgGoalUnfulfilled$", GetPlrValueGain(iPlayer), ApplyFactor(TargetScore, iFactor)), iPlayer);
  return(1);
}

