/*-- BR --*/

#strict
#include TEAM

protected func Initialize()
{
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

public func GetHUDInfo(int player, object hud) {
	
	var team = GetPlayerTeam(player);
	if(team < 1) return(inherited(player));
	
	var bomb = GetBomb();
	if(bomb) // can be null during initialization
	{
		var bc = GetBombCarrier(bomb);
		if(bc) {
			var bcteam = GetPlayerTeam(GetOwner(bc));
			
			// which side
			if(bcteam == team)
				SetHUDGraphics(0,hud,GetID(),2,2,"BombFriendly");
			else
				SetHUDGraphics(0,hud,GetID(),2,2,"BombEnemy");
		}
		// Bombe an Heimatposition
		else if(bomb->IsAtHome()) {
			SetHUDGraphics(0,hud,GetID(),2,2);
		}
		// ansonsten isse lose
		else {
			SetHUDGraphics(0,hud,GetID(),2,2,"BombGone");
		}
	}
	
	return(inherited(player));
}

public func BombTaken() {
	UpdateHUDs();
}

public func BombReturned() {
	UpdateHUDs();
}

public func BombScored() {
	UpdateHUDs();
}

public func BombLoose() {
	UpdateHUDs();
}

/* Globale Funktionen */

global func CreateBomb(int iX, int iY, int iColor) // Platziert eine Bombe an der angegebenen Position mit der angegebenen Farbe.
{
  var pBomb = CreateObject(GBRB, AbsX(iX), AbsY(iY), -1);
  pBomb->Init(iX,iY,iColor);
}

public func Activate(iCaller)
{
 var desc=GetDesc();
 desc=Format("%s||$WinString$",desc,iWinScore);
 MessageWindow(desc,iCaller);
}

// Kann mittels des Spielzielauswählers konfiguriert werden
public func IsConfigurable() { return(1); }

// Taktik für KI

public func GetBomb()
{
  return(FindObject2(Find_ID(GBRB)));
}

public func GetBombCarrier(object bomb)
{
  return(bomb->Contained());
}

public func GetOwnGoal(int owner)
{
  for(var goal in FindObjects(Find_ID(GBRG)))
    if(goal->GetGoalTeam() == GetPlayerTeam(owner))
      return(goal);
}

private func GetEnemyGoal(int owner)
{
  for(var goal in FindObjects(Find_ID(GBRG)))
    if(goal->GetGoalTeam() != GetPlayerTeam(owner))
      return(goal);
}

// Tactic for bombing run
public func AITactic(object pAIPlayer)
{
  var owner = pAIPlayer->GetOwner();
  var bomb = GetBomb();
  var carrier = GetBombCarrier(bomb);
  var goal = GetOwnGoal(owner);
  var egoal = GetEnemyGoal(owner);

  var crewcount = GetCrewCount(GetOwner());

  for(var i=0, pCrew ; pCrew = GetCrew(owner, i) ; i++) {
    if(pCrew->Contained()) continue;

    // bomb carrier
    if(pCrew == carrier) {
      if(pCrew->GetMacroCommand(0, 1) != egoal);
        // rush to enemy goal
        pAIPlayer->Run(pCrew, egoal);
    }
    else {
      // someone has the bomb
      if(carrier) {
        if(pCrew->GetMacroCommand(0, 1) != carrier) {
          // hostile bomb carrier?
          if(Hostile(owner, carrier->GetOwner())) {
              // Intercept!
              pCrew->SetMacroCommand(this, "Follow", carrier, 0,0,0, Aggro_Follow);
          }
          else {
            // friendly bomb carrier, guard
              pCrew->SetMacroCommand(this, "Follow", carrier, 0,0,0, Aggro_Shoot);
          }
        }
      }
      // No one has the bomb!
      else {
        if(pCrew->GetMacroCommand(0, 1) != bomb);
          // rush to bomb
          pAIPlayer->Run(pCrew, bomb);
      }
    }
  }
	return true;
}
