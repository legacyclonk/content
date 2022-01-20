/*-- Tutorial --*/

#strict

static screen, doc, door1, drone, tele, tele2, killcounter;
static process;

func Initialize() {
  LaunchRain(980, Material("Rain"), 1000, 80)->SetCategory(1);

  CreateObject(TIM1, 1075, 460, -1)->Sound("RainLoop", 0,0,50,0, 1);

  screen = CreateObject(CSCR,283,819,0);
  doc = CreateObject (CDOC, 521, 790, -1);
  doc->SetDir(DIR_Left); 
	
	CreateObject(RAI1,918,80,-1)->SetRail([0, 1, 4],1);
	CreateObject(LADR,1880,220,-1)->Set(10);
	CreateObject(WNDW,1608,231,-1);
	CreateObject(RAI1,490,240,-1)->SetRail([0, 1, 1, 1, 1, 1, 4],1);
	CreateObject(CONS,500,322,-1);
	CreateObject(X5RK,280,596,-1);
	CreateObject(X5ST,282,618,-1)->SetDir(0);
	CreateObject(TRE4,1775,465,-1);
	CreateObject(TRE4,1821,456,-1);
	CreateObject(TRB3,1846,456,-1);
	CreateObject(TRE4,1874,460,-1);
	CreateObject(TRE4,1521,527,-1);
	CreateObject(TRE4,1526,525,-1);
	CreateObject(TRB3,1680,496,-1);
	CreateObject(TRB2,1823,483,-1);
	CreateObject(TRE4,1920,478,-1);
	CreateObject(LADR,560,570,-1)->Set(4);
	CreateObject(LFTP,675,553,-1);
	CreateObject(TRE4,1463,534,-1);
	CreateObject(TRE4,1517,530,-1);
	CreateObject(TRE4,1562,536,-1);
	CreateObject(SLDR,587,600,-1)->Lock();
	CreateObject(PKEY,609,594,-1);
	CreateObject(FENC,782,601,-1);
	CreateObject(FENC,842,601,-1);
	CreateObject(FENC,902,601,-1);
	CreateObject(FENC,962,601,-1);
	CreateObject(FENC,1120,601,-1);
	CreateObject(SBBA,1395,599,-1)->SetDir(0);
	CreateObject(VENT,25,669,-1)->SetCon(35);
	CreateObject(VENT,505,669,-1)->SetCon(35);
	CreateObject(LADR,315,710,-1)->Set(4);
	CreateObject(CCP1,570,690,-1);
	
	CreateObject(SEDR,640,690,-1)->Lock();
	CreateObject(SEDR,710,690,-1)->Lock();
	CreateObject(SEDR,710,800,-1)->Lock();
	CreateObject(SEDR,709,910,-1)->Lock();

	CreateObject(PSTR,811,702,-1)->Set(2,3);
	CreateObject(BED2,836,709,-1);
	CreateObject(BED2,879,709,-1);
	CreateObject(BED2,921,709,-1);
	
	// ceiling lights
	CreateObject(CLGH,790,745,-1);
	CreateObject(CLGH,783,644,-1);
	CreateObject(CLGH,904,645,-1);
	CreateObject(CLGH,1004,815,-1);
	CreateObject(CLGH,506,854,-1);
	CreateObject(CLGH,602,854,-1);
	CreateObject(CLGH,1005,870,-1);
	
	CreateObject(ENGT,1057,766,-1);
	CreateObject(LADR,398,910,-1)->Set(15);
	CreateObject(LBDR,452,791,-1);
	CreateObject(GLST,499,790,-1);
	CreateObject(GLST,474,790,-1);
	CreateObject(LLGH,539,774,-1);
	CreateObject(MONI,587,778,-1);
	CreateObject(LTBL,593,789,-1);
	CreateObject(CHAR,796,799,-1);
	CreateObject(CLVM,753,800,-1);
	CreateObject(CHAR,844,799,-1)->SetDir(1);
	CreateObject(TABL,820,799,-1);
	CreateObject(PSTR,802,785,-1)->Set(0,2);
	CreateObject(STDR,875,801,-1);
	CreateObject(LFTP,905,788,-1);
	CreateObject(GSBO,1017,770,-1);
	CreateObject(GSBO,1026,770,-1);
	CreateObject(GAT2,166,812,-1);
	CreateObject(BLGH,210,839,-1);
	CreateObject(SCR2,283,859,-1);
	CreateObject(BLGH,352,840,-1);
	CreateObject(GAT2,736,842,-1);
	CreateObject(GAT2,832,844,-1);
	CreateObject(STDR,935,861,-1);
	CreateObject(FIEX,993,853,-1);
	CreateObject(LCKR,1039,860,-1);
	CreateObject(LCKR,1024,860,-1);
	CreateObject(LCKR,1009,860,-1);

	CreateObject(LCKR,1054,860,-1);
	CreateObject(CCP2,734,910,-1);
	CreateObject(CCP1,779,910,-1);
	CreateObject(PLT2,848,911,-1);
	CreateObject(CCP1,814,910,-1);
	CreateObject(STDR,876,911,-1);
	CreateObject(TABL,968,859,-1);

	CreateObject(LFTP,675,923,-1);
	CreateObject(STDR,937,920,-1);
	CreateObject(WSHB,959,920,-1);
	CreateObject(WSHB,979,920,-1);
	CreateObject(TOIC,1009,920,-1);
	CreateObject(TOIC,1049,920,-1);
	CreateObject(TOIC,1029,920,-1)->SetDir(1);
	
	door1 = CreateObject(SLDR,735,600,-1);
	door1->Lock(0);
	
	tele = CreateObject(TELE,1045,601,-1);	
	tele2 = CreateObject(TELE,1367,971,-1);
	tele->Deactivate();
	tele->SetTarget(tele2);
	tele2->Activate();
	tele2->SetTarget(tele);
		
	CreateObject(FLGH,1470,960,-1)->SetRotation(-45);
	CreateObject(FLGH,1550,960,-1)->SetRotation(30);

	CreateObject(JMPD,1511,932,-1)->Set(50,250,180);
	CreateObject(JMPD,1490,986,-1)->Set(30,-45);
	CreateObject(JMPD,1510,986,-1)->Set(50,-70);
	
	CreateObject(GRAP,1580,970,-1);

	CreateObject(GLWP,1600,971,-1);
	
  var wl = CreateObject (ALGH, 710, 580, -1);
  wl->SetR(180);
  wl->TurnOn();
	
	CreateObject(STAP, 850, 590,-1);
	
  SetDarkness(0);

  ScriptGo(1);

  return(1);
}

