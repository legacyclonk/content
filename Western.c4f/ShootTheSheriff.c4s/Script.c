/*-- Shoot The Sheriff --*/

#strict

static relleft, relright, SHRF1, SHRF2;
static killleft, killright;

protected func Initialize()
{
 // Relaunchanzahl
 relleft=5;
 relright=5;
 
 // Ziel und Regeln erschaffen
 if (!FindObject(MELE)) CreateObject(MELE);  //Teammelee
 if (!FindObject(AMSP)) CreateObject(AMSP);  //Munitionsnachschub
 if (!FindObject(DRSI)) CreateObject(DRSI);  //Angebot im Drugstore
 if (!FindObject(REAC)) CreateObject(REAC);  //Clonks entlassen
 
 if (!FindObject(_TAS)) CreateObject(_TAS, 100, 100);  // Team
 
 //Aufzüge bauen und Schächte graben
 var ELEV1 = CreateConstruction(ELEV, 24, 300, NO_OWNER, 100, 1);
 var ELEV2 = CreateConstruction(ELEV, 1777, 299, NO_OWNER, 100, 1);
 var ELEV3 = CreateConstruction(ELEV, 950, 230, NO_OWNER, 100, 1);
 var ELEV4 = CreateConstruction(ELEV, 804, 351, NO_OWNER, 100, 1);
 while(SetVar(0, FindObject(ELEV, 6, 237, 40, 70, 0, 0, 0, 0, Var())))
    ObjectCall(Var(), "CreateShaft", 207);
 while(SetVar(0, FindObject(ELEV, 785, 288, 40, 70, 0, 0, 0, 0, Var())))
    ObjectCall(Var(), "CreateShaft", 108);
 while(SetVar(0, FindObject(ELEV, 933, 169, 40, 70, 0, 0, 0, 0, Var())))
    ObjectCall(Var(), "CreateShaft", 70);
 while(SetVar(0, FindObject(ELEV, 1758, 236, 40, 70, 0, 0, 0, 0, Var())))
    ObjectCall(Var(), "CreateShaft", 220);
  
 // Nachschub für Fortville
 AddEffect("Supply", FindObject(AMRY, 155, 280, -1, -1), 1, 12500, 0, 0, DYNB, 3);
 AddEffect("Supply", FindObject(AMRY, 366, 280, -1, -1), 1, 7500, 0, 0, CABL, 5);
 AddEffect("Supply", FindObject(AMRY, 366, 280, -1, -1), 1, 7500, 0, 0, GUNP, 5);  
    
 // Nachschub für Coalstown
 AddEffect("Supply", FindObject(AMRY, 1645, 280, -1, -1), 1, 12500, 0, 0, DYNB, 3);
 AddEffect("Supply", FindObject(AMRY, 1440, 280, -1, -1), 1, 7500, 0, 0, CABL, 5);
 AddEffect("Supply", FindObject(AMRY, 1440, 280, -1, -1), 1, 7500, 0, 0, GUNP, 5);     

 // Scoreboard erzeugen
 SetScoreboardData(SBRD_Caption, SBRD_Caption, "Spielstand",                    SBRD_Caption);
 SetScoreboardData(SBRD_Caption, ScoreboardCol(RELA),       "{{COWB}}",              ScoreboardCol(RELA));
 SetScoreboardData(SBRD_Caption, ScoreboardCol(KILL),       "{{REVR}}",              ScoreboardCol(KILL));  
 SetScoreboardData(ScoreboardCol(TEM1),       SBRD_Caption,       "Fortville", ScoreboardCol(TEM1));
 SetScoreboardData(ScoreboardCol(TEM2),       SBRD_Caption,       "Coalstown", ScoreboardCol(TEM2));
 
 ActualizeScoreboard(1);
 
 // Scriptzähler starten
 return(ScriptGo(1));  
       
 return(1);
}

func GetTeamName(iTeam)
{
  if(iTeam==1) return("Fortville");
  if(iTeam==2) return("Coalstown");
}

global func ActualizeScoreboard(fNoShow)
{
 SetScoreboardData(ScoreboardCol(TEM1),       ScoreboardCol(RELA),       Format("%d", relleft), relleft);
 SetScoreboardData(ScoreboardCol(TEM2),       ScoreboardCol(RELA),       Format("%d", relright), relright);
 SetScoreboardData(ScoreboardCol(TEM1),       ScoreboardCol(KILL),       Format("%d", killleft), killleft);
 SetScoreboardData(ScoreboardCol(TEM2),       ScoreboardCol(KILL),       Format("%d", killright), killright); 
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
 CreateObject(AHUD, 0, 0, player);
 // Den Spieler ausrüsten und platzieren
 CreateObject(_PLO, LandscapeWidth() / 2, 50, player)->InitPlayer(player, iTeam);
 return(1);
}


 protected func RemovePlayer(int player)
{
 // Spielervariable zurücksetzen, damit Laufzeitbeitritt mit Aufgeben klappt
 Global(player) = 0;
 return(1);
}

