/*-- Docks --*/

#strict

static Goals;
static switchright, switchleft;

protected func Initialize()
{
  SetSkyParallax(0,45,24,0,0,0,0);
  CreateObject(_DCK,220,360,-1);
  CreateObject(_DCK,727,360,-1);

  CreateObject (CRAT, 750, 250, -1);
  //CreateObject (CRAT, 852, 140, -1);
  CreateObject (CRAT, 850, 310, -1);
  CreateObject (CRAT, 979, 310, -1);
  CreateObject (CRAT, 521, 310, -1);
  CreateObject (CRAT, 650, 310, -1);
  //CreateObject (CRAT, 648, 140, -1);
  CreateObject (LADR, 1304, 174, -1)->Set(7);
  CreateObject (LADR, 1207, 601, -1)->Set(14);
  CreateObject (LADR, 1420, 472, -1)->Set(3);
  CreateObject (LADR, 1176, 461, -1)->Set(11);
  CreateObject (LADR, 916, 210, -1)->Set(11);
  CreateObject (LADR, 781, 352, -1)->Set(14);
  CreateObject (LADR, 1026, 350, -1)->Set(6);
  CreateObject (LADR, 476, 350, -1)->Set(6);
  CreateObject (LADR, 721, 351, -1)->Set(14);
  CreateObject (LADR, 587, 210, -1)->Set(11);
  CreateObject (LADR, 326, 461, -1)->Set(11);
  CreateObject (LADR, 82, 472, -1)->Set(3);
  CreateObject (LADR, 295, 600, -1)->Set(14);
  CreateObject (LADR, 196, 174, -1)->Set(7);
  CreateObject (PLT2, 1473, 121, -1);
  CreateObject (PLT2, 27, 121, -1);
  CreateObject (TABL, 1438, 119, -1);
  CreateObject (TABL, 62, 119, -1);
  CreateObject (GSBO, 823, 140, -1);
  CreateObject (GSBO, 881, 210, -1);
  CreateObject (GSBO, 803, 310, -1);
  CreateObject (GSBO, 814, 310, -1);
  CreateObject (GSBO, 686, 310, -1);
  CreateObject (GSBO, 697, 310, -1);
  CreateObject (GSBO, 619, 210, -1);
  CreateObject (GSBO, 677, 140, -1);
  CreateObject (FLGH, 1092, 642, -1)->SetRotation(-70);
  CreateObject (FLGH, 412, 642, -1)->SetRotation(70);
  CreateObject (SPVM, 1403, 120, -1);
  CreateObject (SBBA, 866, 379, -1);
  CreateObject (SBBA, 635, 378, -1);
  CreateObject (CON1, 897, 310, -1);
  CreateObject (CON1, 603, 310, -1);
  CreateObject (LFTP, 1415, 347, -1);
  CreateObject (LFTP, 85, 373, -1);
  CreateObject (CLVM, 98, 120, -1);
  CreateObject (GSTA, 1344, 359, -1);
  CreateObject (GSTA, 156, 359, -1);
  CreateObject (SEDR, 762, 381, -1);
  CreateObject (SEDR, 739, 381, -1);
  switchright = CreateObject (SLDR, 968, 251, -1)->SetSwitchLock(DIR_Right);
  switchleft  = CreateObject (SLDR, 525, 250, -1)->SetSwitchLock(DIR_Left);
  CreateObject (DEB1, 1027, 643, -1)->SetDir(DIR_Right);
  CreateObject (DEB1, 471, 642, -1);
  CreateObject (VENT, 749, 480, -1)->SetCon(40);
  CreateObject (BART, 773, 90, -1);
  CreateObject (BART, 701, 90, -1);
  CreateObject (LBGH, 1356, 24, -1);
  CreateObject (ALGH, 1343, 268, -1)->SetR(180);
  CreateObject (BLGH, 917, 233, -1);
  CreateObject (BLGH, 1174, 290, -1);
  CreateObject (AWRP, 922, 97, -1)->Set(1260, 640);
  CreateObject (RAI1, 1130, 220, -1)->Set(4);
  CreateObject (VENT, 1365, 475, -1)->SetCon(30);
  CreateObject (FENC, 1100, 380, -1);
  CreateObject (GAT2, 806, 484, -1);
  CreateObject (GAT2, 918, 485, -1);
  CreateObject (GAT2, 1028, 485, -1);
  CreateObject (FIEX, 1259, 111, -1);
  //CreateObject (STRP, 981, 278, -1)->Set(100, RGB(250,25,25), DIR_Left);
  CreateObject (ESGN, 1258, 97, -1);
  CreateObject (MVNT, 1448, 96, -1);
  CreateObject (ENGT, 882, 97, -1)->SetCon(75);
  CreateObject (ENGT, 618, 97, -1)->SetCon(75);
  CreateObject (MVNT, 49, 96, -1);
  CreateObject (ESGN, 241, 97, -1);
  //CreateObject (STRP, 531, 282, -1)->Set(100, RGB(25,250,25), DIR_Right);
  CreateObject (FIEX, 242, 111, -1);
  CreateObject (GAT2, 472, 495, -1);
  CreateObject (GAT2, 582, 495, -1);
  CreateObject (GAT2, 694, 494, -1);
  CreateObject (FENC, 400, 380, -1);
  CreateObject (VENT, 136, 476, -1)->SetCon(30);
  CreateObject (RAI1, 291, 220, -1)->Set(4);
  CreateObject (AWRP, 579, 97, -1)->Set(240, 640);
  CreateObject (AWRP, 750, 601, -1)->Set(750, 200);
  CreateObject (BLGH, 326, 298, -1);
  CreateObject (BLGH, 583, 241, -1);
  CreateObject (ALGH, 157, 267, -1)->SetR(180);
  CreateObject (LBGH, 144, 24, -1);
  for(var lift in FindObjects(Find_ID(LFTP)))
    lift->SetYTop(250);

  PlaceSpawnpoint(ENWP, 220, 110);
  PlaceSpawnpoint(ENAP, 250, 110);
  PlaceSpawnpoint(KRFL, 60, 100);
  PlaceSpawnpoint(NVDN, 160, 230);
  PlaceSpawnpoint(MINE, 50, 430);
  PlaceSpawnpoint(STAP, 670, 370);
  PlaceSpawnpoint(GLWP, 685, 190);
  PlaceSpawnpoint(GRAP, 700, 190);
  PlaceSpawnpoint(FTWP, 620, 670);
  PlaceSpawnpoint(GSAP, 640, 670);
  PlaceSpawnpoint(MIAP, 730, 480);
  PlaceSpawnpoint(ENAP, 720, 30);

  PlaceSpawnpoint(GGWP, 750, 30);
  PlaceSpawnpoint(KLAS, 750, 170);
  PlaceSpawnpoint(PGWP, 750, 370);
  PlaceSpawnpoint(BZWP, 750, 480);
  PlaceSpawnpoint(HARM, 750, 640);

  PlaceSpawnpoint(ENWP, 1280, 110);
  PlaceSpawnpoint(ENAP, 1250, 110);
  PlaceSpawnpoint(KRFL, 1440, 100);
  PlaceSpawnpoint(NVDN, 1340, 230);
  PlaceSpawnpoint(MINE, 1450, 430);
  PlaceSpawnpoint(STAP, 830, 370);
  PlaceSpawnpoint(GLWP, 815, 190);
  PlaceSpawnpoint(GRAP, 800, 190);
  PlaceSpawnpoint(FTWP, 880, 670);
  PlaceSpawnpoint(GSAP, 860, 670);
  PlaceSpawnpoint(MIAP, 770, 480);
  PlaceSpawnpoint(ENAP, 780, 30);

	CreateWaypoints();

  // Die Bombe!
  CreateBomb(750, 70, RGB(200,200,200));

  Goals = CreateArray(3);
}