func InitializePlayer(int iPlr) {
  var clonk = GetCrew(iPlr);
  clonk->SetPosition(50,899);
  SetCrewEnabled(0, clonk);
}

func Script1() {
  SetCommand(GetCrew(),"MoveTo",screen);
	SetMaxPlayer(2);
  CreateScriptPlayer("$Drones$", RGB(1,1,1), 2, 15);
}

func Script10() {
  SetHostility(0,1, true, true);
  screen->SetAction("Noise");
}

func Script15() {
  screen->SetAction("On");
}

func Script17() {
  screen->ActivateEntrance(GetCrew());
  SetCrewEnabled(1,GetCrew());
  SelectCrew(0,GetCrew(),1);
}

func Script18() {
  if(!process)
    return(goto(18));
  screen->SetAction("Noise");
}

func Script23() {
  screen->SetAction("Off");
}

func Script24() {
  if(process < 2) return(goto(24));
  door1->Unlock();
}

func Script25() {
  if(GetX(GetCrew()) < 750)
    return(goto(25));

  screen->ActivateEntrance(GetCrew());
  SetComDir(COMD_Stop,GetCrew());
  door1->Close();
  door1->Lock();
}

func Script26() {
  if(FindObject(STAP))
    return(goto(26));
  
  screen->ActivateEntrance(GetCrew());
  SetComDir(COMD_Stop,GetCrew());

  drone = CreateObject(TDRO,950,585,-1);
  PlaceSpawnpoint(STAP, 850, 590)->LocalN("spawntimer")=500;
}

