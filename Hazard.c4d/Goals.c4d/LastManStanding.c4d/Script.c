/*-- Last Man Standing --*/

#strict
#include TEAM

local aPlayerLives, chooser;

// Kann mittels des Spielzielauswählers konfiguriert werden
public func IsConfigurable() { return(1); }

/* Initialisierungen */

protected func Initialize() {
  // Anzahl Leben wird durch Anzahl Aktivierungen eingestellt
  var pGoal;
  if (pGoal = FindObject(GetID()))
  { 
    LocalN("iWinScore", pGoal)++;
    return(RemoveObject()); 
  }
  iWinScore = 1;
  aPlayerLives = CreateArray();
  if(FindObject(CHOS)) chooser = true;
  return(_inherited());
}

public func GetHUDInfo(int player) {
	
	return(Format("$MsgHUD$",aPlayerLives[player]));

}

protected func InitializePlayer(int iPlr, int iX, int iY, object pBase, int iTeam)
{
  aPlayerLives[iPlr] = iWinScore * GetCrewCount(iPlr) -1;
  inherited(iPlr, iX, iY, pBase, iTeam);
  
  // anzeigen
  if(GetTeamPlayerCount(GetPlayerTeam(iPlr)) > 1)
	  SetScoreboardData(iPlr, TEAM_GoalColumn, Format("%d",aPlayerLives[iPlr]), aPlayerLives[iPlr]);
  else
	  SetScoreboardData(iPlr, TEAM_GoalColumn, Format("<c %x>%d</c>", GetPlrColorDw(iPlr),aPlayerLives[iPlr]), aPlayerLives[iPlr]);
}

public func ChooserFinished()
{
  chooser = false;
}

/* Relaunch */

public func RelaunchPlayer(int iPlr, object pClonk, int iMurdererPlr) {
  if(iPlr == -1 || !GetPlayerName(iPlr)) return();
  if(GetPlayerType(iPlr) == C4PT_Script)
    if(GetAlive(pClonk))
    {
      aPlayerLives[iPlr] += iWinScore;
      return();
    }

  if(iPlr == -1)
    return();

  aPlayerLives[iPlr]--;
  var left = Format("%d", aPlayerLives[iPlr]);

  if(aPlayerLives[iPlr] < 0)
  {
    // Verloren
    EliminatePlayer(iPlr);
    left = "-";
  }
  // richtige anzeigeart wählen
  if(GetTeamPlayerCount(GetPlayerTeam(iPlr)) > 1)
	  SetScoreboardData(iPlr, TEAM_GoalColumn, left, aPlayerLives[iPlr]);
  else
	  SetScoreboardData(iPlr, TEAM_GoalColumn, Format("<c %x>%s</c>", GetPlrColorDw(iPlr),left), aPlayerLives[iPlr]);
  
  inherited(iPlr, pClonk, iMurdererPlr);
}

/* Siegbedingungen */

public func IsFulfilled() // Siegreiches Team?
{
  // Es werden noch Regeln (und KI) gewählt
  if(chooser) return();

  var iTeam;
  for(var i = 0 ; i < GetPlayerCount() ; i++)
  {
    if(!iTeam)
      iTeam = GetPlayerTeam(GetPlayerByIndex(i));
    // Spieler aus einem anderen Team leben noch?
    if(GetPlayerTeam(GetPlayerByIndex(i)) != iTeam)
      return();
  }
  Message("@<c %x>$WinMsg$</c>", 0, GetTeamColor(iTeam), GetTeamName(iTeam));
  return(1);
}

protected func Activate(iPlr)
{
  if (IsFulfilled())
  {
    return(MessageWindow("$MsgGoalFulfilled$",iPlr));
  }
  return(MessageWindow(Format("$MsgGoalUnfulfilled$", aPlayerLives[iPlr]), iPlr));
}

/* Überladungen für das Scoreboard */

private func InitMultiplayerTeam(int iTeam)
{
  inherited(iTeam);
  SetScoreboardData(TEAM_iRow + iTeam, TEAM_GoalColumn, "", iWinScore+1);
  SortTeamScoreboard();
}

private func InitSingleplayerTeam(int iPlr)
{
  inherited(iPlr);

  SetScoreboardData(iPlr, TEAM_GoalColumn, Format("<c %x>%d</c>", GetPlrColorDw(iPlr), aPlayerLives[iPlr]), aPlayerLives[iPlr]);
  SortTeamScoreboard();
}

private func InitPlayer(int iPlr)
{
  inherited(iPlr);
  SetScoreboardData(iPlr, TEAM_GoalColumn, Format("%d", aPlayerLives[iPlr]), aPlayerLives[iPlr]);
  SortTeamScoreboard();
}

private func UpdateScoreboard(int iTeam)
{
  return();
}

private func SortTeamScoreboard()
{
  // Nach Toden (Spieler)
  SortScoreboard(TEAM_DeathColumn);
  // Nach Kills (Spieler)
  SortScoreboard(TEAM_KillColumn, true);
  // Nach Punkten (Spieler & Teams)
  SortScoreboard(TEAM_GoalColumn, true);
  // Nach Teams (Spieler)
  SortScoreboard(TEAM_TeamColumn);
}

// KI Taktik
public func AITactic(object pAIPlayer)
{
  var owner = pAIPlayer->GetOwner();

  for(var i=0, pCrew ; pCrew = GetCrew(owner, i) ; i++) {
    if(pCrew->Contained()) continue;
    pCrew->CheckInventory();
	// nix zu tun?
    if(!(pCrew->GetCommand() || pCrew->GetMacroCommand()))
    {
	  // Leute töten ist nicht so wichtig... such lieber nach guten Waffen
      if(pCrew->CustomContentsCount("IsWeapon") < 2) {
        if(pCrew->SearchWeapon(Aggro_Shoot)) {
		  return(true);
		}
	  }
      // Genug ausgerüstet (oder nix gefunden)... Krieg!!
      pAIPlayer->TacticDefault(pCrew);
    }
  }
  return(true);
}

public func GetMoreScoreText() { return "$MoreRelaunch$"; }

public func GetLessScoreText() { return "$LessRelaunch$"; }