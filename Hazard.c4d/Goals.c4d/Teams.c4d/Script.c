/*-- Teams --*/

#strict
#include GOAL

static const TEAM_TeamColumn = 0;
static const TEAM_PlayerColumn = 1;
static const TEAM_GoalColumn = 2;
static const TEAM_KillColumn = 3;
static const TEAM_DeathColumn = 4;
// 1024 Spieler werden maximal für das Scoreboard unterstützt!
static const TEAM_iRow = 1024;

// Rückwärtskompatibilität
static const TEAM_GOAL_COLUMN = 2;

local init, fNoTeamCheck;
local evaluation;

protected func Activate(iPlr) // Defaultnachrichten
{
  if (IsFulfilled())
  {
    if(aPoints[GetPlayerTeam(iPlr)] >= iWinScore)
      return(MessageWindow("$MsgGoalFulfilled$",iPlr));
    else
      return(MessageWindow("$MsgGoalLost$",iPlr));
  }
  return(MessageWindow(Format("$MsgGoalUnfulfilled$", GetWinScore() - aPoints[GetPlayerTeam(iPlr)]),iPlr));
}

// Variablen zum Speichern des Punktestandes

local aPoints;

// Siegpunktzahl

local iWinScore;

// Spielerdinge

local aKill, aDeath;

protected func Initialize()
{
  aPoints = CreateArray();
  aKill = CreateArray();
  aDeath = CreateArray();
  ScheduleCall(this(),"InitScoreboard",1);
  return(_inherited());
}

protected func InitializePlayer(int iPlr, int iX, int iY, object pBase, int iTeam)
{
  if(!init) return();

  if(!aPoints[iTeam])  aPoints[iTeam] = 0;

//  if(iPlr > -1)
//    DoScoreboardShow(true, iPlr);

  //Verfeindung setzen
  Hostility(iPlr);
  // Ins Scoreboard eintragen

  var teamp = GetTeamPlayerCount(iTeam);
  // Team ist neu
  if(teamp == 1)
    InitSingleplayerTeam(iPlr);
  // Team wechselt von Einzelspieler auf Mehrspieler
  if(teamp == 2)
  {
    InitMultiplayerTeam(iTeam);
    RemoveSingleplayerTeam(iPlr);
    var j = 0;
    while(GetTeamPlayer(iTeam, ++j) != -1)
    {
      InitPlayer(GetTeamPlayer(iTeam, j));
    }
  }
  // Spieler normal einfügen
  if(teamp > 2)
    InitPlayer(iPlr);
  // Sortieren
  SortTeamScoreboard();
	
	UpdateHUD(iPlr);
}

/* HUD-Funktionen */

private func GetGoalHUD(player) {
  if(player <= -1) return;
  
  var bar = FindObjectOwner(3HUD,player);
  if(!bar)
  {
    // init the base hud object
    bar = CreateObject(3HUD, 0,0, player);
		bar->SetPosition(60,140);
    bar->SetGraphics(0, 0, GetID(), 1,4);
    bar->SetObjDrawTransform(1500,0,0,0,1500,-120000,0,1);
    bar->SetVisibility(VIS_None);
  }
  return bar;
}

private func UpdateHUDs() {
	var player;
	for(var i=0; i<GetPlayerCount(); ++i) {
		player = GetPlayerByIndex(i);
		if(player >= 0)
			UpdateHUD(player);
	}
}

private func UpdateHUD(int player) {
	var hud = GetGoalHUD(player);
	if(!hud) return;
	
	// display message under the hud
	// if GetHUDInfo return "" or 0, the icon is made invisible
	// to circumvent this even though you want to display no text,
	// just return a whitespace.
	var g = GetHUDInfo(player, hud);
	if(g && !FindObject(CHOS)) {
		hud->SetVisibility(VIS_Owner);
		Message("@%s", hud,g);
		
		// now, display the icons...
	}
	else {
	    hud->SetVisibility(VIS_None);
	}
}

private func SetHUDGraphics(string szGfxName, object pObj, id idSrcDef, int iOverlay, int iOverlayMode, string szAction, int dwBlitMode, object pOvelayObject) {
	SetGraphics(szGfxName,pObj,idSrcDef,iOverlay,iOverlayMode,szAction,dwBlitMode,pOvelayObject);
	pObj->SetObjDrawTransform(1000,0,35000+(iOverlay-2)*28000,0,1000,-66000,0,iOverlay);
}

