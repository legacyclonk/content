/*-- Hammerfest --*/

#strict
static relleft, relright, destleft, destright;

protected func Initialize()
{

 // Ziel und Regeln erschaffen
 if (!FindObject(MELE)) CreateObject(MELE);  //Teammelee
 if (!FindObject(BDST)) CreateObject(BDST);  //Gebäudezerstörung
 if (!FindObject(STSN)) CreateObject(STSN);  //Gebäude schneien ein
 if (!FindObject(RSNB)) CreateObject(RSNB);  //Schneebälle
 
 // Alle Luken freigraben
 var hatch;
 while (hatch = FindObject(CPHC, 0,0,0,0, 0, 0,0, 0, hatch))
 {
  hatch->Open();
  DigFreeRect(GetX(hatch)-8, GetY(hatch), 16, 12);
  hatch->Close();
 }
 
 // Fahrstuhlschächte graben
 while(SetVar(0, FindObject(CPEL, 0, 0, 0, 0, 0, 0, 0, 0, Var())))
    ObjectCall(Var(), "CreateShaft", 165);

 //Relaunchanzahl - hier 5
 relleft=5;
 relright=5;
       
 // Scoreboard erzeugen
 SetScoreboardData(SBRD_Caption, ScoreboardCol(RELA),       "{{SWOR}}{{ARMR}}",              ScoreboardCol(RELA));
 SetScoreboardData(SBRD_Caption, ScoreboardCol(DEST),       "{{CPOF}}",              ScoreboardCol(DEST));
 SetScoreboardData(SBRD_Caption, SBRD_Caption, "$MsgScore$",                    SBRD_Caption);
 SetScoreboardData(ScoreboardCol(LEFT),       SBRD_Caption, "$MsgLeft$",  ScoreboardCol(LEFT));
 SetScoreboardData(ScoreboardCol(RIGH),       SBRD_Caption, "$MsgRight$", ScoreboardCol(RIGH));
 
 ActualizeScoreboard(1);
 
 // Scriptzähler starten
 return(ScriptGo(1));
}

global func ActualizeScoreboard(fNoShow)
{
 SetScoreboardData(ScoreboardCol(LEFT),       ScoreboardCol(RELA),       Format("%d", relleft), relleft);
 SetScoreboardData(ScoreboardCol(RIGH),       ScoreboardCol(RELA),       Format("%d", relright), relright);
 SetScoreboardData(ScoreboardCol(LEFT),       ScoreboardCol(DEST),       Format("%d/4", destleft), destleft);
 SetScoreboardData(ScoreboardCol(RIGH),       ScoreboardCol(DEST),       Format("%d/4", destright), destright);
 if(!fNoShow) DoScoreboardShowAll(1, 5);
} 

global func DoScoreboardShowAll(iAmount, iTime)
{
  var iPlr; 
  for(var i = 0; i < GetPlayerCount(); i++)
  {
    iPlr = GetPlayerByIndex(i);       
    DoScoreboardShow(iAmount, iPlr+1);
    Schedule(Format("DoScoreboardShow(-%d, %d)", iAmount, iPlr+1), 38*iTime);
  }
}

protected func InitializePlayer(int player, int tx, int ty, object pBase, int iTeam)
{
  // Den Spieler ausrüsten und platzieren
 SetOwner(player, CreateContents(FLAG, GetCrew(player,1)));
 CreateObject(_PLO, LandscapeWidth() / 2, 50, player)->InitPlayer(player, iTeam);
 return(1);
}

protected func RemovePlayer(int player)
{
 // Spielervariable zurücksetzen, damit Laufzeitbeitritt mit Aufgeben funktioniert
 Global(player) = 0;
 return(1);
}

protected func RelaunchPlayer(int player)
{
 // Den Tod des Spielers verkünden
 Log("%s benötigt einen Relaunch!", GetPlayerName(player));
 Sound("Trumpet");

 var team=CheckTeam(player);
 
 if(team==1)
  {
   relleft--;
   ActualizeScoreboard();
   if(relleft < 0)
    {
     EliminatePlayer(player);
     Log("%s wurde besiegt!", GetPlayerName(player));
     return(1);
    }
   if(relleft==1) { Log("1 Relaunch verbleibt für die Schattenklingen"); }
   else Log("%d Relaunches verbleiben für die Schattenklingen",relleft);
  }
 
 else
  {
   relright--;
   ActualizeScoreboard();
   if(relright < 0)
    {
     EliminatePlayer(player);
     Log("%s wurde besiegt!", GetPlayerName(player));
     return(1);
    }
   if(relright==1) { Log("1 Relaunch verbleibt für den Mantisorden"); }
   else Log("%d Relaunches verbleiben für den Mantisorden",relright);
  }

 // Dem Spieler drei neue Ritter geben und diese ausrüsten
 for (var i; i < 3; i++)
 {
  var knight = CreateObject(KNIG, 0,0, player);
  MakeCrewMember(knight, player);
  DoEnergy(100-GetEnergy(knight), knight);
 }
 SetCursor(player, GetCrew(player,1));
 SetOwner(player, CreateContents(FLAG, GetCrew(player,1)));

 // Jetzt muss der Spieler noch etwas warten
 CreateObject(_PLO, LandscapeWidth() / 2, 50, player)->RelaunchPlayer(player);
 return(1);
}

global func Team1Lose()
{
   Log("Der Mantisorden siegt!!");
   var winnerTeam = 1; 
// ...und die Verlierer eliminieren
  for (var i = GetPlayerCount(); i--;)
   {
  if (Global(GetPlayerByIndex(i)) != winnerTeam)
  EliminatePlayer(GetPlayerByIndex(i));
  }
}

global func Team2Lose()
{
   Log("Die Schattenklingen entscheiden den Kampf für sich!");
   var winnerTeam = -1; 
// ...und die Verlierer eliminieren
  for (var i = GetPlayerCount(); i--;)
   {
  if (Global(GetPlayerByIndex(i)) != winnerTeam)
  EliminatePlayer(GetPlayerByIndex(i));
   }
}

/*-- Ein Script zur Teamüberprüfung --*/
/* Global(iPlr)= -1; ->links Global(iPlr)=1; ->rechts siehe PlayerObject script */

global func CheckTeam(int iPlr)
 {
  if(Global(iPlr)==-1) return(1);
  else return(2);
 }


