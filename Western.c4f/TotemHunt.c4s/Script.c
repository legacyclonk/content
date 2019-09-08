/*-- Totemjagd --*/

#strict

static fTeamWolves, fTeamBears, fTeamTrapper;
static relWOLF, relBEAR, relTRPR;
static killWOLF, killBEAR, killTRPR;
static damageWOLF, damageBEAR, damageTRPR;
static fScoreboard;
static fTeam01, fTeam02, fTeam03;

global func MaxTotemDamage() { return(300); }

protected func Initialize()
{
  if(!FindObject(MELE)) CreateObject(MELE, 100, 100);  //Teammelee

  if(!FindObject(_ETG)) CreateObject(_ETG, 50, 50); // Löscher
  if(!FindObject(_TAS)) CreateObject(_TAS,100,100); // Team
  if(!FindObject(ST4V)) CreateObject(ST4V, 50, 50); // Sichtbare Fallen
  if(!FindObject(NR4V)) CreateObject(NR4V, 50, 50); // Keine Ausbildungen
  if(!FindObject(NW4V)) CreateObject(NW4V, 50, 50); // Kein Wallhack
  if(!FindObject(CP4V)) CastObjects(CP4V,2,,50,50); // Begrentzte Tränke

  relWOLF = 3;
  relBEAR = 3;
  relTRPR = 3;

  AddEffect("Goodies", 0, 1, 500, 0, 0,  809, 647);
  AddEffect("Goodies", 0, 1, 500, 0, 0, 1228, 617);
  AddEffect("Goodies", 0, 1, 500, 0, 0, 1281, 777);
  AddEffect("Goodies", 0, 1, 500, 0, 0, 1120, 777);
  AddEffect("Goodies", 0, 1, 500, 0, 0,  764, 776);
  AddEffect("Goodies", 0, 1, 500, 0, 0, 1050, 727);

//  ScriptGo();
  ScriptGo(1);

  return(1);
}
//740,730 - 1135,800 - 1320 _VEG

protected func Script50()
{
  var aList, pObj;
  if(!Random(3))
    // Irgendwelche Clonks im Gebiet gesichtet?
//    if(!FindObject2(Find_OCF(OCF_CrewMember),Find_InRect(740,730,580,70))) {
    // Noch genug Schlangen im Tempel vorhanden?
    if(ObjectCount2(Find_ID(SNKE),Find_OCF(OCF_Alive)) < 2) {
      aList = FindObjects(Find_ID(_VEG),Find_InRect(740,730,580,70));
      // Grünzeug vorhanden?
      if(pObj = aList[Random(GetLength(aList))])
        // Neue Schlange im Grünzeug gebären
        CreateConstruction(SNKE,GetX(pObj),GetY(pObj),-1,40)->~Birth();
    }
  // Zurück zum Anfang
  goto(0);
  return(1);
}

protected func InitializePlayer(int iPlayer, int tx, int ty, object pBase, int iTeam)
{
  // Kein Team? dann automatisch Team wählen
  if(!iTeam) iTeam = Mod(iPlayer, 3)+1;
  // Teamflags setzen
  if(iTeam==1) if(!fTeamWolves)  fTeamWolves  = 1;
  if(iTeam==2) if(!fTeamTrapper) fTeamTrapper = 1;
  if(iTeam==3) if(!fTeamBears)   fTeamBears   = 1;
  // Den Spieler ausrüsten und platzieren
  CreateObject(_PLO, LandscapeWidth() / 2, 50, iPlayer)->InitPlayer(iPlayer, iTeam, 1);
  // Zur Sicherheit
  InitializeTeam(iTeam, iPlayer);
  return(1);
}

protected func RemovePlayer(int iPlayer)
{
  // Spielervariable zurücksetzen, damit Laufzeitbeitritt mit Aufgeben klappt
  Global(iPlayer) = 0;
  return(1);
}