public func GetHUDInfo(int player, object hud) {
	var team = GetPlayerTeam(player);
	
	//eigenes Team immer vorne
	if(team && team > 0)
		var sting = Format("<c %x>%d</c>",GetTeamColor(team),TeamGetScore(team));
	
	var anotherteam;
	for(var i=0; i<GetTeamCount(); ++i) {
		anotherteam = GetTeamByIndex(i);
		if(anotherteam != team)
			sting = Format("%s : <c %x>%d</c>",sting,GetTeamColor(anotherteam),TeamGetScore(anotherteam));
	}
	
	return(sting);
}

public func ChooserFinished()
{
  ScheduleCall(this(),"InitScoreboard",1);
}

//Die Initialisierung des Scoreboards. Damit alle Spalten/Zeilen etc. angezeigt werden
private func InitScoreboard()
{
  UpdateHUDs();
  //bool SetScoreboardData (int iRowID, int iColID, string sText, int iData); 

  //Überschriften
  SetScoreboardData(SBRD_Caption, SBRD_Caption, Format("%d $Score$",iWinScore), SBRD_Caption);
  SetScoreboardData(SBRD_Caption, TEAM_TeamColumn, "{{KILC}}", SBRD_Caption);
  SetScoreboardData(SBRD_Caption, TEAM_PlayerColumn, "{{HZCK}}", SBRD_Caption);
  SetScoreboardData(SBRD_Caption, TEAM_GoalColumn, Format("{{%i}}",GetID()), SBRD_Caption);
  SetScoreboardData(SBRD_Caption, TEAM_KillColumn, "{{PIWP}}", SBRD_Caption);
  SetScoreboardData(SBRD_Caption, TEAM_DeathColumn, "{{KAMB}}", SBRD_Caption);

  var teams = CreateArray(), team;

  //Daten
  //erstmal rausfinden welche Teams wir anzeigen
  for(var i = 0 ; i < GetPlayerCount() ; i++)
  {
    if(!GetPlayerName(GetPlayerByIndex(i)))
	    continue;

	  var done=0, plrteam = GetPlayerTeam(GetPlayerByIndex(i));
	  if(plrteam < 1) continue;

	  for(var team in teams)
	    if(team == plrteam)
	      done++;
	  if(!done)
	  {
	    teams[GetLength(teams)] = plrteam;
	  }
  }

  //so, jetzt wissen wir die Teams. Jetzt noch nach Teamnummern ordnen. %(
  //Bubblesort!
  var t1, t2;
  for(t1 = 0; t1 < GetLength(teams); t1++) {
    for(t2 = t1; t2 < GetLength(teams); t2++)
  	  if(teams[t2] < teams[t1])
      {
	      var tmp = teams[t1];
    		teams[t1] = teams[t2];
		    teams[t2] = tmp;
      }
  }
  //puh, ganz schön viel Aufwand für sowas..
  //aber jetzt müssen wir nur noch darstellen. :)
  for(var x=0; x < GetLength(teams); x++)
  {
    var t = teams[x];
    // Team hat keinen Spieler -> Höh?
    if(!GetTeamPlayerCount(t)) continue;
    // Team hat nur einen Spieler -> "Einzelspielerteam"
    if(GetTeamPlayerCount(t) == 1)
    {
      InitSingleplayerTeam(GetTeamPlayer(t, 1));
      continue;
    }
    // Team einfügen, 1024 Spieler werden maximal unterstützt!
    InitMultiplayerTeam(t);
    // Spieler aus dem Team einfügen
    var j = 0;
    while(GetTeamPlayer(t, ++j) != -1)
    {
      InitPlayer(GetTeamPlayer(t, j));
    }
  }
  // Sortieren
  SortTeamScoreboard();
  // Fertig
  init = true;
}

/* Scoreboard */

// Funktionen zum Eintragen

