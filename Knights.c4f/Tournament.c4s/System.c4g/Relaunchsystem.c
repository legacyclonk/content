/* Scoreboard und Relaunchfunktionen */
#strict

//static const KillsPerRelaunchGain = 4;

// Globale Arrays
global func & Relaunches(int iPlr) { return (Global(iPlr*2)); }
global func & Kills(int iPlr)      { return (Global(iPlr*2+1)); }

global func UpdateScoreboard(int iPlr, bool fShow) {
  //zur Sicherheit
  //if(Relaunches(iPlr)<0) return(GameCall("RemovePlayer",iPlr));
  var iPlrID = GetPlayerID(iPlr);
  // Kills
  var iColID = ScoreboardCol(KILL);
  SetScoreboardData(iPlrID,       iColID,       Format("%d", Kills(iPlr)), Kills(iPlr));
  SortScoreboard(iColID, true);
  // Relaunches
  iColID = ScoreboardCol(KNIG);
  SetScoreboardData(iPlrID,       iColID,       Format("%d", Relaunches(iPlr)), Relaunches(iPlr));
  SortScoreboard(iColID, true);
  // Scoreboard zeigen
  if (fShow)
    {
    DoScoreboardShow(1, iPlr+1);
    Schedule(Format("DoScoreboardShow(-1, %d)", iPlr+1), 38*5);
    }
  return(true);
  }

global func InitializeScoreboard(int iPlr) {
 // Scoreboard-Überschriften initialisieren
 var iPlrID = GetPlayerID(iPlr), iColID = ScoreboardCol(KNIG); // lives
 SetScoreboardData(SBRD_Caption, iColID,       "{{KNIG}}",              iColID);
 iColID = ScoreboardCol(KILL); // kills
 SetScoreboardData(SBRD_Caption, iColID,       "{{SWOR}}",              iColID);

 SetScoreboardData(SBRD_Caption, SBRD_Caption, "Lives / Kills",                    SBRD_Caption);
 SetScoreboardData(iPlrID,       SBRD_Caption, GetTaggedPlayerName(iPlr), GetPlayerID(iPlr));
  
 // Anfangswerte einsetzen
 UpdateScoreboard(iPlr, false); 
 }

global func GiveKill(int iKiller, int iVictim, bool fTeamkill) {
 if (!fTeamkill) {
  var iKills = ++Kills(iKiller);
    // Genug Kills für einen Bonus-Relaunch?
  //if (!(iKills%KillsPerRelaunchGain)) ++Relaunches(iKiller);
  }/*
 var pCursor;
 if (pCursor = GetCursor(iKiller)) {
  if (!fTeamkill) { // Bonus
   CreateContents(Reward(1),pCursor);
   CreateContents(Reward(0),pCursor);
   CreateContents(Reward(0),pCursor);	
   }
  else // Teamkills geben nur einen Sattel.
   CreateContents(KSDL, pCursor);
  }
 //  Scoreboard-Update für den Mörder - Das Opfer kriegt ohnehin ein Update in der aufrufenden Funktion (RelaunchPlayer)*/
 UpdateScoreboard(iKiller);
 return(true);
 } 