protected func RelaunchPlayer(int iPlayer)
{
  // Den Tod des Spielers verkünden
  ExtraLog("$MsgRelaunch$", GetPlayerName(iPlayer));
  Sound("Trumpet");

  var i = 4, iID = INDI, pClonk, fNoRelaunch;

  if(CheckTeam(iPlayer) == 1) {
    if(!relWOLF) fNoRelaunch = 1; 
    relWOLF--; 
  }
  if(CheckTeam(iPlayer) == 2) {
    if(!relTRPR) fNoRelaunch = 1; 
    iID = TRPR;
    i = 3;
    relTRPR--; 
  }
  if(CheckTeam(iPlayer) == 3) {
    if(!relBEAR) fNoRelaunch = 1; 
    relBEAR--; 
  }
  ActualizeScoreboard();
  if(fNoRelaunch) return();

  // Dem Spieler vier neue Clonks geben
  for(; i > 0; i--) {
    pClonk = CreateObject(iID,0,0,iPlayer);
    MakeCrewMember(pClonk,iPlayer);
    DoEnergy(100,pClonk);
  }
  SetCursor(iPlayer, GetCrew(iPlayer,1));
 
  // Jetzt muss der Spieler noch etwas warten
  CreateObject(_PLO, LandscapeWidth() / 2, 50, iPlayer)->RelaunchPlayer(iPlayer);
  return(1);
}

global func Team1Lose()
{
  ExtraLog("$Msg1Lost$");
  // die Verlierer eliminieren
  for(var i = GetPlayerCount(); i >= 0; i--) {
    if(CheckTeam(GetPlayerByIndex(i)) == 1)
    EliminatePlayer(GetPlayerByIndex(i));
  }
  fTeamWolves = 0;
}
global func Team2Lose()
{
  ExtraLog("$Msg2Lost$");
  // die Verlierer eliminieren
  for(var i = GetPlayerCount(); i >= 0; i--) {
    if(CheckTeam(GetPlayerByIndex(i)) == 2)
    EliminatePlayer(GetPlayerByIndex(i));
  }
  fTeamTrapper = 0;
}
global func Team3Lose()
{
  ExtraLog("$Msg3Lost$");
  // die Verlierer eliminieren
  for(var i = GetPlayerCount(); i >= 0; i--) {
    if(CheckTeam(GetPlayerByIndex(i)) == 3)
    EliminatePlayer(GetPlayerByIndex(i));
  }
  fTeamBears = 0;
}

global func FxTotemWOLFDamage(pTarget, iEffectNumber, iDmgEngy, iCause) 
{ damageWOLF+=iDmgEngy; if(damageWOLF>=MaxTotemDamage()) { damageWOLF=MaxTotemDamage(); Incinerate(pTarget); } ActualizeScoreboard();}
global func FxTotemBEARDamage(pTarget, iEffectNumber, iDmgEngy, iCause) 
{ damageBEAR+=iDmgEngy; if(damageBEAR>=MaxTotemDamage()) { damageBEAR=MaxTotemDamage(); Incinerate(pTarget); } ActualizeScoreboard();}
global func FxTotemTRPRDamage(pTarget, iEffectNumber, iDmgEngy, iCause) 
{ damageTRPR+=iDmgEngy; if(damageTRPR>=MaxTotemDamage()) { damageTRPR=MaxTotemDamage(); Incinerate(pTarget); Incinerate(FindObject(_POS)); } ActualizeScoreboard();}

func GetTeamName(iTeam)
{
  if(iTeam==1) return("$TeamWolves$");
  if(iTeam==2) return("$TeamTrappers$");
  if(iTeam==3) return("$TeamBears$");
}

