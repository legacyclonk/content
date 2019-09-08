/*-- Bankraub --*/

#strict

static relleft, relright;
static killleft, killright;

protected func Initialize()
{
 // Relaunchanzahl - Banditen 4, Stadt 7
 relleft=6;
 relright=4;
     
 // Regeln erschaffen
 if(!FindObject(TKNW)) CreateObject(TKNW, 100, 100); // Teambaupläne
 
 // Ziel erschaffen
 if (!FindObject(MELE)) CreateObject(MELE, 100, 100);  //Teammelee
 if (!FindObject(BNKR)) CreateObject(BNKR, 100, 100);  //Bankraub
 
 if (!FindObject(_TAS)) CreateObject(_TAS, 100, 100);  // Team
     
 // Fahrstühle aufbauen und Schächte graben
 var ELEV1 = CreateConstruction(ELEV, 421, 411, NO_OWNER, 100, 1);
 var ELEV2 = CreateConstruction(ELEV, 1981, 140, NO_OWNER, 100, 1);
 var ELEV3 = CreateConstruction(ELEV, 2012, 369, NO_OWNER, 100, 1);
 var ELEV4 = CreateConstruction(ELEV, 2452, 522, NO_OWNER, 100, 1);
 while(SetVar(0, FindObject(ELEV, 395, 340, 50, 70, 0, 0, 0, 0, Var())))
    ObjectCall(Var(), "CreateShaft", 190);
 while(SetVar(0, FindObject(ELEV, 1965, 80, 50, 70, 0, 0, 0, 0, Var())))
    ObjectCall(Var(), "CreateShaft", 200);
 while(SetVar(0, FindObject(ELEV, 1985, 300, 50, 70, 0, 0, 0, 0, Var())))
    ObjectCall(Var(), "CreateShaft", 280);
 while(SetVar(0, FindObject(ELEV, 2435, 462, 50, 70, 0, 0, 0, 0, Var())))
    ObjectCall(Var(), "CreateShaft", 184);
    
 // Tafeltexte
 SignpostText("SgnpLeft", "$SgnpMines$");
 SignpostText("SgnpRight", "$SgnpFort$");
 
 // Den Banditen Startpferde geben
 var obj = CreateObject(HRSS, 454, 620, -1);
 LocalN("iGraphics", obj) = 1; 
 SetGraphics("1", obj);
 SetDir(1, obj);
 obj = CreateObject(HRSS, 468, 391, -1);
 LocalN("iGraphics", obj) = 1; 
 SetGraphics("1", obj);
 SetDir(1, obj);
 
 // Amboss erzeugen
 FindObject(WTNT)->Initialize();

 // Fundamente der Banken unzerstörbar
 AddEffect("NoDamage", FindObject(BAS9, 1820, 372, -1, -1), 1);
 AddEffect("NoDamage", FindObject(BAS9, 2166, 371, -1, -1), 1);
 AddEffect("NoDamage", FindObject(BAS9, 2291, 531, -1, -1), 1);

 // Scoreboard erzeugen
 SetScoreboardData(SBRD_Caption,	SBRD_Caption,		"Spielstand",		SBRD_Caption);
 SetScoreboardData(SBRD_Caption,	ScoreboardCol(GOLD),	"{{WLTH}}",		ScoreboardCol(GOLD));
 SetScoreboardData(SBRD_Caption,	ScoreboardCol(RELA),	"{{BNDT}}{{COWB}}",	ScoreboardCol(RELA));
 SetScoreboardData(SBRD_Caption,	ScoreboardCol(KILL),	"{{REVR}}",		ScoreboardCol(KILL));  
 SetScoreboardData(ScoreboardCol(BNDT),	SBRD_Caption,		"$MsgBandits$",		ScoreboardCol(BNDT));
 SetScoreboardData(ScoreboardCol(COWB),	SBRD_Caption,		"$MsgTown$",		ScoreboardCol(COWB));
}

func GetTeamName(iTeam)
{
  if(iTeam==1) return("$MsgBandits$");
  if(iTeam==2) return("$MsgTown$");
}

