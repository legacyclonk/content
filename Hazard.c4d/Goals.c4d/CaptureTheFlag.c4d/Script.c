/*-- CTF --*/

#strict
#include TEAM

local aFlagCarrier;

protected func Initialize()
{
  //Anzahl Gewinne wird durch Anzahl Aktivierungen eingestellt
  var pGoal;
  if (pGoal = FindObject(GetID()))
  { 
    LocalN("iWinScore", pGoal)++;
    return(RemoveObject()); 
  }
  iWinScore = 1;
  aFlagCarrier = CreateArray();
  return(_inherited());
}

public func GetHUDInfo(int player, object hud) {
	
	var team = GetPlayerTeam(player);
	if(team < 1) return(inherited(player));
	var ourFlag = GetFriendlyFlag(player);
	
	if(ourFlag) // is null during initialization
	{
		// Flag stolen?
		if(ourFlag->GetCarrier()) {
			// stolen
			SetHUDGraphics(0,hud,GetID(),2,2,"FlagStolen");
		}
		else if(ourFlag->IsAtHome())
			SetHUDGraphics(0,hud,GetID(),2,2);
		else
			SetHUDGraphics(0,hud,GetID(),2,2,"FlagLoose");
	}
		
	// flag captured
	if(GetFriendlyFlagCarrier(player)) {
		SetHUDGraphics(0,hud,GetID(),3,2,"FlagCaptured");
	}
	else
		SetHUDGraphics(0,hud,GetID(),3,2);
	
	return(inherited(player));
}

/* Globale Funktionen */

global func CreateFlag(int iTeam, int iX, int iY, int iColor) // Platziert eine Flagge für Team iTeam, Position und Farbe angeben
{
  var pBase = CreateObject(FLBS, AbsX(iX), AbsY(iY), -1);
  var pFlag = CreateObject(FLA2, AbsX(iX), AbsY(iY), -1);
  pFlag->Activate(pBase, iTeam, iColor);
  if(FindObject(GCTF)) FindObject(GCTF)->InitTeam(iTeam);
}

/* Geld */

// Clonks mit Flaggen sind mehr wert
public func Money(int iPlr, object pClonk, int iMurdererPlr)
{
  if(iMurdererPlr != NO_OWNER)
  {
    var extra;
    var flag = FindObject2(Find_ID(FLA2), Find_ActionTarget(pClonk));
    if(flag)
      extra = 30;
    // Teamkill! Wert des Clonks wird abgezogen
    if(GetPlayerTeam(iPlr) == GetPlayerTeam(iMurdererPlr))
      DoWealth(iMurdererPlr, - (pClonk->GetValue(0,0,0, iMurdererPlr) + extra));
    // Regulärer Kill, Wert des Clonks wird vergeben
    else
      DoWealth(iMurdererPlr, + pClonk->GetValue(0,0,0, iMurdererPlr) + extra);
  }
  // Geld für den getöteten, halber Wert des Clonks wird gegeben
  DoWealth(iPlr, + pClonk->GetValue(0,0,0, iPlr) / 2);
}

/* Scoreboard */

public func InitTeam(int iTeam)
{
  aFlagCarrier[iTeam] = -1;
}

public func FlagCaptured(int iTeam, int iCTeam, object pClonk)
{
  aFlagCarrier[iTeam] = GetOwner(pClonk);
  SetFlagMarker(GetOwner(pClonk), iTeam); // pClonk hat die flagge von iTeam
  UpdateHUDs();
}

public func FlagLost(int iTeam)
{
  UpdateHUDs();
  if(aFlagCarrier[iTeam] == -1) return();
  SetFlagMarker(aFlagCarrier[iTeam], 0, true);
  aFlagCarrier[iTeam] = -1;
}

public func FlagScored(int iTeam, int iCTeam, object pClonk)
{
  UpdateHUDs();
  if(aFlagCarrier[iTeam] == -1) return();
  SetFlagMarker(aFlagCarrier[iTeam], 0, true);
  aFlagCarrier[iTeam] = -1;
}

public func FlagReturned(int iTeam)
{
  SetFlagMarker(aFlagCarrier[iTeam], 0, true);
  aFlagCarrier[iTeam] = -1;
  UpdateHUDs();
}

private func SetFlagMarker(int iPlr, int iCTeam, bool fRemove)
{
  var iCol = TEAM_PlayerColumn, iData;
//  var iPlr = GetOwner(pPlr);
  if(GetTeamPlayerCount(GetPlayerTeam(iPlr)) == 1)
  {
    iCol = TEAM_TeamColumn;
    iData = GetPlayerTeam(iPlr);
  }
  if(!fRemove)
    SetScoreboardData(iPlr, iCol, Format("<c %x>{{FLBS}}</c> %s", GetTeamColor(iCTeam), GetTaggedPlayerName(iPlr)), iData);
  else
    SetScoreboardData(iPlr, iCol, GetTaggedPlayerName(iPlr), iData);
}

// Kann mittels des Spielzielauswählers konfiguriert werden
public func IsConfigurable() { return(1); }


// Returns the enemy flag carrier if any
public func GetEnemyFlagCarrier(int iOwner) {
	var ourFlag = GetFriendlyFlag(iOwner);
	return(ourFlag -> GetCarrier());
}