global func ActualizeScoreboard(fNoShow)
{
  var szString = "";
  if(!fScoreboard) {
    SetScoreboardData(SBRD_Caption, SBRD_Caption, "Spielstand", SBRD_Caption);
    if(fTeamWolves)  szString = Format("%s{{SBWL}}", szString);
    if(fTeamTrapper) szString = Format("%s{{TRPR}}", szString);
    if(fTeamBears)   szString = Format("%s{{SBBR}}", szString);
    SetScoreboardData(SBRD_Caption,ScoreboardCol(RELA),szString,  ScoreboardCol(RELA));
    SetScoreboardData(SBRD_Caption,ScoreboardCol(KILL),"{{SKUL}}",ScoreboardCol(KILL));
    SetScoreboardData(SBRD_Caption,ScoreboardCol(TTPL),"{{SBTP}}",ScoreboardCol(TTPL));
    if(fTeamWolves)  SetScoreboardData(ScoreboardCol(WOLF), SBRD_Caption, "$TeamWolves$",   ScoreboardCol(WOLF));
    if(fTeamTrapper) SetScoreboardData(ScoreboardCol(TRPR), SBRD_Caption, "$TeamTrappers$", ScoreboardCol(TRPR));
    if(fTeamBears)   SetScoreboardData(ScoreboardCol(BEAR), SBRD_Caption, "$TeamBears$",    ScoreboardCol(BEAR));
    fScoreboard = 1;
  }   
  if(fTeamWolves) {
    SetScoreboardData(ScoreboardCol(WOLF), ScoreboardCol(RELA), Format("%d", relWOLF),  relWOLF);
    SetScoreboardData(ScoreboardCol(WOLF), ScoreboardCol(KILL), Format("%d", killWOLF), killWOLF);
    SetScoreboardData(ScoreboardCol(WOLF), ScoreboardCol(TTPL), 
      Format("%d", MaxTotemDamage()-damageWOLF), MaxTotemDamage()-damageWOLF);
  }
  if(fTeamTrapper) {
    SetScoreboardData(ScoreboardCol(TRPR), ScoreboardCol(RELA), Format("%d", relTRPR),  relTRPR);
    SetScoreboardData(ScoreboardCol(TRPR), ScoreboardCol(KILL), Format("%d", killTRPR), killTRPR);
    SetScoreboardData(ScoreboardCol(TRPR), ScoreboardCol(TTPL), 
      Format("%d", MaxTotemDamage()-damageTRPR), MaxTotemDamage()-damageTRPR);
  }
  if(fTeamBears) {
    SetScoreboardData(ScoreboardCol(BEAR), ScoreboardCol(RELA), Format("%d", relBEAR),  relBEAR);
    SetScoreboardData(ScoreboardCol(BEAR), ScoreboardCol(KILL), Format("%d", killBEAR), killBEAR);
    SetScoreboardData(ScoreboardCol(BEAR), ScoreboardCol(TTPL), 
      Format("%d", MaxTotemDamage()-damageBEAR), MaxTotemDamage()-damageBEAR);
  }
  if(fTeamWolves)  if(damageWOLF>=MaxTotemDamage()) Team1Lose();
  if(fTeamTrapper) if(damageTRPR>=MaxTotemDamage()) Team2Lose();
  if(fTeamBears)   if(damageBEAR>=MaxTotemDamage()) Team3Lose();
  if(!fNoShow) DoScoreboardShowAll(1, 5);
}

global func DoScoreboardShowAll(iAmount, iTime)
{
  DoScoreboardShow(iAmount);
  if(iTime) Schedule(Format("DoScoreboardShow(-%d)", iAmount), 35*iTime);
}

// Callbacks aus dem PlayerObject
func JoinTeam(iPlr, iTeam)
{
  if(iTeam==1) ExtraLog("$MsgWolfsJoin$", GetPlayerName(iPlr));
  if(iTeam==2) ExtraLog("$MsgTrapperJoin$", GetPlayerName(iPlr));
  if(iTeam==3) ExtraLog("$MsgBearsJoin$", GetPlayerName(iPlr));
}

func GetTeamStartPosition(iTeam, fY)
{
  if(iTeam==1) { if(fY) return(458); return(RandomX( 200, 226)); }
  if(iTeam==3) { if(fY) return(458); return(RandomX(1800,1860)); }
  if(iTeam==2)
  {
    if(fTeamWolves && fTeamBears) { if(fY) return(160); return(RandomX(1000,1040)); }
    else if(!fTeamWolves && !fTeamBears) { if(fY) return(160); return(RandomX(1000,1040)); }
    else if(fTeamWolves) 	  { if(fY) return(250); return(RandomX(1770,1810)); }
    else if(fTeamBears)  	  { if(fY) return(250); return(RandomX( 230, 270)); }
  }
}

