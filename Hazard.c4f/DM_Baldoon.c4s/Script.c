/*-- DM - Burgruine --*/

#strict

static vent1,vent2;
static light1,light2,light3;
static aTeleporters;

func Initialize() {
  aTeleporters = CreateArray();

  SetSkyParallax(0,15,0,0,0,SkyPar_Keep(),SkyPar_Keep());

  //Lichter
  CreateObject(CLGH,265,455,-1);
  CreateObject(CLGH,585,405,-1);
  CreateObject(CLGH,625,405,-1);
  CreateObject(CLGH,235,315,-1);
  CreateObject(CLGH,565,495,-1);
  CreateObject(CLGH,685,535,-1);
  CreateObject(CLGH,255,615,-1);
  CreateObject(CLGH,305,615,-1);
  CreateObject(CLGH,625,925,-1);
  CreateObject(CLGH,675,925,-1);
  CreateObject(CLGH,725,925,-1);
  CreateObject(CLGH,445,465,-1);
  CreateObject(CLGH,635,335,-1);
  CreateObject(CLGH,635,265,-1);
  CreateObject(CLGH,1015,355,-1);
  CreateObject(CLGH,1575,365,-1);
  CreateObject(CLGH,1785,315,-1);
  CreateObject(CLGH,1465,115,-1);
  CreateObject(CLGH,1725,585,-1);
  CreateObject(CLGH,1725,655,-1);
  CreateObject(CLGH,1915,565,-1);
  CreateObject(CLGH,1545,685,-1);
  CreateObject(LBGH,250,480,-1)->Set(40);
  CreateObject(LBGH,350,480,-1)->Set(40);
  CreateObject(LBGH,1580,250,-1);
  CreateObject(LLGH,1670,930,-1);
  CreateObject(LLGH,1890,930,-1);
  CreateObject(LLGH,1615,390,-1);
  CreateObject(LLGH,1635,390,-1);
  CreateObject(LLGH,1615,450,-1);
  CreateObject(LLGH,1635,450,-1);
  CreateObject(LLGH,1615,510,-1);
  CreateObject(LLGH,1635,510,-1);
  CreateObject(LLGH,1615,570,-1);
  CreateObject(LLGH,1635,570,-1);
  CreateObject(LLGH,1615,630,-1);
  CreateObject(LLGH,1635,630,-1);
  CreateObject(LLGH,1615,690,-1);
  CreateObject(LLGH,1635,690,-1);
  
  //Umwelt/Effektobjekte
  CreateObject(SHRK,20,810,-1);
  CreateObject(SHRK,120,830,-1);
  CreateObject(_WEG,1380,380,-1);
  SetCon(110,CreateObject(VINE,60,440,-1));
  SetCon(120,CreateObject(VINE,115,110,-1));
  SetCon(87,CreateObject(VINE,118,165,-1));
  SetCon(104,CreateObject(VINE,112,200,-1));
  SetCon(124,CreateObject(VINE,115,245,-1));
  SetCon(98,CreateObject(VINE,117,270,-1));
  SetCon(105,CreateObject(VINE,115,310,-1));
  SetCon(109,CreateObject(VINE,114,340,-1));
  SetCon(120,CreateObject(VINE,675,190,-1));
  SetCon(87,CreateObject(VINE,678,245,-1));
  SetCon(104,CreateObject(VINE,672,280,-1));
  SetCon(124,CreateObject(VINE,675,325,-1));
  SetCon(98,CreateObject(VINE,677,350,-1));
  SetCon(105,CreateObject(VINE,675,390,-1));
  SetCon(120,CreateObject(VINE,965,320,-1));
  SetCon(87,CreateObject(VINE,968,375,-1));
  SetCon(104,CreateObject(VINE,962,410,-1));
  SetCon(124,CreateObject(VINE,965,455,-1));
  SetCon(78,CreateObject(VINE,967,470,-1));
  SetCon(120,CreateObject(VINE,1055,320,-1));
  SetCon(87,CreateObject(VINE,1058,375,-1));
  SetCon(104,CreateObject(VINE,1052,410,-1));
  SetCon(124,CreateObject(VINE,1055,455,-1));
  SetCon(78,CreateObject(VINE,1057,470,-1));
  SetCon(120,CreateObject(VINE,1435,240,-1));
  SetCon(87,CreateObject(VINE,1438,295,-1));
  SetCon(77,CreateObject(VINE,1434,330,-1));
  SetCon(120,CreateObject(VINE,1405,90,-1));
  SetCon(87,CreateObject(VINE,1408,145,-1));
  SetCon(77,CreateObject(VINE,1404,180,-1));
  CreateConstruction(_FAK,1880,113,-1,100,1);
  CreateConstruction(_FAK,1630,193,-1,100,1);
  CreateConstruction(_FAK,1390,403,-1,100,1);
  CreateConstruction(_FAK,1380,563,-1,100,1);
  CreateConstruction(_FAK,1270,683,-1,100,1);
  CreateConstruction(_FAK,1240,433,-1,100,1);
  CreateConstruction(_FAK,1100,523,-1,100,1);
  CreateConstruction(_FAK,1090,783,-1,100,1);
  CreateConstruction(_FAK,510,893,-1,100,1);
  CreateConstruction(_FAK,500,733,-1,100,1);
  CreateConstruction(_FAK,80,563,-1,100,1);
  CreateConstruction(_FAK,760,553,-1,100,1);
  CreateConstruction(_FAK,930,553,-1,100,1);
  
  var tre1=5;
  while(tre1) {PlaceVegetation(_TR1,0,0,1450,560,100000); tre1--;}
  
  var tre2=4;
  while(tre2) {PlaceVegetation(_TR2,0,0,1450,560,100000); tre2--;}
  
  var tre3=5;
  while(tre3) {PlaceVegetation(_TR3,0,0,1450,560,100000); tre3--;}
  
  var tre4=2;
  while(tre4) {PlaceVegetation(_TR4,0,0,1450,560,100000); tre4--;}
  
  var tre2b=2;
  while(tre2b) {PlaceVegetation(_TR2,1440,0,650,250,100000); tre2b--;}
  
  var tre3b=3;
  while(tre3b) {PlaceVegetation(_TR3,1440,0,650,250,100000); tre3b--;}
  
  CreateObject(_RAN,10,0,-1);
  CreateObject(_AMB,165,550,-1); 
  CreateObject(_QUE,648,781,-1);
  
  light1=CreateObject(ALGH,1390,900,-1);
  light2=CreateObject(ALGH,1450,900,-1);
  light3=CreateObject(ALGH,1510,900,-1);
  
  vent1=CreateObject(GAT2,1420,920,-1);
  vent2=CreateObject(GAT2,1480,920,-1);
  
  //Spielobjekte 
  CreateObject(_ART,480,220,-1);
  CreateObject(_ART,1795,190,-1);
  
  //Jumppads
  CreateObject(JMPD,1420,100)->Set(100,45);
  CreateObject(JMPD,770,960)->Set(82,35);
  CreateObject(JMPD,980,330)->Set(140,18);
  CreateObject(JMPD,660,190)->Set(130,-20);
  CreateObject(JMPD,300,150)->Set(80,30);
  CreateObject(JMPD,1030,930)->Set(85,-40);
  CreateObject(JMPD,580,190)->Set(110,20);
  CreateObject(JMPD,1040,330)->Set(113,-28);
  CreateObject(JMPD,1200,445)->Set(98,-15);
  CreateObject(JMPD,1210,445)->Set(140,12);
  
  //Teleporter
  var teletarget1=CreateObject(TELE,625,250,-1);
  var teletarget2=CreateObject(TELE,1295,940,-1);
  teletarget1->SetTarget(teletarget2);
  teletarget2->SetTarget(teletarget1);
  
  aTeleporters[0] = teletarget1;
  aTeleporters[1] = teletarget2;
  
  CreateObject(RADR,160,100,-1);
  
  //Labor
  CreateObject(CONS,1820,920,-1);
  CreateObject(LTBL,1780,940,-1);
  CreateObject(GADG,1780,920,-1);
  CreateObject(GADG,1790,920,-1);
  CreateObject(MONI,1770,920,-1);
  CreateObject(OPTB,1880,920,-1);
  CreateObject(LBPC,1700,950,-1);
  CreateObject(FRAM,1760,910,-1);
  CreateObject(FRAM,1850,910,-1);
  CreateObject(FRAM,1805,910,-1);
  CreateObject(GSBO,1860,610,-1);
  CreateObject(GSBO,1880,610,-1);
  CreateObject(GLST,1380,950,-1);
  SetCon(30,CreateObject(VENT,1800,900,-1));
  
  //Office
  CreateObject(OTBL,1575,330,-1);
  CreateObject(OCHR,1550,330,-1);
  CreateObject(OCHR,1600,330,-1);
  CreateObject(OTBL,1695,380,-1);
  CreateObject(OCHR,1670,380,-1);
  CreateObject(OCHR,1720,380,-1);
  CreateObject(CCP2,1510,230,-1);
  CreateObject(DEB1,1750,190,-1);
  
  //Base
  CreateObject(GSTA,415,540,-1);
  CreateObject(BED2,625,305,-1);
  CreateObject(BED2,625,375,-1);
  CreateObject(CLVM,230,540,-1);
  CreateObject(SPVM,1560,390,-1);
  CreateObject(LCOM,575,480,-1);
  CreateObject(LCOM,595,480,-1);
  CreateObject(LCOM,615,480,-1);
  CreateObject(FIEX,240,330,-1);
  CreateObject(FIEX,560,510,-1);
  CreateObject(FIEX,670,950,-1);
  CreateObject(FIEX,990,540,-1);
  CreateObject(FIEX,1620,940,-1);
  CreateObject(FIEX,1540,240,-1);
  CreateObject(FIEX,1800,290,-1);
  CreateObject(GTBL,240,430,-1);
  CreateObject(LCKR,650,380,-1);
  CreateObject(LCKR,650,310,-1);
  CreateObject(SBBA,695,520,-1);
  CreateObject(CON1,480,260,-1);
  CreateObject(PLNT,330,430,-1);
  CreateObject(PLNT,320,550,-1);
  CreateObject(PLT2,630,550,-1);
  CreateObject(PLT2,1650,340,-1);
  CreateObject(PLNT,1740,680,-1);
  CreateObject(PLNT,1700,610,-1);
  CreateObject(LFTP,1625,540,-1)->SetLimits(395,745);
  CreateObject(LFTP,165,550,-1)->SetLimits(155,545);; 
  var liftp = CreateObject (LFTP, 870, 825, -1);
  liftp->DisableVertical();
  liftp->ContactLeft();

  CreateObject(BRDG,776,560,-1)->Lock();
  CreateObject(BRDG,848,560,-1)->Lock();
  CreateObject(BRDG,920,560,-1)->Lock();
  CreateObject(BRDG,776,571,-1)->Lock();
  CreateObject(BRDG,848,571,-1)->Lock();
  CreateObject(BRDG,920,571,-1)->Lock();
  CreateObject(LADR,592,390,-1)->Set(11);
  CreateObject(LADR,1976,260,-1)->Set(11);
  CreateObject(LADR,1636,950,-1)->Set(18);
  CreateObject(LADR,1471,400,-1)->Set(15);
  CreateObject(LADR,1481,250,-1)->Set(20);
  CreateObject(LADR,1001,410,-1)->Set(11);
  CreateObject(LADR,1021,550,-1)->Set(18);
  CreateObject(LADR,1195,740,-1)->Set(29);
  CreateObject(LADR,490,560,-1)->Set(11);
  CreateObject(LADR,290,440,-1)->Set(13);
  CreateObject(LADR,575,660,-1)->Set(13);
  CreateObject(TOIC,340,480,-1);
  CreateObject(TOIC,320,480,-1);
  CreateObject(TOIC,300,480,-1);
  CreateObject(TOIC,280,480,-1);
  CreateObject(WSHB,240,480,-1);
  CreateObject(WSHB,220,480,-1);
  
  //Türen
  CreateObject(STDR,1535,400,-1)->OnDestroyed();
  CreateObject(LBDR,1650,950,-1)->OnDestroyed();
  CreateObject(SLDR,1925,940,-1);
  CreateObject(LBDR,1840,620,-1);
  CreateObject(STDR,1495,290,-1)->OnDestroyed();
  CreateObject(SLDR,1435,400,-1);
  CreateObject(STDR,965,550,-1);
  CreateObject(STDR,1055,550,-1);
  CreateObject(STDR,585,250,-1);
  CreateObject(SLDR,345,670,-1);
  CreateObject(STDR,1525,720,-1)->OnDestroyed();
  CreateObject(STDR,1565,720,-1);
  CreateObject(SLDR,1165,510,-1);
  CreateObject(STDR,575,390,-1);
  CreateObject(STDR,705,560,-1);
  CreateObject(STDR,535,560,-1);
  CreateObject(STDR,665,560,-1)->OnDestroyed();
  CreateObject(STDR,545,480,-1);
  CreateObject(STDR,205,480,-1);
  CreateObject(STDR,205,340,-1);
  CreateObject(STDR,205,160,-1);
  
  //Fenster
  CreateObject(_WIN,270,420,-1);
  CreateObject(_WIN,620,300,-1);
  CreateObject(_WIN,620,370,-1);
  CreateObject(_WIN,580,460,-1);
  CreateObject(_WIN,630,460,-1);
  CreateObject(_WIN,1940,600,-1);
  CreateObject(_WIN,1890,600,-1);
  CreateObject(_WIN,1780,360,-1);
  CreateObject(_WIN,1720,380,-1);
  CreateObject(_WIN,1610,330,-1);
  CreateObject(_WIN,1500,380,-1);
  CreateObject(_WIN,1500,340,-1);
  CreateObject(_WIN,1490,230,-1);
  CreateObject(_WIN,1460,200,-1);
  CreateObject(_WIN,1460,150,-1);
  CreateObject(_WIN,280,650,-1);
  CreateObject(_WIN,1010,390,-1);
  CreateObject(_WIN,170,180,-1);
  CreateObject(_WIN,160,240,-1);
  CreateObject(_WIN,170,300,-1);
  CreateObject(_WIN,160,360,-1);
  CreateObject(_WIN,170,420,-1);
  CreateObject(_WIN,160,480,-1);
  CreateObject(_WIN,170,540,-1);
  CreateObject(_WIN,370,540,-1);
  CreateObject(_WIN,270,540,-1);
  CreateObject(_WIN,470,540,-1);
  CreateObject(_WIN,630,540,-1);
  
  // Waffen & Ausrüstung
  PlaceSpawnpoint(AIRS,80,848);
  PlaceSpawnpoint(AIRS,1960,83);
  PlaceSpawnpoint(BZWP,1500,238);
  PlaceSpawnpoint(BZWP,140,88);
  PlaceSpawnpoint(MIAP,1520,238);
  PlaceSpawnpoint(MIAP,160,88);
  PlaceSpawnpoint(ENWP,300,548);
  if(GetPlayerCount()>5) PlaceSpawnpoint(ENWP,1440,708);
  PlaceSpawnpoint(ENAP,320,548);
  if(GetPlayerCount()>5) PlaceSpawnpoint(ENAP,1460,708);
  PlaceSpawnpoint(ENAP,1660,818);
  PlaceSpawnpoint(FTWP,610,378);
  PlaceSpawnpoint(FTWP,1900,608);
  PlaceSpawnpoint(GSAP,630,378);
  PlaceSpawnpoint(GSAP,1920,608);
  PlaceSpawnpoint(GSAP,230,333);
  PlaceSpawnpoint(GLWP,610,178);
  PlaceSpawnpoint(GLWP,660,948); 
  PlaceSpawnpoint(GRAP,680,948);
  PlaceSpawnpoint(GRAP,250,148);
  PlaceSpawnpoint(GRAP,630,178);
  PlaceSpawnpoint(MIWP,1880,313);
  PlaceSpawnpoint(MIWP,1440,958); 
  PlaceSpawnpoint(STAP,1460,958); 
  PlaceSpawnpoint(STAP,1900,313);
  PlaceSpawnpoint(GGWP,280,778);
  PlaceSpawnpoint(ENAP,300,778);
  PlaceSpawnpoint(PGWP,1370,548);
  if(GetPlayerCount()>5) PlaceSpawnpoint(PGWP,280,428);
  PlaceSpawnpoint(STAP,1390,548);
  if(GetPlayerCount()>5) PlaceSpawnpoint(STAP,300,428);
  PlaceSpawnpoint(STAP,1720,468);
  PlaceSpawnpoint(MINE,500,718);
  PlaceSpawnpoint(MINE,1850,938);
  PlaceSpawnpoint(JTPK,220,428);
  PlaceSpawnpoint(JTPK,1750,938);
  PlaceSpawnpoint(KRFL,510,878);
  PlaceSpawnpoint(KRFL,1630,178);
  PlaceSpawnpoint(KLAS,830,548);
  PlaceSpawnpoint(KLAS,1720,608);
  PlaceSpawnpoint(KSLM,1160,748);
  //PlaceSpawnpoint(FLSH,460,778);
  if(GetPlayerCount()>5) PlaceSpawnpoint(MEDI,550,238);
  PlaceSpawnpoint(MEDI,1000,538);
  if(GetPlayerCount()>7) PlaceSpawnpoint(MEDI,1720,678);
  PlaceSpawnpoint(HARM,1010,318);
  PlaceSpawnpoint(AIRS,1130,498);
  PlaceBonusSpawnpoint([AEXB,BSKB,HELB,KAMB], 1510, 798, 2000);
  PlaceBonusSpawnpoint([AMPB,HSTB,INVB,RPFB], 250, 658, 2000);
  
  PlaceWaypoints();
  
  return(1);
}

