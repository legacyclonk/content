/*-- Alienjagd --*/

#strict 2
#include GOAL

local aPlayerLives, iWinScore;

public func Initialize()
{
	// Anzahl Leben wird durch Anzahl Aktivierungen eingestellt
	var goal;
	if(goal = FindObject(GetID()))
	{ 
		goal->AddRelaunch();
		RemoveObject(); 
	}
	iWinScore = 1;
	aPlayerLives = CreateArray();
	InitScoreboard();
}

public func AddRelaunch()
{
	iWinScore++;
}

public func WinScoreChange(iNewScore)
{
	iWinScore = iNewScore;
}

public func ChooserFinished()
{
	InitScoreboard();
}

protected func InitializePlayer(int playerNr, int x, int y, object base, int teamId)
{
  if(!aPlayerLives) aPlayerLives = CreateArray();  // Rückwärtskompatibilität
	aPlayerLives[playerNr] = iWinScore-1;
	
	UpdateScoreboard(playerNr);
}

protected func RemovePlayer(int playerNr)
{
	var playerId = GetPlayerID(playerNr);
	SetScoreboardData(playerId, 0, 0,0);
}

protected func InitScoreboard()
{
	SetScoreboardData(SBRD_Caption, SBRD_Caption, "", SBRD_Caption);
	SetScoreboardData(SBRD_Caption, 0, "{{HZCK}}", SBRD_Caption);
}

protected func UpdateScoreboard(int playerNr)
{
	var playerId = GetPlayerID(playerNr);
	SetScoreboardData(playerId, SBRD_Caption, GetTaggedPlayerName(playerNr));
	SetScoreboardData(playerId, 0, Format("%d", aPlayerLives[playerNr]));
}

public func RelaunchPlayer(int playerNr, object clonk, int killerplayerNr)
{
	if(playerNr == -1 || !GetPlayerName(playerNr)) return;
	if(GetPlayerType(playerNr) == C4PT_Script)
		if(GetAlive(clonk))
		{
			aPlayerLives[playerNr] += iWinScore;
			return;
		}

	if(playerNr == -1)
		return;

	aPlayerLives[playerNr]--;
	UpdateScoreboard(playerNr);

	if(aPlayerLives[playerNr] < 0)
	{
		// Verloren
		EliminatePlayer(playerNr);
	}
}

public func IsFulfilled() {
	if(FindObject(CHOS)) return false;
	
	return(!GetLength(AliensLeft()));
}

protected func Activate(player)
{
	return(MessageWindow(GetFulfillText(),player));
}

private func GetFulfillText()
{
	var aAliens = AliensLeft();
	if(!GetLength(aAliens))
		return("$MsgAllDead$");
	var aliens = CreateArray();
	for(var obj in aAliens) {
		if(!FindIn(aliens,GetID(obj)))
			aliens[GetLength(aliens)] = GetID(obj);
	}

	var text = "$MsgStillLeft$";
	for(var ID in aliens)
	{
		text = Format("%s|{{%i}} %s: %d",text,ID,GetName(0,ID),ObjectCount(ID));
	}
	return(text);
}

private func AliensLeft()
{
	return(FindObjects(Find_Func("IsAlien")));
}

private func FindIn(array a, b)
{
	for(var c in a)
		if(c == b)
			return(true);
			
	return(false);
}

public func OnAlienDeath(object alien, int killedBy)
{
	// halber Wert des getöteten Aliens = money
	DoWealth(killedBy, + alien->GetValue() / 2);
}

// Kann mittels des Spielzielauswählers konfiguriert werden
public func IsConfigurable() { return true; }

public func GetMoreScoreText() { return "$MoreRelaunch$"; }

public func GetLessScoreText() { return "$LessRelaunch$"; }