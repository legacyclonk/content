/*-- Goldrausch --*/


#strict

static pWagon, pHorse;
static fInitialized;
static iDifficulty;
static fUseHungerThirst;

static const fUseHunger = 1;
static const fUseThrist = 1;

static const fTestSection = 0;

static const fTesting = 0;

global func DoInitialize() {
  if(fInitialized) return();
  fInitialized = 1; 
  fUseHungerThirst = 1;

  if(!FindObject(NOPC)) CreateObject(NOPC);

  if(fUseHungerThirst) if (!ObjectCount(NDWA)) CreateObject(NDWA, 50,50, -1); // Durst
  if(fUseHungerThirst) if (!ObjectCount(NDFO)) CreateObject(NDFO, 50,50, -1); // Hunger

  if(FindObject(_ETG)) RemoveObject(FindObject(_ETG));
  if(FindObject(REPR)) RemoveObject(FindObject(REPR)); CreateObject(REPR, 50,50, -1); // Nachwuchs
  if(FindObject(TREP)) RemoveObject(FindObject(TREP)); CreateObject(TREP, 50,50, -1); // Baumnachwuchs
  
  // NPC's sollen Position halten
  var i, pObj;
  while(pObj = FindObjectOwner(0,-1,0,0,0,0,OCF_CrewMember,0,0,pObj)) 
    AddEffect("StayThere",pObj,1,35,pObj);
  
//  if (!GetPlayerCount()) return();
  // Banditen erstellen 
  pObj = CreateObject(BNDT, 3917, 570, -1);
  pObj->SetAI("BanditNoMove", 3);
  pObj->SetColorDw(RGB(150));
  pObj = CreateObject(BNDT, 4120, 560, -1);
  pObj->SetAI("BanditNoMove", 3);
  pObj->SetColorDw(RGB(150));
  pObj = CreateObject(BNDT, 4175, 520, -1);
  pObj->SetAI("BanditNoMove", 3);
  pObj->SetColorDw(RGB(150));
  pObj = CreateObject(BNDT, 4044, 477, -1);
  pObj->SetAI("BanditNoMove", 3);
  pObj->SetColorDw(RGB(150));
  pObj = CreateObject(BNDT, 4309, 471, -1);
  pObj->SetAI("BanditBoss", 3);
  pObj->SetColorDw(RGB(150));
  pObj->MakeBoss();
  pObj->Enter(FindObject(MTNT, 4309, 471, -1, -1));
  // Startmaterial erstellen
  pWagon = CreateObject(COAC, 28, 270, -1);
  pHorse = CreateObject(HORS, 77, 270, -1);
  pHorse->ConnectWagon(pWagon);
  SetDir(1, pWagon);
  SetDir(1, pHorse);

  // Startmaterial
  CreateContents(MEAC,pWagon,6);
  CreateContents(WHKY,pWagon,3);
  for(i=0;i<2;i++) {
    pObj = CreateContents(WSKI, pWagon);
    LocalN("iWater", pObj) = 90;
    pObj->~UpdateGraphics();
  }
  CreateContents(MLLT,pWagon);
  CreateContents(KNFE,pWagon);
  CreateContents(IGNB,pWagon);
  CreateContents(DYNB,pWagon,2);
  CreateContents(METL,pWagon,7);
  CreateContents(WOOD,pWagon,21);
  CreateContents(CNKT,pWagon,4);
  CreateContents(LNKT,pWagon);
  
  // Höhle muss immer vom  Indianer zu betreten sein!
  FreeRect(1970,589,40,9);
  
  // Brauchbare Gebäude mit unzerstörbarkeit belegen
  for(pObj in FindObjects(Find_Category(C4D_StaticBack|C4D_Structure|C4D_Vehicle),Find_Not(Find_Category(32768)),Find_InRect(1130,190,300,140)))
    AddEffect("NoDmg",pObj,200);
  for(pObj in FindObjects(Find_Category(C4D_StaticBack|C4D_Structure|C4D_Vehicle),Find_Func("IsIndianHandcraft"),
                          Find_Or(Find_InRect(1830,230,560,340),Find_InRect(3190,160,670,360))))
    AddEffect("NoDmg",pObj,200);

  // Weil die Kanone hier bearbeitet worden ist
  if(pObj = FindObject(CCAN)) pObj->~Initialize();

  if(fTestSection==1) return(ChangeSection("AshCity"));
  if(fTestSection==2) return(ChangeSection("Caves")); 
  
  // ... und los!
  if(!fTesting) ScriptGo(1);
  return(1);
}