protected func RelaunchPlayer(int player, int killed_by, bool fSheriff)
{
 // Den Tod des Spielers verkünden
 ExtraLog("$MsgRelaunch$", GetPlayerName(player));
 Sound("Trumpet");

 var team=CheckTeam(player);
 
 if(team==1)
  {
   relleft--;
   ActualizeScoreboard();
   if(relleft < 0)
    {
     EliminatePlayer(player);
     ExtraLog("$MsgLost$", GetPlayerName(player));
     return(1);
    }
   if(relleft==1) { ExtraLog("$MsgFortRel1$"); }
   else ExtraLog("$MsgFortRel2$",relleft);
  }
 
 else
  {
   relright--;
   ActualizeScoreboard();
   if(relright < 0)
    {
     EliminatePlayer(player);
     ExtraLog("$MsgLost$", GetPlayerName(player));
     return(1);
    }
   if(relright==1) { ExtraLog("$MsgCoalRel1$"); }
   else ExtraLog("$MsgCoalRel2$",relright);
  }

 // Dem Spieler drei neue Clonks geben und diese ausrüsten
 for (var i; i < 3; i++)
 {
  var cowb = CreateObject(COWB, 0,0, player);
  MakeCrewMember(cowb, player);
  DoEnergy(100-GetEnergy(cowb), cowb);
 }
 SetCursor(player, GetCrew(player,1));
 //SetOwner(player, CreateContents(FLAG, GetCrew(player,1)));

 // Jetzt muss der Spieler noch etwas warten
 CreateObject(_PLO, LandscapeWidth() / 2, 50, player)->RelaunchPlayer(player, fSheriff);
 return(1);
}

// Nachschub
global func FxSupplyStart(pTarget, iNumber, fTmp, idID, iAmount)
{
  if(fTmp) return();    
  EffectVar(0, pTarget, iNumber) = idID;
  EffectVar(1, pTarget, iNumber) = iAmount;
}

global func FxSupplyTimer(pTarget, iNumber)
{
  if(ContentsCount(EffectVar(0, pTarget, iNumber), pTarget)<EffectVar(1, pTarget, iNumber)) CreateContents(EffectVar(0, pTarget, iNumber), pTarget);    
}

// Spielziel
protected func Script10()
{
 if(GetAlive(SHRF1) && GetAlive(SHRF2))
   goto(13);
 else goto(9);
}

 protected func Script14()
{
 if (!GetAlive(SHRF1)) 
 return(Team1Lose());
 if (!GetAlive(SHRF2)) 
 return(Team2Lose());
 goto(13);
}

protected func Team1Lose()
{
   ExtraLog("$MsgFortLost$");
   var winnerTeam = 2; 
// ...und die Verlierer eliminieren
  for (var i = GetPlayerCount(); i--;)
   {
  if (CheckTeam(GetPlayerByIndex(i)) != winnerTeam)
  EliminatePlayer(GetPlayerByIndex(i));
   }
}

protected func Team2Lose()
{
   ExtraLog("$MsgCoalLost$");
   var winnerTeam = 1; 
// ...und die Verlierer eliminieren
  for (var i = GetPlayerCount(); i--;)
   {
  if (CheckTeam(GetPlayerByIndex(i)) != winnerTeam)
  EliminatePlayer(GetPlayerByIndex(i));
   }
}

// Callbacks aus dem PlayerObject
func JoinTeam(iPlr, iTeam)
{
  if(iTeam==1) ExtraLog("$MsgFortJoin$", GetPlayerName(iPlr));
  if(iTeam==2) ExtraLog("$MsgCoalJoin$", GetPlayerName(iPlr));
}

func GetTeamStartPosition(iTeam, fY)
{
  if(iTeam==1) { if(fY) return(294); return(RandomX( 210, 310)); }
  if(iTeam==2) { if(fY) return(294); return(RandomX(1490,1590)); }
}

func InitializeTeam(iTeam, iPlr)
{
  if(iTeam==1)
  {
    AcquireBase(iPlr, 0,110,525,200); 
    SetOwner(iPlr, CreateContents(FLAG, FindObjectOwner(HEDQ, iPlr)));
    SHRF1=GetHiRank(iPlr);
    // Besten Clonk zum Sheriff machen
    RedefineCowboy(SHRF, GetHiRank(iPlr));
    SetPosition(225,234, GetHiRank(iPlr));
    // Dafür noch einen extra Cowboy geben
    MakeCrewMember(CreateObject(COWB,GetX(GetCrew(iPlr, 1)),GetY(GetCrew(iPlr, 1))+10,iPlr),iPlr); 
    SetPhysical("Energy", 125000, 2, SHRF1);
    DoEnergy(125,SHRF1);
    CreateContents(AMBO,SHRF1);
    CreateContents(REVR,SHRF1);
  }
  if(iTeam==2)
  {
    AcquireBase(iPlr, 1275,110,525,200); 
    SetOwner(iPlr, CreateContents(FLAG, FindObjectOwner(HEDQ, iPlr)));
    SHRF2=GetHiRank(iPlr);
    // Besten Clonk zum Sheriff machen
    RedefineCowboy(SHRF, GetHiRank(iPlr));
    SetPosition(1569,234, GetHiRank(iPlr));
    // Dafür noch einen extra Cowboy geben
    MakeCrewMember(CreateObject(COWB,GetX(GetCrew(iPlr, 1)),GetY(GetCrew(iPlr, 1))+10,iPlr),iPlr); 
    SetPhysical("Energy", 125000, 2, SHRF2);
    DoEnergy(125,SHRF2);
    CreateContents(AMBO,SHRF2);
    CreateContents(REVR,SHRF2);
  }
}
