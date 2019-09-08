/*-- Miners Creek --*/

#strict

static relleft, relright, destleft, destright, destutil;
static maxleft, maxright, maxutil;

protected func Initialize()
{
 // Relaunchanzahl - Garde 7, Banditen 5
 relleft=7;
 relright=5;
 maxleft = 2;
 maxright = 6;
 maxutil = 0;
 
 // Ziel erschaffen
 if (!FindObject(MELE)) CreateObject(MELE);  //Teammelee
 if (!FindObject(AMSP)) CreateObject(AMSP);  //Munitionsnachschub  
 if (!FindObject(DRSI)) CreateObject(DRSI);  //Angebot im Drugstore
 if (!FindObject(BDST)) CreateObject(BDST);  //Gebäudezerstörung

 if (!FindObject(BALA)) CreateObject(BALA);  //Balancing
 
 if (!FindObject(_TAS)) CreateObject(_TAS, 100, 100);  // Team
   
 // Fahrstühle aufbauen und Schächte graben 
 var ELEV1 = CreateConstruction(ELEV, 31, 240, NO_OWNER, 100, 1);
 var ELEV2 = CreateConstruction(ELEV, 1859, 211, NO_OWNER, 100, 1);
 var ELEV3 = CreateConstruction(ELEV, 1736, 221, NO_OWNER, 100, 1);
 while(SetVar(0, FindObject(ELEV, 5, 180, 50, 70, 0, 0, 0, 0, Var())))
    ObjectCall(Var(), "CreateShaft", 160);
 while(SetVar(0, FindObject(ELEV, 1835, 150, 50, 70, 0, 0, 0, 0, Var())))
    ObjectCall(Var(), "CreateShaft", 200);
 while(SetVar(0, FindObject(ELEV, 1710, 155, 50, 70, 0, 0, 0, 0, Var())))
    ObjectCall(Var(), "CreateShaft", 50);
 
 // Dynamiteffekt für das Banditen Waffenlager
 AddEffect("Supply", FindObject(AMRY, 1945, 291, -1, -1), 1, 12500, 0, 0, DYNB, 3);

 // Kanonenkugeln und Schießpulver für das Kavalleristen Waffenlager
 AddEffect("Supply", FindObject(AMRY, 417, 212, -1, -1), 1, 8500, 0, 0, CABL, 5);
 AddEffect("Supply", FindObject(AMRY, 417, 212, -1, -1), 1, 8500, 0, 0, GUNP, 5);

 // Scoreboard erzeugen (falls mit Net2 gespielt wird)
 SetScoreboardData(SBRD_Caption, ScoreboardCol(RELA),       "{{CVRM}}{{BNDT}}",              ScoreboardCol(RELA));
 SetScoreboardData(SBRD_Caption, ScoreboardCol(DEST),       "{{DRST}}",              ScoreboardCol(DEST));
 SetScoreboardData(SBRD_Caption, SBRD_Caption, "$MsgScore$",                    SBRD_Caption);
 SetScoreboardData(ScoreboardCol(CVRM),       SBRD_Caption, "$MsgTrooper$",  ScoreboardCol(CVRM));
 SetScoreboardData(ScoreboardCol(BNDT),       SBRD_Caption, "$MsgBandits$", ScoreboardCol(BNDT));
 
 ActualizeScoreboard(1);
 
 // Scriptzähler starten
 return(ScriptGo(1));
}

func GetTeamName(iTeam)
{
  if(iTeam==1) return("$MsgTrooper$");
  if(iTeam==2) return("$MsgBandits$");
}