global func DoInitializeSection()
{
  if(g_iSection) if(fInitialized>g_iSection) return();
  fInitialized++;
  if(g_iSection==1)
  {
    if(!FindObject(NOPC)) CreateObject(NOPC);
    
    if(fUseHungerThirst) if (!ObjectCount(NDWA)) CreateObject(NDWA, 50,50, -1); // Durst
    if(fUseHungerThirst) if (!ObjectCount(NDFO)) CreateObject(NDFO, 50,50, -1); // Hunger

    if(FindObject(_ETG)) RemoveObject(FindObject(_ETG));
    if(FindObject(REPR)) RemoveObject(FindObject(REPR)); CreateObject(REPR, 50,50, -1); // Nachwuchs
    if(FindObject(TREP)) RemoveObject(FindObject(TREP)); CreateObject(TREP, 50,50, -1); // Baumnachwuchs

    // NPC's sollen Position halten
    var obj;
    while(obj = FindObjectOwner(0, -1, 0, 0, 0, 0, OCF_CrewMember(), 0, 0, obj)) 
      AddEffect("StayThere", obj, 1, 35, obj);

    // Energiebedarf erst verzögert
    if(FindObject(ENRG)) RemoveObject(FindObject(ENRG));
    Schedule("CreateObject(ENRG, 100, 100)", 500);

    // Aufzüge fixen
    FixElevator(FindObject(ELEV,1299, 292),176);
    FixElevator(FindObject(ELVB,1520,  71),165);
    FixElevator(FindObject(ELVB,1710, 241),150);
    FixElevator(FindObject(ELEV,2018,  81),209);
    FixElevator(FindObject(ELVB,2360, 291),260);

    // Banditen erzeugen
    var pBandit = CreateObject(BNDT, 1044, 260, -1);
    pBandit->SetAI("BanditNoMove", 3);
    pBandit->SetColorDw(RGB(150));
    pBandit = CreateObject(INDI, 1103, 98, -1);
    pBandit->SetAI("BanditNoMove", 3);
    pBandit->SetColorDw(RGB(150));
    pBandit = CreateObject(BNDT, 1450, 298, -1);
    pBandit->SetAI("BanditRevolver", 3);
    pBandit->SetColorDw(RGB(150));
    pBandit = CreateObject(BNDT, 1838, 259, -1);
    pBandit->SetAI("BanditRevolver", 3);
    pBandit->SetColorDw(RGB(150));
    pBandit = CreateObject(BNDT, 1966, 190, -1);
    pBandit->SetAI("BanditNoMove", 3);
    pBandit->SetColorDw(RGB(150));
    pBandit = CreateObject(BNDT, 1634,  87, -1);
    pBandit->SetAI("BanditNoMove", 3);
    pBandit->SetColorDw(RGB(150));
    if(iDifficulty>1)
    {
      pBandit = CreateObject(BNDT, 1044, 318, -1);
      pBandit->SetAI("BanditNoMove", 3);
      pBandit->SetColorDw(RGB(150));
      pBandit = CreateObject(BNDT, 1508, 259, -1);
      pBandit->SetAI("BanditNoMove", 3);
      pBandit->SetColorDw(RGB(150));
      pBandit = CreateObject(BNDT, 1884, 79, -1);
      pBandit->SetAI("BanditNoMove", 3);
      pBandit->SetColorDw(RGB(150));
//      pBandit = CreateObject(BNDT, 1627, 90, -1);
//      pBandit->SetAI("BanditNoMove", 3);
//      pBandit->SetColorDw(RGB(150));
      pBandit = CreateObject(BNDT, 2188, 309, -1);
      pBandit->SetAI("BanditRevolver", 3);
      pBandit->SetColorDw(RGB(150));
    }
  }
  if(g_iSection==2)
  {
    if(!FindObject(NOPC)) CreateObject(NOPC);
    
    if(fUseHungerThirst) if (!ObjectCount(NDWA)) CreateObject(NDWA, 50,50, -1); // Durst
    if(fUseHungerThirst) if (!ObjectCount(NDFO)) CreateObject(NDFO, 50,50, -1); // Hunger

    if(FindObject(_ETG)) RemoveObject(FindObject(_ETG));
    if(FindObject(REPR)) RemoveObject(FindObject(REPR)); CreateObject(REPR, 50,50, -1); // Nachwuchs
    if(FindObject(TREP)) RemoveObject(FindObject(TREP)); CreateObject(TREP, 50,50, -1); // Baumnachwuchs

    // NPC's sollen Position halten
    var obj;
    while(obj = FindObjectOwner(0, -1, 0, 0, 0, 0, OCF_CrewMember(), 0, 0, obj)) 
      AddEffect("StayThere", obj, 1, 35, obj);
    
    var obj;
    // Höhlenaufzüge brauchen keine Energie
    while( obj = FindObject(_CEL, 0, 0, 0, 0, 0, 0, 0, 0, obj))
	    AddEffect("NoEnergyNeed", obj, 1, 1);
    // Banditen erzeugen
    var pBandit = CreateObject(BNDT, 1378, 115, -1);
    pBandit->SetAI("BanditNoMove", 3);
    pBandit->SetColorDw(RGB(150));
    pBandit = CreateObject(BNDT, 1703, 265, -1);
    pBandit->SetAI("BanditNoMove", 3);
    pBandit->SetColorDw(RGB(150));
    pBandit = CreateObject(BNDT, 1621, 330, -1);
    pBandit->SetAI("BanditRevolver", 3);
    pBandit->SetColorDw(RGB(150));
    pBandit = CreateObject(BNDT, 1544, 169, -1);
    pBandit->SetAI("BanditNoMove", 3);
    pBandit->SetColorDw(RGB(150));
    pBandit = CreateObject(BNDT, 1904, 270, -1);
    pBandit->SetAI("BanditNoMove", 3);
    pBandit->SetColorDw(RGB(150));
    pBandit = CreateObject(BNDT, 1827, 320, -1);
    pBandit->SetAI("BanditNoMove", 3);
    pBandit->SetColorDw(RGB(150));
    pBandit = CreateObject(BNDT, 2062, 318, -1);
    pBandit->SetAI("BanditNoMove", 3);
    pBandit->SetColorDw(RGB(150));
    pBandit->MakeBoss();
    if(iDifficulty>1)
    {
      pBandit = CreateObject(BNDT, 1381, 170, -1);
      pBandit->SetAI("BanditNoMove", 3);
      pBandit->SetColorDw(RGB(150));
      pBandit = CreateObject(BNDT, 1703, 318, -1);
      pBandit->SetAI("BanditNoMove", 3);
      pBandit->SetColorDw(RGB(150));
      pBandit = CreateObject(BNDT, 1412, 310, -1);
      pBandit->SetAI("BanditNoMove", 3);
      pBandit->SetColorDw(RGB(150));
      pBandit = CreateObject(BNDT, 1622, 529, -1);
      pBandit->SetAI("BanditNoMove", 3);
      pBandit->SetColorDw(RGB(150));
      pBandit = CreateObject(BNDT, 1019, 220, -1);
      pBandit->SetAI("BanditNoMove", 3);
      pBandit->SetColorDw(RGB(150));
    }
  }
}