// Team eintragen, dass mehrere Spieler hat
private func InitMultiplayerTeam(int iTeam)
{
  if(iTeam < 1) return();

  var col = GetTeamColor(iTeam);

  SetScoreboardData(TEAM_iRow + iTeam, SBRD_Caption, "");
  SetScoreboardData(TEAM_iRow + iTeam, TEAM_TeamColumn,  Format("<c %x>%s</c>", col, GetTeamName(iTeam)), iTeam);
  SetScoreboardData(TEAM_iRow + iTeam, TEAM_PlayerColumn, "");
  SetScoreboardData(TEAM_iRow + iTeam, TEAM_GoalColumn,  Format("<c %x>%d</c>", col, aPoints[iTeam]), aPoints[iTeam]);
  SetScoreboardData(TEAM_iRow + iTeam, TEAM_KillColumn,  Format("<c %x>%d</c>", col, TeamGetKills(iTeam)), TeamGetKills(iTeam)+1);
  SetScoreboardData(TEAM_iRow + iTeam, TEAM_DeathColumn, Format("<c %x>%d</c>", col, TeamGetDeath(iTeam)));
  SortTeamScoreboard();
}

// Mehrspielerteam austragen
private func RemoveMultiplayerTeam(int iTeam)
{
  if(iTeam < 1) return();

  SetScoreboardData(TEAM_iRow + iTeam, TEAM_TeamColumn);
  SetScoreboardData(TEAM_iRow + iTeam, TEAM_PlayerColumn);
  SetScoreboardData(TEAM_iRow + iTeam, TEAM_GoalColumn);
  SetScoreboardData(TEAM_iRow + iTeam, TEAM_KillColumn);
  SetScoreboardData(TEAM_iRow + iTeam, TEAM_DeathColumn);
  SortTeamScoreboard();
}

// Team eintragen, dass nur einen Spieler hat
private func InitSingleplayerTeam(int iPlr)
{
  if(iPlr == -1) return();

  var col = GetPlrColorDw(iPlr);

  SetScoreboardData(iPlr, SBRD_Caption, "");
  SetScoreboardData(iPlr, TEAM_TeamColumn, GetTaggedPlayerName(iPlr), GetPlayerTeam(iPlr));
  SetScoreboardData(iPlr, TEAM_PlayerColumn, "");
  SetScoreboardData(iPlr, TEAM_GoalColumn,  Format("<c %x>%d</c>", col, aPoints[GetPlayerTeam(iPlr)]), aPoints[GetPlayerTeam(iPlr)]);
  SetScoreboardData(iPlr, TEAM_KillColumn,  Format("<c %x>%d</c>", col, TeamGetKills(GetPlayerTeam(iPlr))), TeamGetKills(GetPlayerTeam(iPlr))+1);
  SetScoreboardData(iPlr, TEAM_DeathColumn, Format("<c %x>%d</c>", col, aDeath[iPlr]), aDeath[iPlr]);
  SortTeamScoreboard();
}

// Einzelspielerteam austragen
private func RemoveSingleplayerTeam(int iPlr)
{
  if(iPlr == -1) return();

  SetScoreboardData(iPlr, TEAM_TeamColumn);
  SetScoreboardData(iPlr, TEAM_PlayerColumn);
  SetScoreboardData(iPlr, TEAM_GoalColumn);
  SetScoreboardData(iPlr, TEAM_KillColumn);
  SetScoreboardData(iPlr, TEAM_DeathColumn);
  SortTeamScoreboard();
}

// Spieler eintragen
private func InitPlayer(int iPlr)
{
  // Team 0 oder -1 können wir nicht behandeln
  if(GetPlayerTeam(iPlr) < 1) return();
  if(iPlr == -1) return();
  
  UpdateHUDs();
  
  // Eintragen
  SetScoreboardData(iPlr, SBRD_Caption, "");
  SetScoreboardData(iPlr, TEAM_TeamColumn, Format("<c %x>\\</c>",GetTeamColor(GetPlayerTeam(iPlr))), GetPlayerTeam(iPlr));
  SetScoreboardData(iPlr, TEAM_PlayerColumn, GetTaggedPlayerName(iPlr));
  SetScoreboardData(iPlr, TEAM_GoalColumn, "", aPoints[GetPlayerTeam(iPlr)]);
  SetScoreboardData(iPlr, TEAM_KillColumn, Format("%d", aKill[iPlr]), aKill[iPlr]);
  SetScoreboardData(iPlr, TEAM_DeathColumn, Format("%d", aDeath[iPlr]), aDeath[iPlr]);
  SortTeamScoreboard();
}