global func ActualizeScoreboard(fNoShow)
{
 SetScoreboardData(ScoreboardCol(BNDT),       ScoreboardCol(GOLD),       Format("%d", GetWealth(GetPlrByTeam(1))), GetWealth(GetPlrByTeam(1)));
 SetScoreboardData(ScoreboardCol(COWB),       ScoreboardCol(GOLD),       Format("%d", GetWealth(GetPlrByTeam(2))), GetWealth(GetPlrByTeam(2)));
 SetScoreboardData(ScoreboardCol(BNDT),       ScoreboardCol(RELA),       Format("%d", relleft), relleft);
 SetScoreboardData(ScoreboardCol(COWB),       ScoreboardCol(RELA),       Format("%d", relright), relright);
 SetScoreboardData(ScoreboardCol(BNDT),       ScoreboardCol(KILL),       Format("%d", killleft), killleft);
 SetScoreboardData(ScoreboardCol(COWB),       ScoreboardCol(KILL),       Format("%d", killright), killright); 
 if(!fNoShow) DoScoreboardShowAll(1, 5);
 if(GetPlrByTeam(1)<0) return();
 if(GetPlrByTeam(2)<0) return();
 if(GetWealth(GetPlrByTeam(1))>GetWealth(GetPlrByTeam(2))) Team2Lose();
 if(!ObjectCount(MTNT, 0,0,0,0, OCF_Fullcon()) && !ObjectCount(WTNT, 0,0,0,0, OCF_Fullcon())) Team1Lose();
 if(relleft<=0) if(!FindFreeBandits()) Team1Lose();
}

global func DoScoreboardShowAll(iAmount, iTime)
{
    DoScoreboardShow(iAmount);
    if(iTime) Schedule(Format("DoScoreboardShow(-%d)", iAmount), 38*iTime);
}

global func FindFreeBandits()
{
  var bandit = false;
  for(var i=0,j ; i < GetPlayerCount() ; i++)
   if(GetPlayerByIndex(i) > -1)
    {
    if(CheckTeam(i)==1)
      for(j=0 ; j < GetCrewCount(i) ; j++)
        {
        if(!Contained(GetCrew(i,j)))
          bandit = true;
        else
          if(! (Contained(GetCrew(i,j))->~IsPrison()))
            bandit = true;
        }
    }
  return(bandit);
}

protected func InitializePlayer(int player, int tx, int ty, object pBase, int iTeam)
{
//  CreateHUD(player);
  CreateObject(AHUD, 0, 0, player);
  // Den Spieler ausrüsten und platzieren
  CreateObject(_PLO, LandscapeWidth() / 2, 50, player)->InitPlayer(player, iTeam);
  CreateObject(_CTD, 0, 0, player);
  var obj, i;
  return(1);
}

protected func RemovePlayer(int player)
{
 // Spielervariable zurücksetzen, damit Laufzeitbeitritt mit Aufgeben funktioniert
 Global(player) = 0;
 RemoveObject(FindObjectOwner(_CTD, player));
 return(1);
}