global func FixElevator(object elev, iDig)
  {
  var fBurned = GetID(elev)==ELVB;
  var x=GetX(elev), y=GetDefBottom(elev);
  elev->RemoveObject();
  elev = CreateConstruction(ELEV, x,y, -1, 100, true); // Fürs Fundament...
  elev->CreateShaft(iDig);
  AddEffect("NoEnergyNeed", elev, 1, 1);
  if(!fBurned) elev->GetActionTarget()->SetComDir(COMD_Down);
  else 
  { 
      RemoveObject(LocalN("pCase", elev)); 
      elev->ChangeDef(ELVB);
      SetCategory(1, elev);
      SetObjectLayer(elev, elev);
      SetCon(100, elev);
          AddEffect("Rebuild", elev, 1, 1);
  }
  }

protected func Script1()
{
  StartMovie("Intro", GetHiRank(GetPlayerByIndex()), GetCrew(GetPlayerByIndex(), 1));
  ScriptGo(0);
}

protected func InitializePlayer(int iPlr, int tx, int ty, object pBase, int iTeam)
  {
  DoInitialize();
  var i;
  while(GetCrew(iPlr,i))
    {
    Enter(pWagon, GetCrew(iPlr,i));
    ++i;
    }
/*  var pTrapper = FindObjectOwner(TRPR, iPlr);
  if(pTrapper)
  {
//    CreateContents(FIPL, pTrapper);
    CreateContents(MLLT, pTrapper);
    CreateContents(KNFE, pTrapper);
  }*/
  // Einer sollte den Wagen lenken
  if(!(pWagon->GetRider()))
  {
    //pWagon->ContainedUp(GetHiRank(iPlr));
    ObjectSetAction(GetHiRank(iPlr), "Ride", pWagon);
  }
  return(1);
  }