protected func CreateWaypoints()
{

  var wp1 = CreateWP(750,208);
  var wp2 = CreateWP(750,71);
  var wp3 = CreateWP(750,290);
  var wp4 = CreateWP(1360,110);
  var wp5 = CreateWP(1138,208);
  var wp6 = CreateWP(1061,239);
  var wp7 = CreateWP(1480,197);
  var wp8 = CreateWP(878,270);
  var wp9 = CreateWP(881,126);
  var wp10 = CreateWP(798,369);
  var wp11 = CreateWP(893,198);
  var wp12 = CreateWP(1005,241);
  var wp13 = CreateWP(1377,348);
  var wp14 = CreateWP(805,624);
  var wp15 = CreateWP(1348,228);
  var wp16 = CreateWP(1197,208);
  var wp17 = CreateWP(996,499);
  var wp18 = CreateWP(1098,490);
  var wp19 = CreateWP(1157,602);
  var wp20 = CreateWP(999,298);
  var wp21 = CreateWP(1102,368);
  var wp22 = CreateWP(1041,605);
  var wp23 = CreateWP(771,478);
  var wp24 = CreateWP(848,498);
  var wp25 = CreateWP(977,499);
  var wp26 = CreateWP(875,498);
  var wp27 = CreateWP(1236,476);
  var wp28 = CreateWP(1252,110);
  var wp29 = CreateWP(140,110);
  var wp30 = CreateWP(362,208);
  var wp31 = CreateWP(439,239);
  var wp32 = CreateWP(20,199);
  var wp33 = CreateWP(622,270);
  var wp34 = CreateWP(619,126);
  var wp35 = CreateWP(702,369);
  var wp36 = CreateWP(607,198);
  var wp37 = CreateWP(495,241);
  var wp38 = CreateWP(123,348);
  var wp39 = CreateWP(695,622);
  var wp40 = CreateWP(152,228);
  var wp41 = CreateWP(303,208);
  var wp42 = CreateWP(504,499);
  var wp43 = CreateWP(402,490);
  var wp44 = CreateWP(338,607);
  var wp45 = CreateWP(503,297);
  var wp46 = CreateWP(398,368);
  var wp47 = CreateWP(458,600);
  var wp48 = CreateWP(729,478);
  var wp49 = CreateWP(652,498);
  var wp50 = CreateWP(523,499);
  var wp51 = CreateWP(625,498);
  var wp52 = CreateWP(264,476);
  var wp53 = CreateWP(253,110);
  wp1 -> AddPath(wp8, Path_MoveTo, 1);
  wp1 -> AddPath(wp33, Path_MoveTo, -1);
  wp2 -> AddPath(wp9, Path_MoveTo, 1);
  wp2 -> AddPath(wp34, Path_MoveTo, -1);
  wp3 -> AddPath(wp10, Path_MoveTo, 1);
  wp3 -> AddPath(wp8, Path_MoveTo, 1);
  wp3 -> AddPath(wp35, Path_MoveTo, -1);
  wp3 -> AddPath(wp33, Path_MoveTo, -1);
  wp4 -> AddPath(wp15, Path_MoveTo, 1);
  wp4 -> AddPath(wp28, Path_Jump, -1);
  wp5 -> AddPath(wp6, Path_Jump, -1);
  wp5 -> AddPath(wp16, Path_MoveTo, 1);
  wp6 -> AddPath(wp12, Path_MoveTo, -1);
  wp6 -> AddPath(wp5, Path_Backflip, 1);
  wp7 -> AddPath(wp15, Path_MoveTo, -1);
  wp8 -> AddPath(wp20, Path_MoveTo, 1);
  wp8 -> AddPath(wp3, Path_MoveTo, -1);
  wp8 -> AddPath(wp11, Path_Jump, -1);
  wp8 -> AddPath(wp1, Path_MoveTo, -1);
  wp9 -> AddPath(wp11, Path_MoveTo, 1);
  wp9 -> AddPath(wp19, Path_Jump, 1);
  wp9 -> AddPath(wp2, Path_MoveTo, -1);
  wp10 -> AddPath(wp21, Path_MoveTo, 1);
  wp10 -> AddPath(wp3, Path_MoveTo, -1);
  wp10 -> AddPath(wp35, Path_MoveTo, -1);
  wp11 -> AddPath(wp8, Path_MoveTo, -1);
  wp11 -> AddPath(wp9, Path_Jump, 1);
  wp12 -> AddPath(wp21, Path_MoveTo, 1);
  wp12 -> AddPath(wp8, Path_MoveTo, -1);
  wp12 -> SetArriveCommand(1, 0, "Grab", switchright); 
  wp12 -> SetArriveCommand(1, 1, "Call", switchright, 0, 0, 0, "ControlThrow");
  wp12 -> SetArriveCommand(1, 2, "Ungrab");
  wp12 -> AddPath(wp6, Path_MoveTo, 1);
  wp13 -> AddPath(wp21, Path_MoveTo, -1);
  wp13 -> AddPath(wp15, Path_MoveTo, -1);
  wp14 -> AddPath(wp22, Path_MoveTo, 1);
  wp14 -> AddPath(wp1, Path_Jump, -1);
  wp14 -> AddPath(wp39, Path_MoveTo, -1);
  wp15 -> AddPath(wp13, Path_MoveTo, 1);
  wp15 -> AddPath(wp7, Path_MoveTo, 1);
  wp16 -> AddPath(wp15, Path_Backflip, 1);
  wp16 -> AddPath(wp5, Path_MoveTo, -1);
  wp17 -> AddPath(wp18, Path_Backflip, 1);
  wp17 -> AddPath(wp25, Path_MoveTo, -1);
  wp18 -> AddPath(wp17, Path_Backflip, -1);
  wp18 -> AddPath(wp27, Path_MoveTo, 1);
  wp18 -> AddPath(wp19, Path_MoveTo, 1);
  wp19 -> AddPath(wp27, Path_Jump, 1);
  wp19 -> AddPath(wp22, Path_MoveTo, -1);
  wp20 -> AddPath(wp21, Path_MoveTo, 1);
  wp20 -> AddPath(wp8, Path_MoveTo, -1);
  wp21 -> AddPath(wp27, Path_MoveTo, 1);
  wp21 -> AddPath(wp13, Path_MoveTo, 1);
  wp21 -> AddPath(wp20, Path_MoveTo, -1);
  wp21 -> AddPath(wp12, Path_Jump, -1);
  wp21 -> AddPath(wp10, Path_MoveTo, -1);
  wp21 -> AddPath(wp6, Path_Jump, -1);
  wp22 -> AddPath(wp19, Path_MoveTo, 1);
  wp22 -> AddPath(wp14, Path_MoveTo, -1);
  wp23 -> AddPath(wp24, Path_Jump, 1);
  wp23 -> AddPath(wp48, Path_MoveTo, -1);
  wp24 -> AddPath(wp26, Path_MoveTo, 1);
  wp24 -> AddPath(wp23, Path_Jump, -1);
  wp25 -> AddPath(wp26, Path_Backflip, -1);
  wp25 -> AddPath(wp17, Path_MoveTo, 1);
  wp26 -> AddPath(wp25, Path_Backflip, 1);
  wp26 -> AddPath(wp24, Path_MoveTo, -1);
  wp27 -> AddPath(wp18, Path_MoveTo, -1);
  wp27 -> AddPath(wp19, Path_MoveTo, -1);
  wp27 -> AddPath(wp21, Path_Jump, -1);
  wp28 -> AddPath(wp4, Path_Jump, 1);
  wp28 -> AddPath(wp16, Path_MoveTo, -1);
  wp29 -> AddPath(wp40, Path_MoveTo, -1);
  wp29 -> AddPath(wp53, Path_Jump, 1);
  wp30 -> AddPath(wp31, Path_Jump, 1);
  wp30 -> AddPath(wp41, Path_MoveTo, -1);
  wp31 -> AddPath(wp37, Path_MoveTo, 1);
  wp31 -> AddPath(wp30, Path_Backflip, -1);
  wp32 -> AddPath(wp40, Path_MoveTo, 1);
  wp33 -> AddPath(wp45, Path_MoveTo, -1);
  wp33 -> AddPath(wp3, Path_MoveTo, 1);
  wp33 -> AddPath(wp36, Path_Jump, 1);
  wp33 -> AddPath(wp1, Path_MoveTo, 1);
  wp34 -> AddPath(wp36, Path_MoveTo, -1);
  wp34 -> AddPath(wp44, Path_Jump, -1);
  wp34 -> AddPath(wp2, Path_MoveTo, 1);
  wp35 -> AddPath(wp46, Path_MoveTo, -1);
  wp35 -> AddPath(wp3, Path_MoveTo, 1);
  wp35 -> AddPath(wp10, Path_MoveTo, 1);
  wp36 -> AddPath(wp33, Path_MoveTo, 1);
  wp36 -> AddPath(wp34, Path_Jump, -1);
  wp37 -> AddPath(wp46, Path_MoveTo, -1);
  wp37 -> AddPath(wp33, Path_MoveTo, 1);
  wp37 -> SetArriveCommand(1, 0, "Grab", switchleft);
  wp37 -> SetArriveCommand(1, 1, "Call", switchleft, 0, 0, 0, "ControlThrow");
  wp37 -> SetArriveCommand(1, 2, "Ungrab");
  wp37 -> AddPath(wp31, Path_MoveTo, -1);
  wp38 -> AddPath(wp46, Path_MoveTo, 1);
  wp38 -> AddPath(wp40, Path_MoveTo, 1);
  wp39 -> AddPath(wp1, Path_Jump, 1);
  wp39 -> AddPath(wp14, Path_MoveTo, 1);
  wp39 -> AddPath(wp47, Path_MoveTo, -1);
  wp40 -> AddPath(wp38, Path_MoveTo, -1);
  wp40 -> AddPath(wp32, Path_MoveTo, -1);
  wp41 -> AddPath(wp40, Path_Backflip, -1);
  wp41 -> AddPath(wp30, Path_MoveTo, 1);
  wp42 -> AddPath(wp43, Path_Backflip, -1);
  wp42 -> AddPath(wp50, Path_MoveTo, 1);
  wp43 -> AddPath(wp42, Path_Backflip, 1);
  wp43 -> AddPath(wp52, Path_MoveTo, -1);
  wp43 -> AddPath(wp44, Path_MoveTo, -1);
  wp44 -> AddPath(wp52, Path_Jump, -1);
  wp44 -> AddPath(wp47, Path_MoveTo, 1);
  wp45 -> AddPath(wp46, Path_MoveTo, -1);
  wp45 -> AddPath(wp33, Path_MoveTo, 1);
  wp46 -> AddPath(wp52, Path_MoveTo, -1);
  wp46 -> AddPath(wp38, Path_MoveTo, -1);
  wp46 -> AddPath(wp45, Path_MoveTo, 1);
  wp46 -> AddPath(wp37, Path_Jump, 1);
  wp46 -> AddPath(wp35, Path_MoveTo, 1);
  wp46 -> AddPath(wp31, Path_Jump, 1);
  wp47 -> AddPath(wp44, Path_MoveTo, -1);
  wp47 -> AddPath(wp39, Path_MoveTo, 1);
  wp48 -> AddPath(wp49, Path_Jump, -1);
  wp48 -> AddPath(wp23, Path_MoveTo, 1);
  wp49 -> AddPath(wp51, Path_MoveTo, -1);
  wp49 -> AddPath(wp48, Path_Jump, 1);
  wp50 -> AddPath(wp51, Path_Backflip, 1);
  wp50 -> AddPath(wp42, Path_MoveTo, -1);
  wp51 -> AddPath(wp50, Path_Backflip, -1);
  wp51 -> AddPath(wp49, Path_MoveTo, 1);
  wp52 -> AddPath(wp43, Path_MoveTo, 1);
  wp52 -> AddPath(wp44, Path_MoveTo, 1);
  wp52 -> AddPath(wp46, Path_Jump, 1);
  wp53 -> AddPath(wp29, Path_Jump, -1);
  wp53 -> AddPath(wp41, Path_MoveTo, 1);

}

protected func InitTeamGoal(int iTeam)
{
  var x,y,dir;
  if(iTeam == 1)
  {
    x = 15;
    y = 240;
    dir = DIR_Right;
  }
  if(iTeam == 2)
  {
    x = 1485;
    y = 240;
    dir = DIR_Left;
  }
  CreateBombGate(iTeam, x, y, dir);
}

/* Regelwähler */

public func ChooserFinished()
{
  Arena_ChooserFinished();
}

/* Relaunch */

protected func InitializePlayer(int iPlr, int iX, int iY, object pBase, int iTeam)
{
  // Tore setzen (wenn nicht schon erledigt
  if(!Goals[iTeam])
  {
    Goals[iTeam] = true;
    InitTeamGoal(iTeam);
  }
  for(var i=0, pCrew ; pCrew = GetCrew(iPlr, i) ; i++)
    RelaunchPlayer(iPlr, pCrew, -1, iTeam);
}

public func RelaunchPlayer(int iPlr, object pCrew, int iKiller, int iTeam)
{
  Arena_RelaunchPlayer(iPlr, pCrew, iKiller, iTeam);
}

public func RelaunchPosition(int iTeam)
{
  if(iTeam == 1) return [110,100];
  if(iTeam == 2) return [1390,100];
}