func PlaceWaypoints()
{
  var wp1 = CreateWP(1417,90);
  var wp2 = CreateWP(1839,610);
  var wp3 = CreateWP(1658,679);
  var wp4 = CreateWP(1813,680);
  var wp5 = CreateWP(1918,609);
  var wp6 = CreateWP(1956,167);
  var wp7 = CreateWP(1968,265);
  var wp8 = CreateWP(1502,238);
  var wp9 = CreateWP(1677,194);
  var wp10 = CreateWP(1452,88);
  var wp11 = CreateWP(1959,82);
  var wp12 = CreateWP(1794,199);
  var wp13 = CreateWP(1549,238);
  var wp14 = CreateWP(1658,607);
  var wp15 = CreateWP(1224,426);
  var wp16 = CreateWP(1185,435);
  var wp17 = CreateWP(1484,391);
  var wp18 = CreateWP(1594,391);
  var wp19 = CreateWP(1659,389);
  var wp20 = CreateWP(1489,282);
  var wp21 = CreateWP(1602,342);
  var wp22 = CreateWP(1648,342);
  var wp23 = CreateWP(1766,274);
  var wp24 = CreateWP(1875,311);
  var wp25 = CreateWP(1922,314);
  var wp26 = CreateWP(1657,513);
  var wp27 = CreateWP(999,534);
  var wp28 = CreateWP(657,180);
  var wp29 = CreateWP(638,548);
  var wp30 = CreateWP(505,549);
  var wp31 = CreateWP(574,548);
  var wp32 = CreateWP(464,548);
  var wp33 = CreateWP(166,537);
  var wp34 = CreateWP(418,469);
  var wp35 = CreateWP(620,378);
  var wp36 = CreateWP(202,145);
  var wp37 = CreateWP(288,140);
  var wp38 = CreateWP(641,180);
  var wp39 = CreateWP(582,180);
  var wp40 = CreateWP(140,89);
  var wp41 = CreateWP(231,326);
  var wp42 = CreateWP(222,428);
  var wp43 = CreateWP(321,429);
  var wp44 = CreateWP(515,468);
  var wp45 = CreateWP(1407,546);
  var wp46 = CreateWP(1017,319);
  var wp47 = CreateWP(1540,388);
  var wp48 = CreateWP(1590,711);
  var wp49 = CreateWP(1215,578);
  var wp50 = CreateWP(1215,653);
  var wp51 = CreateWP(1134,498);
  var wp52 = CreateWP(1185,744);
  var wp53 = CreateWP(991,797);
  var wp54 = CreateWP(249,656);
  var wp55 = CreateWP(507,717);
  var wp56 = CreateWP(554,680);
  var wp57 = CreateWP(764,947);
  var wp58 = CreateWP(596,944);
  var wp59 = CreateWP(544,877);
  var wp60 = CreateWP(463,912);
  var wp61 = CreateWP(356,892);
  var wp62 = CreateWP(361,787);
  var wp63 = CreateWP(752,796);
  var wp64 = CreateWP(459,767);
  var wp65 = CreateWP(289,774);
  var wp66 = CreateWP(197,819);
  var wp67 = CreateWP(627,238);
  var wp68 = CreateWP(1036,319);
  var wp69 = CreateWP(986,320);
  var wp70 = CreateWP(1651,752);
  var wp71 = CreateWP(1657,821);
  var wp72 = CreateWP(1849,938);
  var wp73 = CreateWP(1818,936);
  var wp74 = CreateWP(1652,939);
  var wp75 = CreateWP(1608,939);
  var wp76 = CreateWP(1297,941);
  var wp77 = CreateWP(1038,919);
  wp1 -> AddPath(wp11, Path_MoveTo, 1);
  wp2 -> AddPath(wp5, Path_MoveTo, 1);
  wp2 -> AddPath(wp14, Path_Backflip, -1);
  wp2 -> AddPath(wp3, Path_MoveTo, -1);
  wp3 -> AddPath(wp4, Path_MoveTo, 1);
  wp3 -> AddPath(wp48, Path_MoveTo, -1);
  wp3 -> AddPath(wp18, Path_Lift, -1);
  wp3 -> AddPath(wp19, Path_Lift, 1);
  wp3 -> AddPath(wp26, Path_Lift, -1);
  wp4 -> AddPath(wp3, Path_MoveTo, -1);
  wp4 -> AddPath(wp14, Path_Jump, -1);
  wp4 -> AddPath(wp2, Path_Jump, 1);
  wp4 -> AddPath(wp2, Path_MoveTo, 1);
  wp5 -> AddPath(wp2, Path_MoveTo, -1);
  wp6 -> AddPath(wp12, Path_MoveTo, -1);
  wp8 -> AddPath(wp10, Path_Jump, -1);
  wp8 -> AddPath(wp13, Path_MoveTo, 1);
  wp9 -> AddPath(wp12, Path_Jump, 1);
  wp9 -> AddPath(wp23, Path_MoveTo, 1);
  wp9 -> AddPath(wp13, Path_MoveTo, -1);
  wp10 -> AddPath(wp1, Path_MoveTo, -1);
  wp10 -> AddPath(wp8, Path_MoveTo, 1);
  wp10 -> AddPath(wp9, Path_Jump, 1);
  wp10 -> AddPath(wp13, Path_Jump, 1);
  wp11 -> AddPath(wp12, Path_MoveTo, -1);
  wp12 -> AddPath(wp6, Path_Jump, 1);
  wp12 -> AddPath(wp9, Path_MoveTo, -1);
  wp13 -> AddPath(wp8, Path_MoveTo, -1);
  wp13 -> AddPath(wp9, Path_Jump, 1);
  wp14 -> AddPath(wp48, Path_Lift, -1);
  wp14 -> AddPath(wp18, Path_Lift, -1);
  wp14 -> AddPath(wp19, Path_Lift, 1);
  wp15 -> AddPath(wp17, Path_MoveTo, 1);
  wp17 -> AddPath(wp20, Path_Jump, 1);
  wp17 -> AddPath(wp15, Path_MoveTo, -1);
  wp18 -> AddPath(wp47, Path_MoveTo, -1);
  wp18 -> AddPath(wp19, Path_Backflip, 1);
  wp18 -> AddPath(wp48, Path_Lift, -1);
  wp18 -> AddPath(wp26, Path_Lift, 1);
  wp18 -> AddPath(wp14, Path_Lift, 1);
  wp18 -> AddPath(wp3, Path_Lift, 1);
  wp19 -> AddPath(wp24, Path_MoveTo, 1);
  wp19 -> AddPath(wp18, Path_Backflip, -1);
  wp19 -> AddPath(wp26, Path_Lift, -1);
  wp19 -> AddPath(wp48, Path_Lift, -1);
  wp19 -> AddPath(wp14, Path_Lift, -1);
  wp19 -> AddPath(wp3, Path_Lift, -1);
  wp21 -> AddPath(wp22, Path_Jump, 1);
  wp21 -> AddPath(wp20, Path_Jump, -1);
  wp22 -> AddPath(wp21, Path_Jump, -1);
  wp22 -> AddPath(wp23, Path_MoveTo, 1);
  wp22 -> AddPath(wp23, Path_Jump, 1);
  wp23 -> AddPath(wp9, Path_Jump, -1);
  wp23 -> AddPath(wp22, Path_MoveTo, -1);
  wp24 -> AddPath(wp23, Path_Jump, -1);
  wp24 -> AddPath(wp19, Path_MoveTo, -1);
  wp24 -> AddPath(wp25, Path_MoveTo, 1);
  wp25 -> AddPath(wp7, Path_Jump, 1);
  wp25 -> AddPath(wp24, Path_MoveTo, -1);
  wp26 -> AddPath(wp18, Path_Lift, -1);
  wp26 -> AddPath(wp19, Path_Lift, 1);
  wp26 -> AddPath(wp48, Path_Lift, -1);
  wp26 -> AddPath(wp3, Path_Lift, 1);
  wp27 -> AddPath(wp29, Path_MoveTo, -1);
  wp27 -> AddPath(wp51, Path_MoveTo, 1);
  wp27 -> AddPath(wp46, Path_Jump, 1);
  wp28 -> AddPath(wp40, Path_MoveTo, -1);
  wp29 -> AddPath(wp30, Path_Jump, -1);
  wp29 -> AddPath(wp27, Path_MoveTo, 1);
  wp30 -> AddPath(wp29, Path_Jump, 1);
  wp30 -> AddPath(wp31, Path_MoveTo, 1);
  wp31 -> AddPath(wp30, Path_MoveTo, -1);
  wp31 -> AddPath(wp56, Path_MoveTo, -1);
  wp32 -> AddPath(wp30, Path_MoveTo, 1);
  wp32 -> AddPath(wp33, Path_MoveTo, -1);
  wp32 -> AddPath(wp44, Path_Jump, 1);
  wp33 -> AddPath(wp36, Path_Lift, 1);
  wp33 -> AddPath(wp41, Path_Lift, 1);
  wp33 -> AddPath(wp32, Path_MoveTo, 1);
  wp34 -> AddPath(wp32, Path_MoveTo, 1);
  wp35 -> AddPath(wp32, Path_MoveTo, -1);
  wp36 -> AddPath(wp41, Path_Lift, 1);
  wp36 -> AddPath(wp33, Path_Lift, -1);
  wp36 -> AddPath(wp37, Path_MoveTo, 1);
  wp37 -> AddPath(wp38, Path_MoveTo, 1);
  wp38 -> AddPath(wp28, Path_MoveTo, 1);
  wp38 -> AddPath(wp39, Path_MoveTo, -1);
  wp38 -> AddPath(wp27, Path_Jump, 1);
  wp39 -> AddPath(wp46, Path_MoveTo, 1);
  wp41 -> AddPath(wp36, Path_Lift, -1);
  wp41 -> AddPath(wp33, Path_Lift, -1);
  wp41 -> AddPath(wp43, Path_MoveTo, 1);
  wp42 -> AddPath(wp43, Path_MoveTo, 1);
  wp43 -> AddPath(wp34, Path_MoveTo, 1);
  wp43 -> AddPath(wp42, Path_MoveTo, -1);
  wp43 -> AddPath(wp41, Path_Jump, -1);
  wp44 -> AddPath(wp35, Path_Jump, 1);
  wp45 -> AddPath(wp49, Path_MoveTo, -1);
  wp45 -> AddPath(wp47, Path_Jump, 1);
  wp46 -> AddPath(wp29, Path_Jump, -1);
  wp46 -> AddPath(wp51, Path_Jump, 1);
  wp46 -> AddPath(wp68, Path_MoveTo, 1);
  wp46 -> AddPath(wp69, Path_MoveTo, -1);
  wp47 -> AddPath(wp17, Path_Jump, -1);
  wp47 -> AddPath(wp45, Path_MoveTo, -1);
  wp47 -> AddPath(wp18, Path_MoveTo, 1);
  wp47 -> AddPath(wp17, Path_Backflip, -1);
  wp48 -> AddPath(wp50, Path_MoveTo, -1);
  wp48 -> AddPath(wp18, Path_Lift, 1);
  wp48 -> AddPath(wp19, Path_Lift, 1);
  wp48 -> AddPath(wp26, Path_Lift, 1);
  wp48 -> AddPath(wp14, Path_Lift, 1);
  wp48 -> AddPath(wp3, Path_Jump, 1);
  wp48 -> AddPath(wp70, Path_MoveTo, 1);
  wp49 -> AddPath(wp51, Path_MoveTo, -1);
  wp49 -> AddPath(wp52, Path_MoveTo, -1);
  wp49 -> AddPath(wp45, Path_MoveTo, 1);
  wp50 -> AddPath(wp48, Path_MoveTo, 1);
  wp51 -> AddPath(wp49, Path_MoveTo, 1);
  wp51 -> AddPath(wp50, Path_MoveTo, 1);
  wp51 -> AddPath(wp52, Path_MoveTo, 1);
  wp51 -> AddPath(wp27, Path_MoveTo, -1);
  wp51 -> AddPath(wp16, Path_Jump, 1);
  wp52 -> AddPath(wp50, Path_Jump, 1);
  wp52 -> AddPath(wp49, Path_Jump, 1);
  wp52 -> AddPath(wp51, Path_Jump, 1);
  wp52 -> AddPath(wp53, Path_MoveTo, -1);
  wp53 -> AddPath(wp63, Path_Lift, -1);
  wp53 -> AddPath(wp52, Path_MoveTo, 1);
  wp54 -> AddPath(wp55, Path_MoveTo, 1);
  wp55 -> AddPath(wp54, Path_Jump, -1);
  wp55 -> AddPath(wp56, Path_MoveTo, 1);
  wp55 -> AddPath(wp64, Path_MoveTo, -1);
  wp56 -> AddPath(wp55, Path_MoveTo, -1);
  wp56 -> AddPath(wp31, Path_Jump, 1);
  wp57 -> AddPath(wp53, Path_MoveTo, 1);
  wp58 -> AddPath(wp57, Path_MoveTo, 1);
  wp59 -> AddPath(wp58, Path_MoveTo, 1);
  wp60 -> AddPath(wp59, Path_MoveTo, 1);
  wp61 -> AddPath(wp60, Path_MoveTo, 1);
  wp62 -> AddPath(wp61, Path_MoveTo, -1);
  wp63 -> AddPath(wp64, Path_MoveTo, -1);
  wp63 -> AddPath(wp53, Path_Lift, 1);
  wp64 -> AddPath(wp55, Path_Jump, 1);
  wp64 -> AddPath(wp65, Path_MoveTo, -1);
  wp64 -> AddPath(wp62, Path_MoveTo, -1);
  wp64 -> AddPath(wp63, Path_MoveTo, 1);
  wp65 -> AddPath(wp62, Path_MoveTo, 1);
  wp65 -> AddPath(wp64, Path_MoveTo, 1);
  wp66 -> AddPath(wp65, Path_MoveTo, 1);
  wp67 -> AddPath(wp76, Path_MoveTo, 1);
  wp67 -> SetArriveCommand(0, 0, "Enter", aTeleporters[0], 0, 0);
  wp68 -> AddPath(wp38, Path_MoveTo, -1);
  wp69 -> AddPath(wp10, Path_MoveTo, 1);
  wp70 -> AddPath(wp48, Path_Jump, -1);
  wp71 -> AddPath(wp75, Path_MoveTo, -1);
  wp71 -> AddPath(wp74, Path_MoveTo, -1);
  wp71 -> AddPath(wp70, Path_Jump, 1);
  wp72 -> AddPath(wp73, Path_MoveTo, -1);
  wp73 -> AddPath(wp72, Path_MoveTo, 1);
  wp73 -> AddPath(wp74, Path_MoveTo, -1);
  wp74 -> AddPath(wp73, Path_MoveTo, 1);
  wp74 -> AddPath(wp75, Path_MoveTo, -1);
  wp74 -> AddPath(wp71, Path_Jump, -1);
  wp75 -> AddPath(wp74, Path_MoveTo, 1);
  wp75 -> AddPath(wp71, Path_Jump, 1);
  wp75 -> AddPath(wp76, Path_MoveTo, -1);
  wp76 -> AddPath(wp67, Path_MoveTo, -1);
  wp76 -> SetArriveCommand(0, 0, "Enter", aTeleporters[1], 0, 0);
  wp76 -> AddPath(wp75, Path_MoveTo, 1);
  wp76 -> AddPath(wp77, Path_MoveTo, -1);
  wp77 -> AddPath(wp63, Path_MoveTo, -1);
  wp77 -> AddPath(wp76, Path_MoveTo, 1);
}

/* Regelwähler */

public func ChooserFinished()
{
  Arena_ChooserFinished();

  if(FindObject(WPCH))
    FindObject(WPCH)->RemoveWare(AIRT);
}

/* Relaunch */

protected func InitializePlayer(int iPlr, int iX, int iY, object pBase, int iTeam)
{
  for(var i=0, pCrew ; pCrew = GetCrew(iPlr, i) ; i++)
    RelaunchPlayer(iPlr, pCrew, -1, iTeam);
}

public func RelaunchPlayer(int iPlr, object pCrew, int iKiller, int iTeam)
{
  Arena_RelaunchPlayer(iPlr, pCrew, iKiller, iTeam);
}

public func RelaunchPosition(int iTeam)
{
  // Anzahl Zufallpositionen, iTeam verwendet für spezielle Teamstartpositionen
  var rand = Random(10);
  if(!rand)   return [190,90];
  if(!--rand) return [90, 430];
  if(!--rand) return [610, 380];
  if(!--rand) return [450, 770];
  if(!--rand) return [990, 400];
  if(!--rand) return [1140, 940];
  if(!--rand) return [1320, 560];
  if(!--rand) return [1870, 610];
  if(!--rand) return [1650, 340];
  if(!--rand) return [1440, 90];
}