global func CreateDifficultyMenu()
{
  var pClonk = GetHiRank(GetPlayerByIndex());
  CreateMenu(WIPF,pClonk,pClonk,0,"$MnuChooseDifficulty$");
  AddMenuItem("$MnuLevelItmEasy$",  "SetDifficulty(0)",AMBO,pClonk,0,0,"$DescLevelEasy$");
  AddMenuItem("$MnuLevelItmNormal$","SetDifficulty(1)",REVR,pClonk,0,0,"$DescLevelNormal$");
  AddMenuItem("$MnuLevelItmHard$",  "SetDifficulty(2)",WINC,pClonk,0,0,"$DescLevelHard$");
  AddMenuItem("$MnuLevelItmExtrem$","SetDifficulty(3)",DYNB,pClonk,0,0,"$DescLevelExtrem$");
  SelectMenuItem(1,pClonk);
}

global func SetDifficulty(int iSetDifficulty)
  {
  // Schwierigkeitsstufe setzen
  iDifficulty = iSetDifficulty;

  // Bei leicht kein Nahrungsbedarf
  if(!iDifficulty) 
  {
  	fUseHungerThirst = 0;
  	if(FindObject(NDFO)) RemoveObject(FindObject(NDFO));
  	if(FindObject(NDWA)) RemoveObject(FindObject(NDWA));
  }

  // Message über dem Clonk ausgeben
  if (iSetDifficulty == 0) var strDifficulty = "$MnuLevelItmEasy$";
  if (iSetDifficulty == 1) var strDifficulty = "$MnuLevelItmNormal$";
  if (iSetDifficulty == 2) var strDifficulty = "$MnuLevelItmHard$";
  if (iSetDifficulty == 3) var strDifficulty = "$MnuLevelItmExtrem$";
  Message("%s", GetHiRank(0), strDifficulty);
  
  // Nachinitialisierung der Physicals schon erzeugter Gegner
  var o;
  while (o = FindObject(BNDT, 0,0,0,0, OCF_Alive, 0,0, 0, o))
  {
    o->~SetDifficultyPhysicals(iSetDifficulty);
  }

  // Und los geht's
//  ScriptGo(1);
  }

/* Szenenwechsel */

static g_szSection;
static g_iOldSection;
static g_iSection;
static g_iFarestSection;

// 
global func ChangeSection(szNewSection)
  {
  if (g_szSection == szNewSection) return();
  if(!g_szSection && szNewSection S="Main") return();
  g_szSection = szNewSection;
  g_iOldSection = g_iSection;
  if(szNewSection=="Main") g_iSection = 0;
  if(szNewSection=="AshCity") g_iSection = 1;
  if(szNewSection=="Caves") g_iSection = 2;
  if(g_iSection>g_iFarestSection) g_iFarestSection = g_iSection;
  Message("$MsgLoadingSect$");
  SetSkyAdjust(RGB(127,127,127));
  SetMatAdjust(RGB(127,127,127));
  // Los
  goto(99); ScriptGo(1);
  }

protected func Script100() { DoChangeSection(); }