func InitializeTeam(iTeam, iPlr)
{
  if(iTeam==1 && !fTeam01) {
    fTeam01 = 1;
    AcquireBase(iPlr, 0,350,510,430); 
    SetOwner(iPlr, CreateContents(FLAG, FindObjectOwner(MTIP, iPlr)));
    AddEffect("TotemWOLF", FindObject(TTPL, 284, 700, -1, -1), 1, 0);
    fScoreboard = 0;
    ActualizeScoreboard(1);
  }
  if(iTeam==2 && !fTeam02) {
    fTeam02 = 1;
    var i, pObj, pPost;
    if(!fTeamWolves && !fTeamBears) pPost = CreateObject(_POS, 1020, 128+35, -1);
    else if(!fTeamWolves) pPost = CreateObject(_POS, 239, 218+35, -1);
    else if(!fTeamBears) pPost = CreateObject(_POS, 1808, 217+35, -1);
    else pPost = CreateObject(_POS, 1020, 128+35, -1);
    SetColorDw(GetPlrColorDw(iPlr), pPost);
    pObj = CreateObject(TTPL, GetX(pPost), GetY(pPost)+32, -1);
    SetClrModulation(RGBa(255,255,255,255), pObj);
    Message("", pObj);
    SetName(GetName(pPost), pObj);
    SetCategory(16777217, pObj);
    AddEffect("TotemTRPR", pObj, 1, 0);
    SetOwner(iPlr, CreateContents(FLAG, GetHiRank(iPlr)));
    for(i = 0; i < 2; i++) {
      pObj = CreateObject(TENP,GetX(pPost)+RandomX(-25,25),GetY(pPost)+28,iPlr);
      CreateContents(WOOD,pObj,3);
      CreateContents(LTHR,pObj,2);
      CreateContents(ROPE,pObj,2);
//    SetOwner(iPlr, CreateContents(TENP, GetHiRank(iPlr)));
//    CreateObject(TENP, GetX(pPost), GetY(pPost)+28);
    }
    fScoreboard = 0;
    ActualizeScoreboard(1);
  }
  if(iTeam==3 && !fTeam03) {
    fTeam03 = 1;
    AcquireBase(iPlr, 1530,350,510,430);
    SetOwner(iPlr, CreateContents(FLAG, FindObjectOwner(MTIP, iPlr)));
    AddEffect("TotemBEAR", FindObject(TTPL,1773, 700, -1, -1), 1, 0);
    fScoreboard = 0;
    ActualizeScoreboard(1);
  }
}

global func FxGoodiesStart(pTarget, iNumber, fTmp, iX, iY)
{
  if(fTmp) return();
  EffectVar(0,pTarget,iNumber) = iX;
  EffectVar(1,pTarget,iNumber) = iY;
}

global func FxGoodiesTimer(pTarget, iNumber)
{
  if(Random(20)) return(0);
  var iX = EffectVar(0,pTarget,iNumber), iY = EffectVar(1,pTarget,iNumber), iObj;
  if(iObj = EffectVar(2,pTarget,iNumber)) {
    if(Distance(GetX(iObj),GetY(iObj),iX,iY) > 30) EffectVar(2,pTarget,iNumber) = 0;
    else return(0);
  }
  var aCreate = [_BOK,_BOK,_BOK,_BOK,ALC_,ALC_], iID;
  if(!FindObject2(Find_ID(CRWN),                       Find_Distance(310,1030,750),Find_NoContainer()))
    aCreate[GetLength(aCreate)] = CRWN;
  if(!FindObject2(Find_Or(Find_ID(GBLT),Find_ID(EGBL)),Find_Distance(310,1030,750),Find_NoContainer()))
    aCreate[GetLength(aCreate)] = EGBL;
  iID = aCreate[Random(GetLength(aCreate))];
  if(iID == ALC_) EffectVar(2,pTarget,iNumber) = CreateAlchemBag( iX,iY,-1);
  else            EffectVar(2,pTarget,iNumber) = CreateObject(iID,iX,iY,-1);
  if(EffectVar(2,pTarget,iNumber)) CastObjects(SPRK,10,20,iX,iY);
  return(1);
}

global func CreateAlchemBag(int iX, int iY, int iOwner)
{
  var i, iID, iObj = CreateObject(ALC_, iX, iY, iOwner);
  for(i = RandomX(5, 10); i > 0; i--) {
    iID = [IBLO,IBON,IHAI,ILOA,IPOI,IROC][Random(6)];
    SetComponent(iID,GetComponent(iID,0,iObj)+1,iObj);
  }
  return(iObj);
}