protected func RelaunchPlayer(int player)
{
 // Den Tod des Spielers verkünden
 var team=CheckTeam(player);
 
 if(team==1)
  {
   Log("$MsgGangLost$", GetPlayerName(player));
   Sound("Trumpet");
   relleft--;
   if(relleft < 0)
    {
     EliminatePlayer(player);
     Log("$MsgEliminate$",GetPlayerName(player));
     return(1);
    }
   if(relleft==1) { Log("$MsgRelaunchBAND1$"); }
   else Log("$MsgRelaunchBAND$",relleft);
  }
 
 else
  {
   Log("$MsgCowbLost$", GetPlayerName(player));
   Sound("Trumpet");
   relright--;
   if(relright < 0)
    {
     EliminatePlayer(player);
     Log("$MsgEliminate$",GetPlayerName(player));
     return(1);
    }
   if(relright==1) { Log("$MsgRelaunchCOWB1$"); }
   else Log("$MsgRelaunchCOWB$",relright);
  }

 // Der Spieler muss noch etwas warten
 var pRelauncher = CreateObject(_PLO, LandscapeWidth() / 2, 50, player);
 
 // Dem Spieler vier neue Clonks geben
 var id = COWB;
 if(CheckTeam(player)==1) id = BNDT;
 var iFirst;
 for (var i; i < 4; i++)
 {
  var caval = CreateObject(id, 0,0, player);
  if(!i) iFirst = caval;
  MakeCrewMember(caval, player);
  DoEnergy(100-GetEnergy(caval), caval);
  Enter(pRelauncher,caval);
 }
 if(id==BNDT) {
 CreateContents(AMBO, iFirst);
 CreateContents(DYNB, iFirst);
 CreateContents(WINC, iFirst);
 }
 UnselectCrew(player);
 SetCursor(player, iFirst);

 pRelauncher->RelaunchPlayer(player);

 // Scoreboard aktualisieren
 ActualizeScoreboard();
 return(1);
}

protected func BanditCaught(iPlr)
{
  if(!FindFreeBanditPlayer(iPlr))
    RelaunchPlayer(iPlr,NO_OWNER);
}

global func FindFreeBanditPlayer(iPlr)
{
  var bandit = 0;// = false;
  for(var j=0 ; j < GetCrewCount(iPlr) ; j++)
  {
    if(!Contained(GetCrew(iPlr,j)))
      bandit++;// = true;
    else
      if(! (Contained(GetCrew(iPlr,j))->~IsPrison()))
       bandit++;// = true;
  }
  return(bandit);
}

global func SetWealth (int iPlr, int iValue)
{
  var iRes = _inherited(iPlr, iValue);
  Schedule("ActualizeScoreboard()", 1);
  return(iRes);
}

global func Sell (int iPlr, object pObj)
{
  var iRes = _inherited(iPlr, pObj);
  Schedule("ActualizeScoreboard()", 1);
  return(iRes);
}

global func Team1Lose()
{
   Log("$MsgCowbsWin$");
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
   Log("$MsgBanditsWin$");
   var winnerTeam = 1; 
// ...und die Verlierer eliminieren
  for (var i = GetPlayerCount(); i--;)
   {
  if (CheckTeam(GetPlayerByIndex(i)) != winnerTeam)
  EliminatePlayer(GetPlayerByIndex(i));
   }
}

protected func TimeUp()
{
   Log("Die Kavallerie kommt!");
   Sound("Trumpet");
   Schedule("Sound(\"Trumpet\")", 1, 1);
   var pTower;
   // Tore öffnen
   if(pTower = FindObject(CTWR, 2379, 310))
   {
     if(!pTower->RightGateOpen()) pTower->ControlRight();
     if(!pTower->LeftGateOpen()) pTower->ControlLeft();
   }
   ScriptGo(1);
   SetPlrKnowledge(GetPlrByTeam(2, 0),CTWR);
   SetPlrKnowledge(GetPlrByTeam(2, 0),HEDQ);
   SetPlrKnowledge(GetPlrByTeam(2, 0),AMRY);
   SetPlrKnowledge(GetPlrByTeam(2, 0),DYNB);
   SetPlrKnowledge(GetPlrByTeam(2, 0),IGNB);
   SetPlrKnowledge(GetPlrByTeam(2, 0),SABR);
   DoHomebaseMaterial(GetPlrByTeam(2, 0), SABR,10);
   DoHomebaseMaterial(GetPlrByTeam(2, 0), CVRM, 5);
}

protected func Script1()
{
  AddEffect("CVRMHorseArrive", 0, 1, 1, 0, 0, GetPlrByTeam(2, 0), 2125); 
}

protected func Script4()
{
  AddEffect("CVRMWagonArrive", 0, 1, 1, 0, 0, GetPlrByTeam(2,3), CCAN, 2164);
}

protected func Script8()
{
  AddEffect("CVRMWagonArrive", 0, 1, 1, 0, 0, GetPlrByTeam(2, 2), CCAN, 2210);
}