// Spieler austragen
private func RemoveScoreboardPlayer(int iPlr)
{
  if(iPlr == -1) return();
  
  if(GetTeamPlayerCount(GetPlayerTeam(iPlr)) == 1) return(RemoveSingleplayerTeam(iPlr));

  SetScoreboardData(iPlr, TEAM_TeamColumn);
  SetScoreboardData(iPlr, TEAM_PlayerColumn);
  SetScoreboardData(iPlr, TEAM_GoalColumn);
  SetScoreboardData(iPlr, TEAM_KillColumn);
  SetScoreboardData(iPlr, TEAM_DeathColumn);

  SortTeamScoreboard();

  // Team nicht prüfen
  if(fNoTeamCheck) return();
  fNoTeamCheck = true;

  // Team wechselt zum Einzelspielerteam
  if(GetTeamPlayerCount(GetPlayerTeam(iPlr)) == 2) // 2, weil der Spieler in diesem Moment noch im Spiel ist
  {
    var j = 0, p;
    while(GetTeamPlayer(GetPlayerTeam(iPlr), ++j) != -1)
    {
      p = GetTeamPlayer(iPlr, j);
      if(p == iPlr) continue;
      //Log("%d", p);
//      RemovePlayer(p, true);
      RemoveMultiplayerTeam(GetPlayerTeam(iPlr));
      InitSingleplayerTeam(p);
    }
  }
  fNoTeamCheck = false;
}

// Engine-Call
public func RemovePlayer(int iPlr)
{
  if(iPlr == -1) return();

	// Auswertungsdialog
	DoEvaluation(iPlr);

  UpdateHUDs();
  RemoveScoreboardPlayer(iPlr);
  aDeath[iPlr] = 0;
  aKill[iPlr] = 0;
}

// Sortieren
private func SortTeamScoreboard()
{
  // Nach Toden (Spieler)
  SortScoreboard(TEAM_DeathColumn);
  // Nach Kills (Spieler)
  SortScoreboard(TEAM_KillColumn, true);
  // Nach Teams (Spieler)
  SortScoreboard(TEAM_TeamColumn);
  // Nach Punkten (Teams)
  SortScoreboard(TEAM_GoalColumn, true);
}

/* Helferlein */

// Schaut, wie viele Spieler in einem Team sind
private func GetTeamPlayerCount(int iTeam)
{
  for(var i = GetPlayerCount()-1,j ; i > -1 ; i--)
    if(GetPlayerTeam(GetPlayerByIndex(i)) == iTeam)
      j++;
  return(j);
}

// Liefert nacheinander die einzelnen Spieler eines Teams (Parameter 1 - x)
private func GetTeamPlayer(int iTeam, int iNo)
{
  for(var i = GetPlayerCount()-1,j ; i > -1 ; i--)
    if(GetPlayerTeam(GetPlayerByIndex(i)) == iTeam)
      if(!--iNo)
        return(GetPlayerByIndex(i));
  return(-1);

}

// Liefert alle Kills eines Teams
private func TeamGetKills(int iTeam)
{
  for(var i = GetPlayerCount()-1,j ; i > -1 ; i--)
    if(GetPlayerTeam(GetPlayerByIndex(i)) == iTeam)
      j += aKill[GetPlayerByIndex(i)];
  return(j);
}

private func TeamGetScore(int iTeam)
{
  return(aPoints[iTeam]);
}

private func BestTeamGetScore(int iExclude)
{
  var greatest = 0;
  for(var i = 0; i<GetLength(aPoints) ;++i) {
    if(i != iExclude)
      greatest = Max(aPoints[i],greatest);
  }
  return greatest;
}

// Liefert alle Death eines Teams
// *copy'n'paste + -Kill +Death!
private func TeamGetDeath(int iTeam)
{
  for(var i = GetPlayerCount()-1,j ; i > -1 ; i--)
    if(GetPlayerTeam(GetPlayerByIndex(i)) == iTeam)
      j += aDeath[GetPlayerByIndex(i)];
  return(j);
}

