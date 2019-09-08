/*-- Capture The Flag --*/

#strict

static scoreleft, scoreright;
static killleft, killright;
static capturelimit;
static stateleft, stateright;

protected func Initialize()
{
   
 capturelimit = 5;
 
 // Einstellungen und Ziel erschaffen
 if (!FindObject(TACC)) CreateObject(TACC,  50,  50); // Teamkonto   
 if (!FindObject(CAPT)) CreateObject(CAPT, 100, 100); // Einstellungen   
 if (!FindObject(MELE)) CreateObject(MELE, 100, 100);  //Teammelee
 
 if (!FindObject(_TAS)) CreateObject(_TAS, 100, 100);  // Team
      
 // Fahrstühle aufbauen und Schächte graben 
 var ELEV1 = CreateConstruction(ELEV, 19, 261, NO_OWNER, 100, 1);
 var ELEV2 = CreateConstruction(ELEV, 420, 261, NO_OWNER, 100, 1);
 var ELEV3 = CreateConstruction(ELEV, 780, 100, NO_OWNER, 100, 1);
 var ELEV4 = CreateConstruction(ELEV, 1200, 100, NO_OWNER, 100, 1);
 var ELEV5 = CreateConstruction(ELEV, 1581, 261, NO_OWNER, 100, 1);
 var ELEV6 = CreateConstruction(ELEV, 1980, 261, NO_OWNER, 100, 1);
 while(SetVar(0, FindObject(ELEV, 0, 200, 50, 70, 0, 0, 0, 0, Var())))
    ObjectCall(Var(), "CreateShaft", 260);
 while(SetVar(0, FindObject(ELEV, 403, 202, 50, 70, 0, 0, 0, 0, Var())))
    ObjectCall(Var(), "CreateShaft", 200);
 while(SetVar(0, FindObject(ELEV, 763, 38, 50, 70, 0, 0, 0, 0, Var())))
    ObjectCall(Var(), "CreateShaft", 230);
 while(SetVar(0, FindObject(ELEV, 1184, 38, 50, 70, 0, 0, 0, 0, Var())))
    ObjectCall(Var(), "CreateShaft", 230);
 while(SetVar(0, FindObject(ELEV, 1564, 202, 50, 70, 0, 0, 0, 0, Var())))
    ObjectCall(Var(), "CreateShaft", 200);
 while(SetVar(0, FindObject(ELEV, 1964, 202, 50, 70, 0, 0, 0, 0, Var())))
    ObjectCall(Var(), "CreateShaft", 230);
    
 // Fahnenmasten setzen
 var FLAG1 = CreateConstruction(FLPO, 217, 560, NO_OWNER, 100, 1);
 LocalN("iTeam", FLAG1) = 1;
 var FLAG2 = CreateConstruction(FLPO, 1806, 540, NO_OWNER, 100, 1);
 LocalN("iTeam", FLAG2) = 2;
 SetColorDw(RGB(255), FLAG2);
 stateleft = stateright = 1;
 
 // Nachlieferung Waffenlager
 AddEffect("Supply", FindObject(AMRY, 291, 238, -1, -1), 1, 10000, 0, 0, DYNB, 3);
 AddEffect("Supply", FindObject(AMRY, 1707, 238, -1, -1), 1, 10000, 0, 0, DYNB, 3);

 // Kanonenkugeln und Schießpulver für die Kanonen
 AddEffect("Supply", FindObject(CCAN, 169, 247, -1, -1), 1, 6000, 0, 0, CABL, 4);
 AddEffect("Supply", FindObject(CCAN, 169, 247, -1, -1), 1, 6000, 0, 0, GUNP, 4);
 AddEffect("Supply", FindObject(CCAN, 1835, 247, -1, -1), 1, 6000, 0, 0, CABL, 4);
 AddEffect("Supply", FindObject(CCAN, 1835, 247, -1, -1), 1, 6000, 0, 0, GUNP, 4);
 
 // Gebäude sollen mehr aushalten
  var obj;
  while(obj = FindObject(0, 0, 0, -1, -1, OCF_Exclusive, 0, 0, 0, obj))
  {
    if(GetID(obj)==ELEV) { AddEffect("LessDamage", obj, 400, 0, 0, 0, 25); SetCategory(1, obj);}
    else  AddEffect("LessDamage", obj, 400, 0, 0, 0, 50);
  }
  obj = 0;
  while(obj = FindObject(BRDG, 0, 0, -1, -1, 0, 0, 0, 0, obj))
  {
    AddEffect("LessDamage", obj, 400, 0, 0, 0, 50);
  }
  obj = 0;
  while(obj = FindObject(ELEC, 0, 0, -1, -1, 0, 0, 0, 0, obj))
  {
    AddEffect("LessDamage", obj, 400, 0, 0, 0, 25);
  }
  // Fundamente für HQ und Armory unzerstörbar
  obj = 0; while(obj = FindObject(BAS9, 0, 0, -1, -1, 0, 0, 0, 0, obj)) AddEffect("LessDamage", obj, 400, 0, 0, 0, 0);
 
 // Scoreboard erzeugen
 SetScoreboardData(SBRD_Caption, SBRD_Caption, Format("Capturelimit: %d", capturelimit),                    SBRD_Caption);
 SetScoreboardData(SBRD_Caption, ScoreboardCol(FLAG),       "{{FLAG}}",              ScoreboardCol(FLAG));
 SetScoreboardData(SBRD_Caption, ScoreboardCol(KILL),       "{{REVR}}",              ScoreboardCol(KILL));  
 SetScoreboardData(SBRD_Caption, ScoreboardCol(STAT),       " ",              ScoreboardCol(STAT));
 SetScoreboardData(ScoreboardCol(TEM1),       ScoreboardCol(STAT),       "{{SFB1}}", 1);
 SetScoreboardData(ScoreboardCol(TEM2),       ScoreboardCol(STAT),       "{{SFR1}}", 1);
 SetScoreboardData(SBRD_Caption, ScoreboardCol(CVRM),       "{{SBFL}}",              ScoreboardCol(CVRM));
 SetScoreboardData(ScoreboardCol(TEM1),       SBRD_Caption,       "<c 0000ff>$MsgBlue$", ScoreboardCol(TEM1));
 SetScoreboardData(ScoreboardCol(TEM2),       SBRD_Caption,       "<c ff0000>$MsgRed$", ScoreboardCol(TEM2));
 
}