protected func Script12()
{
  AddEffect("CVRMHorseArrive", 0, 1, 1, 0, 0, GetPlrByTeam(2,4), 2251); 
}

protected func Script16()
{
  AddEffect("CVRMHorseArrive", 0, 1, 1, 0, 0, GetPlrByTeam(2,5), 2295);
}

protected func Script20()
{ 
  AddEffect("CVRMWagonArrive", 0, 1, 1, 0, 0, GetPlrByTeam(2, 1), COAC, 2340);
  ScriptGo(0);
}

// Callbacks aus dem PlayerObject
func JoinTeam(iPlr, iTeam)
{
  // Teamkonto updaten
  FindObjectOwner(ACNT,iPlr)->~TeamChanged();
  if(iTeam==1)
  {
    while(FindObjectOwner(COWB, iPlr)) RedefineCowboy(BNDT, FindObjectOwner(COWB, iPlr));
    CreateContents(AMBO, GetHiRank(iPlr));
    CreateContents(DYNB, GetHiRank(iPlr));
    CreateContents(WINC, GetHiRank(iPlr));
    ExtraLog("$MsgBanditsJoin$", GetPlayerName(iPlr));
  }
  if(iTeam==2) ExtraLog("$MsgCowboysJoin$", GetPlayerName(iPlr));
}

func GetTeamStartPosition(iTeam, fY)
{
  if(iTeam==1) { if(fY) return(628); return(RandomX( 500, 590)); }
  if(iTeam==2) { if(fY) return(135); return(RandomX(1880,1940)); }
}