//aktualisiert den Punktestand
private func UpdateScoreboard(int iTeam)
{
  UpdateHUDs();

  if(GetTeamPlayerCount(iTeam) > 1)
  {
    SetScoreboardData(TEAM_iRow + iTeam, TEAM_GoalColumn, Format("<c %x>%d</c>", GetTeamColor(iTeam), aPoints[iTeam]), aPoints[iTeam]);
    var p, j=0;
    while(GetTeamPlayer(iTeam, ++j) != -1)
    {
      p = GetTeamPlayer(iTeam, j);
      SetScoreboardData(p, TEAM_GoalColumn, "", aPoints[iTeam]);
    }
  }
  else
    SetScoreboardData(GetTeamPlayer(iTeam, 1), TEAM_GoalColumn, Format("<c %x>%d</c>", GetPlrColorDw(GetTeamPlayer(iTeam, 1)), aPoints[iTeam]), aPoints[iTeam]);

  SortTeamScoreboard();
}

private func Hostility(int iPlr)
{
  for(var x; x < GetPlayerCount(); x++)
    if(GetPlayerByIndex(x) && iPlr != x) {
      if(GetPlayerTeam(iPlr) != GetPlayerTeam(GetPlayerByIndex(x)))
        SetHostility(iPlr, GetPlayerByIndex(x), 1, 1);
      else
        SetHostility(iPlr, GetPlayerByIndex(x), 0, 1);
    }
}

public func Destruction()
{
  //Scoreboard löschen
  for(var x=1; GetTeamName(x); x++) {
    var iColID = 1;
    SetScoreboardData(x,       SBRD_Caption, 0);
    SetScoreboardData(x,       iColID,       0);
  }
}

/* Aufrufe */

public func WinScoreChange(int iNewScore)
{
  iWinScore = iNewScore;
  UpdateHUDs();
  SetScoreboardData(SBRD_Caption, SBRD_Caption, Format("%d $Score$",iWinScore), SBRD_Caption);
}

public func ScoreChange(int iTeam, int iChange)
{
  aPoints[iTeam] += iChange;
  UpdateScoreboard(iTeam);
  UpdateHUDs();
  CheckTime();
}

public func IsFulfilled() // Siegreiches Team?
{
	// leben noch menschliche spieler?
	if(!GetPlayerCount(C4PT_User)) {
		if(IsNetwork()) // nein - wenn wir im Netzwerk sind beenden.
		{
            EvaluationForAll();
			return(1);
		}
	}

  var team;
  for(var iScore in aPoints) {
    if(iScore >= iWinScore)
    {
      EliminateLosers();
	  
      Message("@<c %x>$WinMsg$</c>",0,GetTeamColor(team),GetTeamName(team));
	  
      //Leben noch Verlierer? -> Ligainkompatibilität!
      if(LosersAlive())
        return(0);
      
      EvaluationForAll();
      return(1);
    }
    team++;
 }
  // Nur noch ein Team und das hat schon Punkte? -> Sieg (Verlierer sind rausgegangen)
  team = OneTeam();
  if(team != -1)
    if(aPoints[team] > 0)
    {
      Message("@<c %x>$WinMsg$</c>",0,GetTeamColor(team),GetTeamName(team));

      EvaluationForAll();
      return(1);
    }
}

private func EvaluationForAll() {
	// Auswertungsdialog
	if(!evaluation) {
		for(var i=0 ; i < GetPlayerCount() ; i++)
		{
	    	var plr = GetPlayerByIndex(i);
			DoEvaluation(plr);
		}
		evaluation = true;
	}
}

func DoEvaluation(int plr)
{
	AddEvaluationData(Format("{{PIWP}}$Kills$: %d", aKill[plr]), GetPlayerID(plr));
	AddEvaluationData(Format("{{KAMB}}$Death$: %d", aDeath[plr]), GetPlayerID(plr));
}

private func OneTeam() // Nur noch ein einziges Team im Spiel?
{
  var plr, team;
  for(var i = 0 ; i < GetPlayerCount() ; i++)
  {
    plr = GetPlayerByIndex(i);
    if(!team) team = GetPlayerTeam(plr);
    else if(GetPlayerTeam(plr) != team)
      return(-1);
  }
  return(team);
}

private func EliminateLosers()  // Hier trennt sich die Spreu vom Weizen
{
  for(var i=0 ; i < GetPlayerCount() ; i++)
    if(aPoints[GetPlayerTeam(GetPlayerByIndex(i))] < iWinScore)
      EliminatePlayer(GetPlayerByIndex(i)); // HARHAR
}