func Script30() {
  if(FindObject(TARG))
    return(goto(30));
    
  screen->ActivateEntrance(GetCrew());
  SetComDir(COMD_Stop,GetCrew());

  drone = CreateObject(TDRO,1040,480,-1);  
}

func Script31() {
  if(FindObject(TARG))
    return(goto(31));
  
  screen->ActivateEntrance(GetCrew());
  SetComDir(COMD_Stop,GetCrew());
  
  tele->Activate();
  RemoveAll(SPNP);  
}

func Script32() {
  if(GetX(GetCrew()) < 1400)
    return(goto(32));

  screen->ActivateEntrance(GetCrew());
  SetComDir(COMD_Stop,GetCrew());
  
  tele2->Deactivate();
}

func Script33() {
  if(FindObject(GRAP) || !FindContents(GLWP,GetCrew()))
    return(goto(33));
  
  screen->ActivateEntrance(GetCrew());
  SetComDir(COMD_Stop,GetCrew());
  
  drone = CreateObject(TDRO,1710,915,-1);
  PlaceSpawnpoint(GRAP, 1580, 965)->LocalN("spawntimer")=500;
}

func Script35() {
  if(FindObject(TARG))
    return(goto(35));
  
  screen->ActivateEntrance(GetCrew());
  SetComDir(COMD_Stop,GetCrew());

  FindObject(GLWP)->SetFireMode(2);

  drone = CreateObject(TDRO,1420,930);
  drone->SetDir(1);
  drone->LocalN("drop")=KLAS;
}

func Script36() {
  if(FindObject(TARG))
    return(goto(36));
  
  screen->ActivateEntrance(GetCrew());
  SetComDir(COMD_Stop,GetCrew());
  
  RemoveAll(SPNP);
}

func Script37() {
  if(FindObject(KLAS))
    return(goto(37));
  
  screen->ActivateEntrance(GetCrew());
  SetComDir(COMD_Stop,GetCrew());
  
  PlaceSpawnpoint(JTPK,850,590);
  PlaceSpawnpoint(ENAP,900,590);
  CreateObject(MEDI,925,600);
  
  tele2->Activate();
}

func Script40() {
  if(!FindContents(JTPK,GetCrew()))
    return(goto(40));

  screen->ActivateEntrance(GetCrew());
  SetComDir(COMD_Stop,GetCrew());

  tele->Deactivate();
}

func Script43() {
	if(FindContents(JTPK,GetCrew()))
    return(goto(43));
	
  screen->ActivateEntrance(GetCrew());
  SetComDir(COMD_Stop,GetCrew());
}

func Script45() {
  if(GetX(GetCrew()) < 1070 && GetY(GetCrew()) > 450)
    return(goto(45));
  
  screen->ActivateEntrance(GetCrew());
  SetComDir(COMD_Stop,GetCrew());
  
  //Muni wegnehmen *hidez*
  DoAmmo(STAM,-GetAmmo(STAM,GetCrew()),GetCrew());
  DoAmmo(GRAM,-GetAmmo(GRAM,GetCrew()),GetCrew());
  DoAmmo(GRAM,-GetAmmo(GRAM,FindObject(GLWP)),FindObject(GLWP));
  
  CreateObject(TDRO,1180,450,-1)->StartMoving(0,1,0,25);
  CreateObject(TDRO,1250,450,-1)->StartMoving(0,-1,0,25);
  CreateObject(TDRO,1320,450,-1)->StartMoving(0,1,0,25);
  drone = CreateObject(TDRO,1390,450,-1);
  drone->StartMoving(0,-1,0,25);
  drone->LocalN("drop")=KLAS;
}