// Returns the friendly flag carrier if any. 
public func GetFriendlyFlagCarrier(int iOwner) {
  // The flag...
  for(var pFlag in FindObjects(Find_ID(FLA2))) {
    // ... that is attached to a clonk...
    if(WildcardMatch(pFlag->GetAction(), "*Attach*")) {
      var flagger = pFlag->GetActionTarget(); 
      var flagger_team = GetPlayerTeam(flagger->GetOwner());
      // ... which is in our team ...
      if( flagger_team == GetPlayerTeam(iOwner) ) {
        // ...his flag
        return(flagger);
      }
    }
  }
  return(0);
}

public func GetFriendlyFlag(int iOwner) {
  // The flag...
  for(var pFlag in FindObjects(Find_ID(FLA2))) {
    // that is not ours
    if( pFlag->GetTeam() == GetPlayerTeam(iOwner) ) {
      return(pFlag);
    }
  }
  return(0);
}

// Returns the flag of a random enemy
public func GetEnemyFlag(int iOwner) {
  var flags = CreateArray();
  // A flag...
  for(var pFlag in FindObjects(Find_ID(FLA2))) {
    // that is not ours
    if( pFlag->GetTeam() != GetPlayerTeam(iOwner) ) {
      flags[GetLength(flags)] = pFlag;
    }
  }
  // Choose a flag randomly
  return(flags[Random(GetLength(flags))]);
}

// Returns the own flag base
public func GetOwnFlagBase(int iOwner) {
  // The flag...
  for(var pFlag in FindObjects(Find_ID(FLA2))) {
    // ... of our team ...
    if( pFlag->GetTeam() == GetPlayerTeam(iOwner) ) {
      // ...its base
      return(pFlag->GetBase());
    }
  }
  return(0);
}

// Some constants
private func MaxFlagBaseDistance()  { return(250); }

// KI Taktik
public func AITactic(object pAIPlayer)
{
  var owner = pAIPlayer->GetOwner();
  
  var pEFC = GetEnemyFlagCarrier(owner);
  var flag = GetFriendlyFlag(owner);
  if(!pEFC)
	if(!(flag->IsAtHome()))
		pEFC = flag;
  var pFFC = GetFriendlyFlagCarrier(owner);
  var flagbase = GetOwnFlagBase(owner);
  var eflagbase = GetEnemyFlag(owner);

  if(pEFC) DebugLog("Flag was stolen by %s (%d,%d)","tactics",GetName(pEFC),GetX(pEFC),GetY(pEFC));
  if(pFFC) DebugLog("%s (%d,%d) got the enemy flag","tactics",GetName(pFFC),GetX(pFFC),GetY(pFFC));

  var crewcount = GetCrewCount(owner);

  for(var i=0, pCrew ; pCrew = GetCrew(owner, i) ; i++) {
    if(pCrew->Contained()) continue;
    pCrew->CheckInventory();

    // flag carrier returns home
    if(pCrew == pFFC) {
      
      // return to own flag if own flag is not stolen or he is too far from the flag base
      if(!pEFC || ObjectDistance(pCrew,flagbase) > MaxFlagBaseDistance()) {
        if(GetCrewCount(pCrew->GetOwner())>1 || !pEFC) {
          pAIPlayer->Run(pCrew,flagbase,"Returning home!");
          continue;
        }
        else if(pEFC) {
          pAIPlayer->Follow(pCrew,pEFC,0,"Attacking enemy flag carrier");
          continue;
        }
      }
      // own flag is stolen
      else {
          // I'm not the only one
          if(GetCrewCount(pCrew->GetOwner())>1) {
            pAIPlayer->Guard(pCrew,"Guarding our flag base!");
            continue;
          }
          // however, if I'm the only one, I need to perhaps follow the enemy flagger...
          else if(pEFC) {
            pAIPlayer->Follow(pCrew,pEFC,0,"Attacking enemy flag carrier");
            continue;
        }
      }
    }
    else {
      var intercept = 0;
      // flag was stolen. Everyone who can intercept, does that
      if(pEFC) {
        intercept = pAIPlayer->Interceptor(pCrew,eflagbase,pEFC);
         if(intercept) {
          pAIPlayer->Follow(pCrew,pEFC,0,"Intercepting enemy flag carrier!");
          continue;
        }
      }
      if(!intercept) {
        // friendly flag carrier around
        if(pFFC && !pEFC) {
          var follower = pAIPlayer->Interceptor(pCrew,flagbase,pFFC);
          // between flagger and own flag: follow flagger
          if(follower) {
            pAIPlayer->Follow(pCrew,pFFC,0,"Escorting friendly flag carrier!");
            continue;
          }
        }
        // else just go to enemy flag/enemy flag carrier
        if(!pEFC) {
          if(!(pCrew->GetCommand() || pCrew->GetMacroCommand())) {
            pCrew->SetMacroCommand(this(), "MoveTo",  eflagbase,0,0,0,Aggro_Follow);
            DebugMessage("@Going to enemy flag base.", "tactics");
          }
        }
        else {
          pAIPlayer->Follow(pCrew,pEFC,0,"Hunting down enemy flag carrier!");
        }
    
      }
    }
  }
	return true;
}