global func ActualizeScoreboard(fNoShow)
{
 SetScoreboardData(ScoreboardCol(TEM1),       ScoreboardCol(FLAG),       Format("%d", scoreleft), scoreleft);
 SetScoreboardData(ScoreboardCol(TEM2),       ScoreboardCol(FLAG),       Format("%d", scoreright), scoreright);
 SetScoreboardData(ScoreboardCol(TEM1),       ScoreboardCol(KILL),       Format("%d", killleft), killleft);
 SetScoreboardData(ScoreboardCol(TEM2),       ScoreboardCol(KILL),       Format("%d", killright), killright);
 SetScoreboardData(ScoreboardCol(TEM1),       ScoreboardCol(STAT),       GetStateSymbol(1, stateleft), stateleft);
 SetScoreboardData(ScoreboardCol(TEM2),       ScoreboardCol(STAT),       GetStateSymbol(2, stateright), stateright);
 var pFlag1 = GetFlagCarrier(1), pFlag2 = GetFlagCarrier(2);
 if(pFlag1) SetScoreboardData(ScoreboardCol(TEM2), ScoreboardCol(CVRM), Format("<i>%s",GetPlayerName(GetOwner(pFlag1))), 1);
 else SetScoreboardData(ScoreboardCol(TEM2),       ScoreboardCol(CVRM),       0, 0);
 if(pFlag2) SetScoreboardData(ScoreboardCol(TEM1), ScoreboardCol(CVRM), Format("<i>%s",GetPlayerName(GetOwner(pFlag2))), 1);
 else SetScoreboardData(ScoreboardCol(TEM1),       ScoreboardCol(CVRM),       0, 0);
 if(!fNoShow) DoScoreboardShowAll(1, 5);
}

global func GetStateSymbol(iTeam, iStat)
{
  if(iTeam==1)
  {
  if(iStat==1) return("{{SFB1}}");  
  if(iStat==2) return("{{SFB2}}");
  if(iStat==3) return("{{SFB3}}");
  }
  if(iStat==1) return("{{SFR1}}");  
  if(iStat==2) return("{{SFR2}}");
  if(iStat==3) return("{{SFR3}}");
}