global func DoChangeSection()
  {
  // Effekte löschen
  while(GetEffect("*")) RemoveEffect("*");
  // Objekte speichern
  SaveObjects();
  // Auf in die Berge! - ARG - MainSection muss gespeichert werden...
  if (!LoadScenarioSection(g_szSection, 3))
    return(Message("$MsgLoadingFail$"), Sound("Error", true), Schedule("GameOver()", 1));
  SetSkyAdjust(16777215); SetMatAdjust();
  // Alles wiederherstellen
  RestoreSectObjs();
  // An passend Position setzen
  AdjustSectPosition();
  DoInitializeSection();
  ScriptGo(0);
  }

global func GetSecCoachPosition(bool fY)
{
  if(g_iSection==0) { if(fY) return(302); return(4325); } // Indianerland
  if(g_iSection==1) {
    if(g_iSection>g_iOldSection) { if(fY) return(320); return(27); }
    else { if(fY) return(594); return(2902); }
  }
  if(g_iSection==2) { if(fY) return(320); return(27); }
}

global func AdjustSectPosition()
{
  // Wagen und Pferd
  var pCoach = FindObject(COAC);
  //pHorse = GetActionTarget(0, FindObject(CHBM,0,0,0,0,0,"Connect", pCoach));
  if(g_iSection==0) // Indianerland
  {   
    if(g_iSection>g_iOldSection) // von links
    {
       // wohl eher nicht so häufig..
    }
    else // von rechts
    {
       SetPosition(4325, 302, pCoach);
       SetPosition(4281, 302, pHorse);
       SetDir(0, pCoach); SetDir(0, pHorse);
    }
  }
  else if(g_iSection==1) // Ash City
  {
    if(g_iSection>g_iOldSection) // von links
    {
       SetPosition(27, 320, pCoach);
       SetPosition(78, 319, pHorse);
       SetDir(1, pCoach); SetDir(1, pHorse);
    }
    else // von rechts
    {
       SetPosition(2902, 594, pCoach);
       SetPosition(2847, 593, pHorse);
       SetDir(0, pCoach); SetDir(0, pHorse);
    }
  }
  else if(g_iSection==2) // Höhlen
  {
    if(g_iSection>g_iOldSection) // von links
    {
       SetPosition(27, 320, pCoach);
       SetPosition(78, 319, pHorse);
       SetDir(1, pCoach); SetDir(1, pHorse);
    }
    else // von rechts
    {
       // wohl eher nicht so häufig..
    }
  }
  if(pCoachRider) ObjectSetAction(pCoachRider, "Ride", pCoach);
  if(pHorse)
  {
    pHorse->ConnectWagon(pWagon);
    pHorse->SetGait(1);
    if(pHorseRider) ObjectSetAction(pHorseRider, "Ride", pHorse);
  }
}

static pCoachRider;
static pHorseRider;

global func SaveObjects()
{
  var obj;
  // Teamkonto speichern
  SaveSectObj(FindObject(TACC));
  while(obj = FindObject(ACNT)) SaveSectObj(obj);
  
  // Dialoge beenden
  while(obj = FindObject(_TLK, 0, 0, 0, 0, 0, 0, 0, obj))
  {
          obj->StopDialog();
  }
  // Wagen und Pferd
  var pCoach = FindObject(COAC);
  pHorse = GetActionTarget(0, FindObject(CHBM,0,0,0,0,0,"Connect", pCoach));
  // Reiter sichern
  pCoachRider = pCoach->GetRider();
  if(pHorse) pHorseRider = pHorse->GetRider();
  else pHorseRider = 0;
  // Pferde und Kanonen in der Nähe mitnehmen
  while(obj = pCoach->FindObject(CCAN, -100, -100, 200, 200)) 
    { SetPosition(GetSecCoachPosition(), GetSecCoachPosition(1), obj); SaveSectObj(obj); }
  while(obj = pCoach->FindObject(HORS, -100, -100, 200, 200))
    { SetPosition(GetSecCoachPosition(), GetSecCoachPosition(1), obj); SaveSectObj(obj); }
  while(obj = pCoach->FindObject(HRSS, -100, -100, 200, 200))
    { SetPosition(GetSecCoachPosition(), GetSecCoachPosition(1), obj); SaveSectObj(obj); }
  // Spieler sichern
  for(var i = 0; i < GetPlayerCount(); i++)
  {
    obj=0;
    while(obj = FindObjectOwner(AHUD, GetPlayerByIndex(i))) SaveSectObj(obj);
    while(obj = FindObjectOwner(0, GetPlayerByIndex(i), 0, 0, 0, 0, OCF_Alive | OCF_CrewMember , 0, 0))
    {       
//      DoEnergy(100, obj);
      Extinguish(obj);
      // Wenn er am Schild ist und kein Kutscher da ist, wird er neuer Kutscher
      if(GetAction(obj)S="Push" && GetID(GetActionTarget(0, obj)==SGNP)) 
          if(!pCoachRider) pCoachRider = obj;
      // Alle werden eingepackt
      Enter(pCoach, obj);
      ObjectSetAction(obj, "Walk");
      SaveSectObj(obj);
      SetPosition(100, 100);
    }
  }
  SaveSectObj(pCoach);
  if(pHorse) SaveSectObj(pHorse);
}