global func ActualizeScoreboard(fNoShow)
{
 if(ScoreboardCol(NET2))
 {
 SetScoreboardData(ScoreboardCol(CVRM),       ScoreboardCol(RELA),       Format("%d", relleft), relleft);
 SetScoreboardData(ScoreboardCol(BNDT),       ScoreboardCol(RELA),       Format("%d", relright), relright);
 SetScoreboardData(ScoreboardCol(CVRM),       ScoreboardCol(DEST),       Format("%d/%d", destleft, maxleft), destleft);
 SetScoreboardData(ScoreboardCol(BNDT),       ScoreboardCol(DEST),       Format("%d/%d", destright, maxright), destright);
 if(maxutil) 
  SetScoreboardData(ScoreboardCol(BNDT),       ScoreboardCol(UTIL),       Format("%d/%d", destutil, maxutil), destutil); 
 if(!fNoShow) DoScoreboardShowAll(1, 5);
 }
 else // Für Nicht-Net2 gibt es eine Ersatzinfo
 {
   var szUtil = "";
   if(maxutil) szUtil = Format("%d/%d{{PUMW}}", destutil, maxutil);
   var szCVRM = Format("<c %x> $MsgTrooper$ %d{{CVRM}} %d/%d{{DRST}}</c>", RGB(255,255,255), relleft, destleft, maxleft);
   var szBNDT = Format("|<c %x> $MsgBandits$ %d{{BNDT}} %d/%d{{DRST}} %s</c>", RGB(255,255,255), relright, destright, maxright, szUtil);
   Message("%s%s", 0, szCVRM, szBNDT);
 }
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
  CreateObject(_PLO, LandscapeWidth() / 2, 50, player)->InitPlayer(player, iTeam);
  var obj, i;
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
 var team=CheckTeam(player);
 
 if(team==1)
  {
   ExtraLog("$MsgTroopLost$", GetPlayerName(player));
   Sound("Trumpet");
   relleft--;
   ActualizeScoreboard();
   if(relleft < 0)
    {
     EliminatePlayer(player);
     ExtraLog("$MsgEliminate$",GetPlayerName(player));
     return(1);
    }
   if(relleft==1) { ExtraLog("$MsgRelaunchCVRM1$"); }
   else ExtraLog("$MsgRelaunchCVRM$",relleft);
  }
 
 else
  {
   ExtraLog("$MsgGangLost$", GetPlayerName(player));
   Sound("Trumpet");
   relright--;
   ActualizeScoreboard();
   if(relright < 0)
    {
     EliminatePlayer(player);
     ExtraLog("$MsgEliminate$",GetPlayerName(player));
     return(1);
    }
   if(relright==1) { ExtraLog("$MsgRelaunchBNDT1$"); }
   else ExtraLog("$MsgRelaunchBNDT$",relright);
  }

 // Dem Spieler vier neue Clonks geben
 var id = CVRM;
 if(CheckTeam(player)==2) id = BNDT;
 for (var i; i < 4; i++)
 {
  var caval = CreateObject(id, 0,0, player);
  MakeCrewMember(caval, player);
  DoEnergy(100-GetEnergy(caval), caval);
 }
 SetCursor(player, GetCrew(player,1));
 //SetOwner(player, CreateContents(FLAG, GetCrew(player,1))); //Flagge gibts erstmal keine beim Rel.

 // Jetzt muss der Spieler noch etwas warten
 CreateObject(_PLO, LandscapeWidth() / 2, 50, player)->RelaunchPlayer(player);
 return(1);
}

global func Team1Lose()
{
   ExtraLog("$MsgBanditsWin$");
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
   ExtraLog("$MsgTrooperWin$");
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
  if(iTeam==1)
  {
    ExtraLog("$MsgTrooperJoin$", GetPlayerName(iPlr));
    while(FindObjectOwner(COWB, iPlr)) RedefineCowboy(CVRM, FindObjectOwner(COWB, iPlr)); 
  }
  if(iTeam==2)
  {
    ExtraLog("$MsgBanditsJoin$", GetPlayerName(iPlr));
    while(FindObjectOwner(COWB, iPlr))
    {
      var obj = FindObjectOwner(COWB, iPlr);
      RedefineCowboy(BNDT, obj);
      AddEffect("DeathMoney", obj, 1, 0, obj);
    }
  }
}

func GetTeamStartPosition(iTeam, fY)
{
  if(iTeam==1) { if(fY) return(184); return(RandomX( 165, 285)); }
  if(iTeam==2) { if(fY) return(195); return(RandomX(1888,1960)); }
}

func InitializeTeam(iTeam, iPlr)
{
  if(iTeam==1)
  {
    AcquireBase(iPlr, 0, 60, 600, 200);
    SetOwner(iPlr, CreateContents(FLAG, FindObjectOwner(HEDQ, iPlr)));
  }
  if(iTeam==2)
  {
    AcquireBase(iPlr, 1685, 100, 315, 460);
    SetOwner(iPlr, CreateContents(FLAG, FindObjectOwner(HEDQ, iPlr)));
  }
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