func InitializeTeam(iTeam, iPlr)
{
  if(iTeam==1)
  {
    AcquireBase(iPlr, 200, 200, 650, 500);
    DoWealth(iPlr, 150);
    SetPlrKnowledge(iPlr, TENP); // Zeltpack
    SetPlrKnowledge(iPlr, WGTW); // Wachturm
  }
  if(iTeam==2)
  {
    AcquireBase(iPlr, 1560, 0, 840, 600);
    DoWealth(iPlr, 600);
    SetPlrKnowledge(iPlr,CABL); // Kanonenkugel
    SetPlrKnowledge(iPlr,CCAN); // Kanone
    SetPlrKnowledge(iPlr,POS1); // Fahne
    SetPlrKnowledge(iPlr, SHOF); // Sheriffsbüro
    SetPlrKnowledge(iPlr, SALO); // Saloon
    SetPlrKnowledge(iPlr, DRST); // Drugstore
    SetPlrKnowledge(iPlr, WRKW); // Werkstatt
    DoHomebaseMaterial(iPlr, COWB, 4); // Cowboys kaufbar
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

global func FxCVRMHorseArriveStart(pTarget, iNumber, fTmp, iPlr, iDest)
{
  if(fTmp) return(0);   
  EffectVar(1, pTarget, iNumber) = iPlr;
  EffectVar(2, pTarget, iNumber) = CreateObject(HRSS, 2473, 360,iPlr); // gesatteltes Pferd
  EffectVar(3, pTarget, iNumber) = CreateObject(CVRM, 2473, 342,iPlr); // Reiter
  EffectVar(4, pTarget, iNumber) = 50*1000;
  EffectVar(7, pTarget, iNumber) = iDest;
  SetObjectLayer(EffectVar(2, pTarget, iNumber), EffectVar(2, pTarget, iNumber));
  SetObjectLayer(EffectVar(2, pTarget, iNumber), EffectVar(3, pTarget, iNumber));

  SetDir(0, EffectVar(2, pTarget, iNumber));
  SetDir(0, EffectVar(3, pTarget, iNumber));

  MakeCrewMember(EffectVar(3, pTarget, iNumber), iPlr);
  SetCrewEnabled(0, EffectVar(3, pTarget, iNumber));
  
  SetGraphics("", EffectVar(2, pTarget, iNumber), HRSS);
  LocalN("iGraphics", EffectVar(2, pTarget, iNumber)) = 0; 
  
  ObjectSetAction(EffectVar(3, pTarget, iNumber), "Ride", EffectVar(2, pTarget, iNumber));
  EffectVar(2, pTarget, iNumber)->SetObjDrawTransform(1000, 0, 50*1000, 0, 1000); 
  EffectVar(3, pTarget, iNumber)->SetObjDrawTransform(1000, 0, 50*1000, 0, 1000); 

  // Inhalt Reiter
  CreateContents(AMBO, EffectVar(3, pTarget, iNumber));
  CreateContents(AMBO, EffectVar(3, pTarget, iNumber));
  CreateContents(SABR, EffectVar(3, pTarget, iNumber));
  // volle Waffen geben
  var pWeap = CreateContents(REVR, EffectVar(3, pTarget, iNumber));
  for(var i=0;i<6;i++) CreateContents(CART, pWeap);
  pWeap = CreateContents(WINC, EffectVar(3, pTarget, iNumber));
  for(var i=0;i<6;i++) CreateContents(CART, pWeap);
  // Inhalt Pferd
  CreateContents(AMBO, EffectVar(2, pTarget, iNumber));
  CreateContents(AMBO, EffectVar(2, pTarget, iNumber));
  // Weg frei räumen
  FreeRect(2403,309,500,47);
}

global func FxCVRMHorseArriveTimer(pTarget, iNumber)
{
  // Variablen hohlen   
  var pHorse = EffectVar(2, pTarget, iNumber), 
      pRider = EffectVar(3, pTarget, iNumber),
      iTime = EffectVar(0, pTarget, iNumber),
      iState = EffectVar(5, pTarget, iNumber),
      iShift = EffectVar(4, pTarget, iNumber),
      iPlr = EffectVar(1, pTarget, iNumber),
      iDest = EffectVar(7, pTarget, iNumber);
  if(!pRider) return(-1);
  if(!pHorse) return(-1);
//  Message("%d", pHorse, iState);
  if(iState==0)
  {
    if(EffectVar(4, pTarget, iNumber)<0) 
    {
      EffectVar(5, pTarget, iNumber)++;
      SetPosition(2473, 340, pHorse);
      ObjectSetAction(pHorse, "Walk");
      SetPhase(Mod(iTime, 20), pHorse);
      SetCommand(pHorse, "Call", pHorse, 0, 0, 0, "StopMe");
      AddCommand(pHorse, "MoveTo", 0, iDest,340);//2246
      SetCrewEnabled(1, pRider);
      SetObjectLayer(0, pRider);
      SetObjectLayer(0, pHorse);
      return();
    }
    iTime = EffectVar(0, pTarget, iNumber)++;
    SetPosition(2473, 340, pHorse);
    pHorse->~SetGait(1);
    ObjectSetAction(pHorse, "Walk");
    SetPhase(Mod(iTime, 20), pHorse);
    EffectVar(4, pTarget, iNumber) -= 1500;
    pHorse->SetObjDrawTransform(1000, 0, Max(iShift,0), 0, 1000); 
    pRider->SetObjDrawTransform(1000, 0, Max(iShift,0), 0, 1000);   
    return();
  }
  if(iState==1)
  {
    if(!GetCommand(pHorse)) return(-1);
    if(Distance(GetX(pHorse), GetY(pHorse), iDest,340)<20)
    {
      pHorse->~SetGait();
      SetCommand(pHorse, "None");
      return(-1);
    }
    if(GetDir(pHorse)) { pHorse->~SetGait(); SetDir(0, pHorse); SetCommand(pHorse, "None"); return(-1); }
  }
}

global func StopMe(pObj)
{
  pObj->~SetGait(0);    
}

global func FxCVRMWagonArriveStart(pTarget, iNumber, fTmp, iPlr, idWagon, iDest)
{
  if(fTmp) return(0);   
  EffectVar(6, pTarget, iNumber) = idWagon;
  EffectVar(7, pTarget, iNumber) = iPlr;
  EffectVar(1, pTarget, iNumber) = 0;
  EffectVar(2, pTarget, iNumber) = CreateObject(idWagon, 2475, 360,iPlr); // Planwagen
  EffectVar(3, pTarget, iNumber) = CreateObject(HRSS, 2473, 360,iPlr); // gesatteltes Pferd
  EffectVar(4, pTarget, iNumber) = CreateObject(CVRM, 2473, 360,iPlr); // Reiter
  EffectVar(5, pTarget, iNumber) = 100*1000;
  EffectVar(8, pTarget, iNumber) = iDest;
  SetObjectLayer(EffectVar(2, pTarget, iNumber), EffectVar(2, pTarget, iNumber));
  SetObjectLayer(EffectVar(2, pTarget, iNumber), EffectVar(3, pTarget, iNumber));
  SetObjectLayer(EffectVar(2, pTarget, iNumber), EffectVar(4, pTarget, iNumber));

SetDir(0, EffectVar(2, pTarget, iNumber));
SetDir(0, EffectVar(3, pTarget, iNumber));
SetDir(0, EffectVar(4, pTarget, iNumber));

  MakeCrewMember(EffectVar(4, pTarget, iNumber), iPlr);
  SetCrewEnabled(0, EffectVar(3, pTarget, iNumber));

SetGraphics("", EffectVar(3, pTarget, iNumber), HRSS);
LocalN("iGraphics", EffectVar(3, pTarget, iNumber)) = 0; 

ObjectSetAction(EffectVar(4, pTarget, iNumber), "Ride", EffectVar(3, pTarget, iNumber));
EffectVar(3, pTarget, iNumber)->~ConnectWagon(EffectVar(2, pTarget, iNumber));

 EffectVar(2, pTarget, iNumber)->SetObjDrawTransform(1000, 0, 100*1000, 0, 1000); 
 EffectVar(3, pTarget, iNumber)->SetObjDrawTransform(1000, 0, 50*1000, 0, 1000); 
 EffectVar(4, pTarget, iNumber)->SetObjDrawTransform(1000, 0, 50*1000, 0, 1000); 

  // Inhalt Reiter
  CreateContents(AMBO, EffectVar(4, pTarget, iNumber));
  CreateContents(AMBO, EffectVar(4, pTarget, iNumber));
  CreateContents(SABR, EffectVar(4, pTarget, iNumber));
  // volle Waffen geben
  var pWeap = CreateContents(REVR, EffectVar(4, pTarget, iNumber));
  for(var i=0;i<6;i++) CreateContents(CART, pWeap);
  pWeap = CreateContents(WINC, EffectVar(4, pTarget, iNumber));
  for(var i=0;i<6;i++) CreateContents(CART, pWeap);
  // Inhalt Pferd
  CreateContents(AMBO, EffectVar(3, pTarget, iNumber));
  CreateContents(AMBO, EffectVar(3, pTarget, iNumber));
  // Inhalt Planwagen
  if(idWagon==COAC) 
  {
    for(var i=0;i<8;i++)      
      CreateContents(DYNB, EffectVar(2, pTarget, iNumber));
    for(var i=0;i<2;i++)
      CreateContents(IGNB, EffectVar(2, pTarget, iNumber));
    for(var i=0;i<15;i++)
    {
      CreateContents(CABL, EffectVar(2, pTarget, iNumber));
      CreateContents(GUNP, EffectVar(2, pTarget, iNumber));
    }
    for(var i=0;i<10;i++)
    CreateContents(AMBO, EffectVar(2, pTarget, iNumber));
  }
  else // Inhalt Kanone
  {
    for(var i=0;i<4;i++)
    {
      CreateContents(CABL, EffectVar(2, pTarget, iNumber));
      CreateContents(GUNP, EffectVar(2, pTarget, iNumber));
    }
  }
  // Weg frei räumen
  FreeRect(2403,309,500,47);
}

global func FxCVRMWagonArriveTimer(pTarget, iNumber)
{
  // Variablen hohlen   
  var pCoach = EffectVar(2, pTarget, iNumber), 
      pHorse = EffectVar(3, pTarget, iNumber), 
      pRider = EffectVar(4, pTarget, iNumber),
      iTime = EffectVar(0, pTarget, iNumber),
      iState = EffectVar(1, pTarget, iNumber),
      iShift = EffectVar(5, pTarget, iNumber),
      idWagon = EffectVar(6, pTarget, iNumber),
      iPlr = EffectVar(7, pTarget, iNumber),
      iDest = EffectVar(8, pTarget, iNumber);
  if(!pHorse) return(-1); 
  if(iState==0)
  {
    if(EffectVar(5, pTarget, iNumber)<=50*1000) 
    {
      iTime = EffectVar(0, pTarget, iNumber)++;
      EffectVar(1, pTarget, iNumber)++;
      SetPosition(2473, 340, pCoach);
      SetPosition(2473, 340, pHorse);
      if(idWagon==COAC) ObjectSetAction(pCoach, "Drive2");
      else ObjectSetAction(pCoach, "Drive1");
      ObjectSetAction(pHorse, "Pull");
      SetPhase(Mod(iTime, 20), pHorse);
      if(idWagon==COAC) SetPhase(Mod(iTime, 4), pCoach);
      else SetPhase(Mod(iTime, 6), pCoach);
      EffectVar(5, pTarget, iNumber) -= 1000;
      pCoach->SetObjDrawTransform(1000, 0, iShift, 0, 1000); 
      SetCommand(pHorse, "MoveTo", 0, iDest,350);//2132
      return();
    }
    iTime = EffectVar(0, pTarget, iNumber)++;
    SetPosition(2475, 340, pCoach);
    SetPosition(2473, 340, pHorse);
    SetDir(0, pRider);
    if(idWagon==COAC) ObjectSetAction(pCoach, "Drive2");
    else ObjectSetAction(pCoach, "Drive1");
    ObjectSetAction(pHorse, "Pull");
    SetPhase(Mod(iTime, 20), pHorse);
    if(idWagon==COAC) SetPhase(Mod(iTime, 4), pCoach);
    else SetPhase(Mod(iTime, 6), pCoach);
    EffectVar(5, pTarget, iNumber) -= 1500;
    pCoach->SetObjDrawTransform(1000, 0, +iShift, 0, 1000); 
    pHorse->SetObjDrawTransform(1000, 0, Max(+iShift-50*1000,0), 0, 1000); 
    pRider->SetObjDrawTransform(1000, 0, Max(+iShift-50*1000,0), 0, 1000);   
    return();
  }
  else if(iState==1)
  {
    if(Distance(GetX(pHorse), GetY(pHorse), iDest,350)<20)
    {
      pHorse->~SetGait();
      SetCommand(pHorse, "None");
    }
    if(GetDir(pHorse)) { pHorse->~SetGait(); SetDir(0, pHorse); SetCommand(pHorse, "None"); }
    if(!GetCommand(pHorse) && !EffectVar(5, pTarget, iNumber)) return(-1);
    if(EffectVar(5, pTarget, iNumber))
    {
    iTime = EffectVar(0, pTarget, iNumber)++;
    if(idWagon==COAC) SetPosition(2475, 340, pCoach);
    else SetPosition(2475, 345, pCoach);
    if(idWagon==COAC) ObjectSetAction(pCoach, "Drive2");
    else ObjectSetAction(pCoach, "Drive1");
    if(idWagon==COAC) SetPhase(Mod(iTime, 4), pCoach);
    else SetPhase(Mod(iTime, 6), pCoach);
    EffectVar(5, pTarget, iNumber) -= 1500;
    pCoach->SetObjDrawTransform(1000, 0, Max(iShift,0), 0, 1000);   
    }
    else 
    { 
      SetCrewEnabled(1, pRider); 
      SetObjectLayer(0, pCoach);
      SetObjectLayer(0, pRider);
      SetObjectLayer(0, pHorse);
    }
    return();
  }
}

global func FxNoDamageDamage() { return(0); }