private func LosersAlive()
{
  for(var x = 0 ; x < GetPlayerCount() ; x++)
    if(aPoints[GetPlayerTeam(GetPlayerByIndex(x))] < iWinScore)
      if(GetPlayerByIndex(x) >= 0)
        return(1);

  return(0);
}

public func RelaunchPlayer(int iPlr, object pClonk, int iMurdererPlr)
{
  if(iPlr == -1 || !GetPlayerName(iPlr)) return();

  UpdateHUDs();
  
  //Tode++
  aDeath[iPlr]++;
  if(GetTeamPlayerCount(GetPlayerTeam(iPlr)) > 1)
    SetScoreboardData(iPlr, TEAM_DeathColumn, Format("%d", aDeath[iPlr]), aDeath[iPlr]);
  else
    SetScoreboardData(iPlr, TEAM_DeathColumn, Format("<c %x>%d</c>", GetPlrColorDw(iPlr), aDeath[iPlr]), aDeath[iPlr]);
  if(GetTeamPlayerCount(GetPlayerTeam(iPlr)) > 1)
    SetScoreboardData(TEAM_iRow + GetPlayerTeam(iPlr), TEAM_DeathColumn, Format("<c %x>%d</c>", GetTeamColor(GetPlayerTeam(iPlr)), TeamGetDeath(GetPlayerTeam(iPlr))), TeamGetDeath(GetPlayerTeam(iPlr))+1);
  //kein Selfkill? kein Teamkill?
  if(iMurdererPlr != -1 && GetPlayerTeam(iPlr) != GetPlayerTeam(iMurdererPlr))
  {	
    //kill++
    aKill[iMurdererPlr]++;
    SetScoreboardData(iMurdererPlr, TEAM_KillColumn, Format("<c %x>%d</c>", GetPlrColorDw(iMurdererPlr), aKill[iMurdererPlr]), aKill[iMurdererPlr]);
    //team: kill++
    if(GetTeamPlayerCount(GetPlayerTeam(iMurdererPlr)) > 1)
    {
      SetScoreboardData(TEAM_iRow + GetPlayerTeam(iMurdererPlr), TEAM_KillColumn, Format("<c %x>%d</c>", GetTeamColor(GetPlayerTeam(iMurdererPlr)), TeamGetKills(GetPlayerTeam(iMurdererPlr))), TeamGetKills(GetPlayerTeam(iMurdererPlr))+1);
      SetScoreboardData(iMurdererPlr, TEAM_KillColumn, Format("%d", aKill[iMurdererPlr]), aKill[iMurdererPlr]);
    }
  }
  SortTeamScoreboard();
  // Geld
  Money(iPlr, pClonk, iMurdererPlr);
}

// Default-Verhalten für Geld
public func Money(int iPlr, object pClonk, int iMurdererPlr)
{
  if(iMurdererPlr != NO_OWNER)
  {
    // Teamkill! Wert des Clonks wird abgezogen
    if(GetPlayerTeam(iPlr) == GetPlayerTeam(iMurdererPlr))
      DoWealth(iMurdererPlr, - pClonk->GetValue(0,0,0, iMurdererPlr));
    // Regulärer Kill, Wert des Clonks wird vergeben
    else
      DoWealth(iMurdererPlr, + pClonk->GetValue(0,0,0, iMurdererPlr));
  }
  // Geld für den getöteten, halber Wert des Clonks wird gegeben
  DoWealth(iPlr, + pClonk->GetValue(0,0,0, iPlr) / 2);
}
// Hinweis: die Werte können in CalcValue-Aufrufe verändert werden

/* Globale Funktionen */

global func DoTeamScore(int iTeam, int iChange)
{
  for(var pGoal in FindObjects(Find_Func("IsTeamGoal")))
    pGoal->~ScoreChange(iTeam, iChange);
}

global func SetWinScore(int iNewScore, object pGoal)
{
  if(pGoal)
  {
    pGoal->~WinScoreChange(iNewScore);
    return(0);
  }
  for(var goal in FindObjects(Find_Func("IsTeamGoal")))
    goal->~WinScoreChange(iNewScore);
}

global func GetWinScore(object pGoal)
{
  return(LocalN("iWinScore", pGoal));
}

/* Status */

public func IsTeamGoal() { return(1); }