static g_iSaveSectIndex;

global func SaveSectObj(object obj)
  {
  // Inhalt sichern
  var o2,i=obj->ContentsCount(); while (o2=obj->Contents(--i)) SaveSectObj(o2);
  // Attachte Objekte sichern (Schilde, Amulette, ect...)
  o2 = 0; while (o2=FindObject(0, 0, 0, 0, 0, 0, 0, obj)) SaveSectObj(o2);
  // Objekt sichern
  (Global(g_iSaveSectIndex++) = obj)->SetObjectStatus(2);
  }

global func RestoreSectObjs()
  {
  var obj;
  // Alles gespeicherte wieder herstellen
  if (g_iSaveSectIndex)
    while (g_iSaveSectIndex--)
      if (obj=Global(g_iSaveSectIndex))
        { obj->SetObjectStatus(1); Global(g_iSaveSectIndex)=0; }
  g_iSaveSectIndex=0;
  }

// Objekte sortieren
global func ResortAll()
{
  var o;
  while(o = FindObject(0, 0, 0, 0, 0, 0, 0, 0, o)) o->Resort();
  return(1);
}

// // // // //
// Effekte  //
// // // // //

// Wiederaufbau //
global func FxRebuildStart(pTarget, iNumber, fTmp)
{
  if(fTmp) return();
  // Neues Gebäude erstellen
  EffectVar(0, pTarget, iNumber) = CreateObject(GetDefConstructTo(GetID(pTarget)), 
          GetX(pTarget), GetY(pTarget)-GetDefOffset(GetID(pTarget), 1));
  RemoveObject(LocalN("basement"), EffectVar(0, pTarget, iNumber));
  if(GetID(EffectVar(0, pTarget, iNumber))==ELEV) RemoveObject(LocalN("pCase", EffectVar(0, pTarget, iNumber)));
  if(GetID(EffectVar(0, pTarget, iNumber))==PUMW) pTarget->RemoveObject(FindObject(BARL, 0, 0, -1, -1));
  SetCon(1, EffectVar(0, pTarget, iNumber));
  // Kein Energiebedarf
  AddEffect("NoEnergyNeed", EffectVar(0, pTarget, iNumber), 1, 1);
}

global func FxRebuildTimer(pTarget, iNumber)
{
  if(GetCon(EffectVar(0, pTarget, iNumber))>=100) RemoveObject(pTarget);
}

global func FxRebuildDamage() { return(); }

// Kein Energiebedarf //
global func FxNoEnergyNeedTimer(pTarget, iNumber)
{
  DoEnergy(100, pTarget);
}

// KI: Rückkehr //
global func FxStayThereStart(pTarget, iNumber, fTmp)
{
  if(fTmp) return();
  EffectVar(0, pTarget, iNumber) = GetX(pTarget);
  EffectVar(1, pTarget, iNumber) = GetY(pTarget);
}

global func FxStayThereTimer(pTarget, iNumber)
{
  if(!GetCommand(pTarget))
    SetCommand(pTarget, "MoveTo", 0, EffectVar(0, pTarget, iNumber), EffectVar(1, pTarget, iNumber));
}

// Kein Schaden //
global func FxNoDmgDamage(pTarget, iNumber, iDamage, iCause) { return(0); }
global func FxNoDmgEffect(szNewEffectName, pTarget, iNumber) { if(WildcardMatch(szNewEffectName,"*Fire*")) return(-1); }