global func GetFlagCarrier(iTeam)
{
  var obj, carrier;
  while(obj = FindObject(POSP, 0, 0, 0, 0, 0, "PortFly", 0, 0, obj))
  {
    if(carrier = GetActionTarget(0,obj))
    {
      if(CheckTeam(GetOwner(carrier))==iTeam) return(carrier);      
    }
  }
}

static iScoreboardShow;

global func DoScoreboardShowAll(iAmount, iTime)
{
    ResetScoreboard();
    DoScoreboardShow(iAmount);
    iScoreboardShow += iAmount;
    if(iTime)
    {	//Schedule("ResetScoreboard()", 38*iTime);
        goto(Max(1000-iTime*3, 0));
	ScriptGo(1);
    }
}

global func ResetScoreboard()
{
    DoScoreboardShow(-iScoreboardShow);
    iScoreboardShow=0;
    ScriptGo(0);
}

protected func Script1000() { ResetScoreboard(); }

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


protected func RelaunchPlayer(int player)
{
 // Den Tod des Spielers verkünden
 ExtraLog("$MsgRelaunch$", GetPlayerName(player));
 Sound("Trumpet");

 // Dem Spieler vier neue Kavalleristen geben
 for (var i; i < 4; i++)
 {
  var pCaval = CreateObject(CVRM, 0,0, player);
  var pFrom = GetPlayerClonkFromList(player);
  if(!pFrom) MakeCrewMember(pCaval, player);
  else GrabObjectInfo(pFrom, pCaval);
  DoEnergy(100-GetEnergy(pCaval), pCaval);
 }
 SetCursor(player, GetCrew(player,1));
 
 // Jetzt muss der Spieler noch etwas warten
 CreateObject(_PLO, LandscapeWidth() / 2, 50, player)->RelaunchPlayer(player);
 return(1);
}

// Flagge wird abgeliefert
global func Score(iTeam)
{
  if(iTeam==1) 
  { 
    DoWealth(GetPlrByTeam(1), 50); 
    Sound("Cash"); 
    if(++scoreleft>=capturelimit) Team2Lose();
    
  }
  if(iTeam==2) 
  { 
    DoWealth(GetPlrByTeam(2), 50);
    Sound("Cash");
    if(++scoreright>=capturelimit) Team1Lose();
  }
  ActualizeScoreboard();
}

global func Team1Lose()
{
  ExtraLog("$MsgRedWin$");
  var winnerTeam = 2; 
// ...und die Verlierer eliminieren
  for (var i = GetPlayerCount(); i--;)
   {
  if (CheckTeam(GetPlayerByIndex(i)) != winnerTeam)
  EliminatePlayer(GetPlayerByIndex(i));
   }
}

global func Team2Lose()
{
  ExtraLog("$MsgBlueWin$");
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
  if(iTeam==1) ExtraLog("$MsgBlueJoin$", GetPlayerName(iPlr));
  if(iTeam==2) ExtraLog("$MsgRedJoin$", GetPlayerName(iPlr));
}

func GetTeamStartPosition(iTeam, fY)
{
  if(iTeam==1) { if(fY) return(207); return(RandomX( 235, 285)); }
  if(iTeam==2) { if(fY) return(207); return(RandomX(1710,1760)); }
}

func InitializeTeam(iTeam, iPlr)
{
  if(iTeam==1)
  {
    AcquireBase(iPlr, 0,75,470,225);
    SetOwner(iPlr, CreateContents(FLAG, FindObjectOwner(HEDQ, iPlr)));
  }
  if(iTeam==2)
  {
    AcquireBase(iPlr, 1530,75,470,225);
    SetOwner(iPlr, CreateContents(FLAG, FindObjectOwner(HEDQ, iPlr)));
  }
}

func GetTeamName(iTeam)
{
  if(iTeam==1) return("$MsgBlue$");
  if(iTeam==2) return("$MsgRed$");
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

// Weniger Schaden
global func FxLessDamageStart(pTarget, iNumber, fTmp, iAmount)
{
  if(fTmp) return();    
  if(!iAmount) iAmount = 50;
  EffectVar(0, pTarget, iNumber) = iAmount;
}

global func FxLessDamageDamage (pTarget, iNumber, iDmgEngy, iCause)
{
  if(iCause == 1) return(iDmgEngy*EffectVar(0, pTarget, iNumber)/100);
  return(iDmgEngy);   
}