func Script46() {
  if(FindObject(TARG))
    return(goto(46));
  
  screen->ActivateEntrance(GetCrew());
  SetComDir(COMD_Stop,GetCrew());
  
  drone = CreateObject(TDRO,1250,350,-1);
  drone->StartMoving(1,1,50,20,25,0);
  drone->LocalN("drop")=BZWP;
  FindObject(TARG)->SetGraphics(0,0,BZWP);
  
  RemoveObject(Contained(FindObject(ENAP)));
  RemoveObject(FindObject(PIWP));
  
  PlaceSpawnpoint(ENAP,1075,450);
}

func Script47() {
  if(FindObject(KLAS))
    return(goto(47));
  
  screen->ActivateEntrance(GetCrew());
  SetComDir(COMD_Stop,GetCrew());
  
  RemoveObject(FindObject(GLWP));
  
  PlaceSpawnpoint(MIAP,1375,590);
  drone = CreateObject(TDRO,1645,675,-1);
  //Zielsuch-Hack *cough*
  drone->SetCategory(C4D_Living);
  drone->SetAlive(1);
}

func Script48() {
  if(FindObject(TARG))
    return(goto(48));
  
  screen->ActivateEntrance(GetCrew());
  SetComDir(COMD_Stop,GetCrew());
  
  RemoveObject(Contained(FindObject(MIAP)));
  RemoveObject(FindObject(BZWP));
  
  GetCrew()->SetAction("Walk");
  
  CreateObject(GGWP,1075,450);
}


func Script49() {
  if(!FindContents(GGWP,GetCrew()))
    return(goto(49));
  
  screen->ActivateEntrance(GetCrew());
  SetComDir(COMD_Stop,GetCrew());
  
  for(var x; x < 10; x++) {
    CreateObject(TDRO,1180+30*x,450,-1)->StartMoving(1,1,15,15,7);
    if(x < 8 ) CreateObject(TDRO,980-30*x,450,-1)->StartMoving(-1,-1,15,15,0,7);
  }
}

func Script50() {
  if(FindObject(TARG))
    return(goto(50));
  
  screen->ActivateEntrance(GetCrew());
  SetComDir(COMD_Stop,GetCrew());
  
  GetCrew()->SetAction("Walk");
  
  RemoveObject(Contained(FindObject(ENAP)));
  RemoveObject(FindObject(GGWP));
  
  var mg = CreateObject(MIWP,GetX(GetCrew()),GetDefBottom(GetCrew()));
  AddEffect("MinigunMegarammoBonus",GetCursor(),190,0,GetCursor());
  AddSpawnEffect(mg);
  //vieeeel ammo.
  DoAmmo(STAM,500,mg);
  DoAmmo(STAM,300,GetCrew());
}

func Script51() {
  var x,y;
  for(x=0; x < 5; x++)
    for(y=0; y < 5; y++)
      CreateObject(TDRO, 850+x*20, 400+y*20, -1)->SetDir(1);
}

func Script52() {
  var x,y;
  for(x=0; x < 5; x++)
    for(y=0; y < 5; y++)
      CreateObject(TDRO, 1230+x*20, 400+y*20, -1);
}

func Script53() {
  var x,y;
  for(x=0; x < 24; x++)
    for(y=0; y < 5; y++)
      CreateObject(TDRO, 850+x*20, 300+y*20, -1)->SetDir(x < 12);
}

func Script54() {
  if(FindObject(TARG))
    return(goto(54));
    
  screen->ActivateEntrance(GetCrew());
  SetComDir(COMD_Stop,GetCrew());
  
  GetCrew()->SetAction("Walk");
}

func Script55() {
  if(process < 19)
    return(goto(55));
}

func Script65() {
	Sound("Applause");
  DoCrewExp(100,GetCrew());
  EliminatePlayer(1);
  GameOver();
}

global func FxMinigunMegarammoBonusFMData(object pTarget, int iEffectNumber, int iData, value) {
  if(GetID(Contents(0,pTarget)) != MIWP)
    return(value);
  if(iData == FM_AmmoLoad)   return(500);
  return(value);
}
