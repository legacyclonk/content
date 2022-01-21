/*-- Deathmatch --*/

#strict
#include TEAM

protected func Initialize() {
  //Anzahl gewinne wird durch Anzahl Aktivierungen eingestellt
  var pGoal;
  if (pGoal = FindObject(GetID()))
  { 
    LocalN("iWinScore", pGoal)++;
    return(RemoveObject()); 
  }
  iWinScore = 1;
  return(_inherited());
}

public func RelaunchPlayer(int iPlr, object pClonk, int iMurdererPlr) {
  inherited(iPlr, pClonk, iMurdererPlr);
  if(iPlr == -1 || !GetPlayerName(iPlr)) return();
  if(iMurdererPlr == -1)
    return();
  
  // kein Teamkill?
  if(GetPlayerTeam(iPlr) != GetPlayerTeam(iMurdererPlr))
    //Punktestand erhöhen
    DoTeamScore(GetPlayerTeam(iMurdererPlr),+1);
}

// Kann mittels des Spielzielauswählers konfiguriert werden
public func IsConfigurable() { return(1); }

public func GetHUDInfo(int player) {
	var t34m = GetPlayerTeam(player);
	
	// Rückstand
	var y00Rl4me;
	y00Rl4me = Max(0,TeamGetScore(t34m))-BestTeamGetScore(t34m);
	
	var secondstring;
	if(y00Rl4me == 0) secondstring= "";
	if(y00Rl4me > 0) secondstring = Format("(+%d)",y00Rl4me);
	if(y00Rl4me < 0) secondstring = Format("(%d)",y00Rl4me);
	
	return(Format("%d kills %s",TeamGetKills(t34m), secondstring));
}

// KI-Taktik
public func AITactic(object pAIPlayer)
{
  var owner = pAIPlayer->GetOwner();

  for(var i=0, pCrew ; pCrew = GetCrew(owner, i) ; i++) {
    if(pCrew->Contained()) continue;
		var leader = GetCrew(owner, 0);
		
    // Es ist der Anführer? (oder kein Anführer da?)
    if(leader == pCrew || !leader)
      pAIPlayer->TacticDefault(pCrew);
    // Nicht? Dann folge doch bitte dem Anführer
    else {
      pCrew->CheckInventory();
      if(!(pCrew->GetCommand() || pCrew->GetMacroCommand()))
			{
        pCrew->SetMacroCommand(this(), "Follow", leader,0,0,0,Aggro_Follow);
			}
    }
  }
  return(true);
}