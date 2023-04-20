/*-- Schlachtfest --*/

#strict

func Initialize()
{
  CreateObject (CON1, 601, 70, -1);
  CreateObject (FLGH, 782, 739, -1);
  CreateObject (LADR, 541, 387, -1)->Set(9);
  CreateObject (LADR, 123, 740, -1)->Set(14);
  CreateObject (LADR, 956, 745, -1)->Set(19);
  CreateObject (LADR, 1157, 610, -1)->Set(23);
  CreateObject (LADR, 916, 430, -1)->Set(20);
  CreateObject (LADR, 136, 470, -1)->Set(22);
  CreateObject (LADR, 526, 630, -1)->Set(16);
  CreateObject (LADR, 701, 631, -1)->Set(16);
  CreateObject (LADR, 584, 491, -1)->Set(14);
  CreateObject (LADR, 692, 471, -1)->Set(13);
  CreateObject (ENGT, 287, 285, -1)->SetCon(50);
  CreateObject (CCP2, 924, 632, -1);
  CreateObject (GSTA, 1033, 429, -1);
  CreateObject (GSTA, 997, 429, -1);
  CreateObject (LCKR, 319, 400, -1);
  CreateObject (LCKR, 337, 400, -1);
  CreateObject (LCKR, 944, 430, -1);
  CreateObject (CHAR, 319, 629, -1);
  CreateObject (GSBO, 212, 400, -1);
  CreateObject (GSBO, 223, 400, -1);
  CreateObject (CRN1, 522, 98, -1)->Set(5);
  CreateObject (CCP1, 253, 180, -1);
  CreateObject (CRAT, 401, 400, -1);
  CreateObject (CRAT, 478, 400, -1);
  CreateObject (TABB, 1085, 133, -1);
  CreateObject (TABL, 346, 629, -1);
  CreateObject (JMPD, 302, 180, -1)->Set(75,35);
  CreateObject (SBBA, 797, 261, -1);
  CreateObject (CLVM, 293, 630, -1);
  CreateObject (SPVB, 1117, 130, -1);
  CreateObject (SLDR, 1075, 170, -1);
  CreateObject (SLDR, 175, 220, -1);
  CreateObject (SLDR, 215, 220, -1);
  CreateObject (STDR, 232, 300, -1);
  CreateObject (LBDR, 843, 581, -1);
  CreateObject (LBDR, 242, 631, -1);
  CreateObject (LBDR, 414, 630, -1);
  CreateObject (LFTP, 1105, 291, -1)->SetAutoLimits();
  CreateObject (LFTP, 35, 190, -1)->SetAutoLimits();
  CreateObject (RAI3, 759, 190, -1);
  CreateObject (BLGH, 1106, 327, -1);
  CreateObject (BLGH, 955, 545, -1);
  CreateObject (CLGH, 332, 581, -1);
  CreateObject (CLGH, 300, 451, -1);
  CreateObject (CLGH, 389, 451, -1);
  CreateObject (BLGH, 87, 118, -1);
  CreateObject (BLGH, 216, 117, -1);
  CreateObject (HSGN, 1005, 287, -1)->SetCon(50);
  CreateObject (AWRP, 271, 298, -1)->Set(1091, 94);
  CreateObject (AWRP, 755, 578, -1)->Set(66, 466);
  CreateObject (FIEX, 422, 305, -1);
  CreateObject (VENT, 1002, 390, -1)->SetCon(25);
  CreateObject (GAT2, 199, 714, -1);
  CreateObject (LCOM, 385, 615, -1);
  CreateObject (LCOM, 385, 629, -1);
  CreateObject (RAI1, 129, 151, -1)->SetRailLength(2);
  CreateObject (VENT, 279, 70, -1)->SetCon(25);
  CreateObject (RAI1, 169, 180, -1)->SetRailLength(3);
  CreateObject (WSHB, 871, 130, -1);
  CreateObject (STLC, 0, 0, -1);
  CreateObject(CONS, 490, 60, -1)->Set(CreateObject(_GNC,1,1,-1));
  CreateObject(SEGU, 430, 121, -1)->Arm(MGSA);
  CreateObject(SEGU, 630, 121, -1)->Arm(MGSA);
  
  // Waffen und Ausrüstung
  PlaceSpawnpoint(ENWP, 240, 170);
  PlaceSpawnpoint(ENAP, 225, 170);
  PlaceSpawnpoint(STAP, 190, 50);
  PlaceSpawnpoint(KSLM, 230, 290);
  PlaceSpawnpoint(GRAP, 210, 290);
  PlaceSpawnpoint(GRAP, 360, 500);
  PlaceSpawnpoint(GLWP, 380, 500);
  PlaceSpawnpoint(MIAP, 60, 470);
  PlaceSpawnpoint(BZWP, 320, 750);
  PlaceSpawnpoint(MIAP, 340, 750);
  PlaceSpawnpoint(STAP, 890, 740);
  PlaceSpawnpoint(PGWP, 1030, 420);
  PlaceSpawnpoint(STAP, 1050, 420);
  PlaceSpawnpoint(KLAS, 810, 570);
  PlaceSpawnpoint(GGWP, 540, 60);
  PlaceSpawnpoint(ENAP, 560, 60);
  PlaceSpawnpoint(FTWP, 1070, 120);
  PlaceSpawnpoint(GSAP, 1090, 120);
  PlaceSpawnpoint(GSAP, 1180, 90);
  PlaceSpawnpoint(JTPK, 1060, 280);
  PlaceSpawnpoint(MEDI, 640, 250);
  PlaceSpawnpoint(MINE, 430, 260);
  PlaceSpawnpoint(KRFL, 300, 390);
  PlaceSpawnpoint(HSHD, 710, 370);
  PlaceSpawnpoint(MEZL, 670, 639);
  PlaceSpawnpoint(GSAP, 655, 639);

  PlaceWP();
  //Debug
  //WaypointsVisible(1);
}

func PlaceWP() {
  var wp1 = CreateWP(1055,746);
  var wp2 = CreateWP(914,492);
  var wp3 = CreateWP(1118,116);
  var wp4 = CreateWP(1071,277);
  var wp5 = CreateWP(1074,418);
  var wp6 = CreateWP(945,419);
  var wp7 = CreateWP(882,490);
  var wp8 = CreateWP(802,239);
  var wp9 = CreateWP(876,190);
  var wp10 = CreateWP(1176,158);
  var wp11 = CreateWP(1055,160);
  var wp12 = CreateWP(1140,597);
  var wp13 = CreateWP(761,564);
  var wp14 = CreateWP(1009,674);
  var wp15 = CreateWP(919,618);
  var wp16 = CreateWP(542,730);
  var wp17 = CreateWP(420,751);
  var wp18 = CreateWP(936,693);
  var wp19 = CreateWP(95,617);
  var wp20 = CreateWP(183,616);
  var wp21 = CreateWP(938,749);
  var wp22 = CreateWP(866,736);
  var wp23 = CreateWP(481,707);
  var wp24 = CreateWP(152,751);
  var wp25 = CreateWP(166,553);
  var wp26 = CreateWP(124,483);
  var wp27 = CreateWP(66,466);
  var wp28 = CreateWP(230,479);
  var wp29 = CreateWP(504,500);
  var wp30 = CreateWP(551,501);
  var wp31 = CreateWP(511,620);
  var wp32 = CreateWP(521,394);
  var wp33 = CreateWP(70,286);
  var wp34 = CreateWP(263,284);
  var wp35 = CreateWP(160,288);
  var wp36 = CreateWP(161,390);
  var wp37 = CreateWP(854,280);
  var wp38 = CreateWP(724,369);
  var wp39 = CreateWP(561,391);
  var wp40 = CreateWP(333,229);
  var wp41 = CreateWP(141,207);
  var wp42 = CreateWP(70,207);
  var wp43 = CreateWP(639,250);
  var wp44 = CreateWP(338,80);
  var wp45 = CreateWP(71,60);
  var wp46 = CreateWP(268,169);
  var wp47 = CreateWP(431,257);
  var wp48 = CreateWP(557,58);
  var wp49 = CreateWP(526,307);
  var wp50 = CreateWP(399,303);
  var wp51 = CreateWP(603,499);
  var wp52 = CreateWP(666,470);
  var wp53 = CreateWP(673,641);
  var wp54 = CreateWP(724,502);
  var wp55 = CreateWP(583,709);
  wp1 -> AddPath(wp21, Path_MoveTo, -1);
  wp2 -> AddPath(wp15, Path_MoveTo, -1);
  wp3 -> AddPath(wp10, Path_MoveTo, 1);
  wp3 -> AddPath(wp9, Path_MoveTo, -1);
  wp4 -> AddPath(wp11, Path_MoveTo, -1);
  wp4 -> AddPath(wp10, Path_MoveTo, 1);
  wp4 -> AddPath(wp5, Path_MoveTo, 1);
  wp4 -> AddPath(wp37, Path_MoveTo, -1);
  wp5 -> AddPath(wp6, Path_MoveTo, -1);
  wp5 -> AddPath(wp4, Path_MoveTo, -1);
  wp5 -> AddPath(wp12, Path_MoveTo, 1);
  wp6 -> AddPath(wp5, Path_MoveTo, 1);
  wp6 -> AddPath(wp7, Path_MoveTo, -1);
  wp6 -> AddPath(wp37, Path_Jump, -1);
  wp6 -> AddPath(wp4, Path_Jump, -1);
  wp6 -> AddPath(wp52, Path_MoveTo, -1);
  wp6 -> AddPath(wp54, Path_MoveTo, -1);
  wp7 -> AddPath(wp2, Path_MoveTo, 1);
  wp8 -> AddPath(wp37, Path_MoveTo, -1);
  wp8 -> AddPath(wp43, Path_MoveTo, 1);
  wp8 -> AddPath(wp9, Path_Jump, 1);
  wp9 -> AddPath(wp11, Path_MoveTo, 1);
  wp9 -> AddPath(wp8, Path_MoveTo, -1);
  wp10 -> AddPath(wp4, Path_MoveTo, -1);
  wp10 -> AddPath(wp3, Path_Jump, -1);
  wp10 -> AddPath(wp11, Path_MoveTo, -1);
  wp11 -> AddPath(wp9, Path_MoveTo, -1);
  wp11 -> AddPath(wp4, Path_MoveTo, 1);
  wp11 -> AddPath(wp10, Path_MoveTo, 1);
  wp12 -> AddPath(wp14, Path_MoveTo, -1);
  wp12 -> AddPath(wp5, Path_Jump, 1);
  wp13 -> AddPath(wp27, Path_MoveTo, -1);
  wp14 -> AddPath(wp12, Path_Jump, 1);
  wp14 -> AddPath(wp21, Path_MoveTo, -1);
  wp14 -> AddPath(wp18, Path_MoveTo, -1);
  wp14 -> AddPath(wp1, Path_MoveTo, 1);
  wp15 -> AddPath(wp13, Path_Jump, -1);
  wp15 -> AddPath(wp13, Path_MoveTo, -1);
  wp16 -> AddPath(wp23, Path_Jump, -1);
  wp16 -> AddPath(wp17, Path_MoveTo, -1);
  wp16 -> AddPath(wp55, Path_MoveTo, 1);
  wp17 -> AddPath(wp24, Path_MoveTo, -1);
  wp17 -> AddPath(wp23, Path_Jump, 1);
  wp17 -> AddPath(wp16, Path_MoveTo, 1);
  wp18 -> AddPath(wp15, Path_Jump, 1);
  wp18 -> AddPath(wp14, Path_MoveTo, 1);
  wp19 -> AddPath(wp24, Path_MoveTo, 1);
  wp19 -> AddPath(wp25, Path_Jump, 1);
  wp19 -> AddPath(wp20, Path_MoveTo, 1);
  wp20 -> AddPath(wp24, Path_MoveTo, -1);
  wp20 -> AddPath(wp31, Path_MoveTo, 1);
  wp20 -> AddPath(wp28, Path_Jump, 1);
  wp20 -> AddPath(wp19, Path_MoveTo, -1);
  wp21 -> AddPath(wp15, Path_Jump, 1);
  wp21 -> AddPath(wp22, Path_MoveTo, -1);
  wp21 -> AddPath(wp14, Path_Jump, 1);
  wp22 -> AddPath(wp18, Path_Jump, 1);
  wp22 -> AddPath(wp55, Path_MoveTo, -1);
  wp22 -> AddPath(wp21, Path_MoveTo, 1);
  wp23 -> AddPath(wp17, Path_MoveTo, -1);
  wp23 -> AddPath(wp16, Path_MoveTo, 1);
  wp24 -> AddPath(wp17, Path_MoveTo, 1);
  wp24 -> AddPath(wp20, Path_Jump, -1);
  wp24 -> AddPath(wp19, Path_Jump, -1);
  wp25 -> AddPath(wp19, Path_MoveTo, 1);
  wp25 -> AddPath(wp20, Path_MoveTo, 1);
  wp26 -> AddPath(wp27, Path_MoveTo, -1);
  wp26 -> AddPath(wp28, Path_Backflip, 1);
  wp26 -> AddPath(wp25, Path_MoveTo, 1);
  wp26 -> AddPath(wp36, Path_Jump, 1);
  wp26 -> AddPath(wp35, Path_Jump, 1);
  wp26 -> AddPath(wp33, Path_Jump, 1);
  wp27 -> AddPath(wp26, Path_MoveTo, 1);
  wp28 -> AddPath(wp29, Path_MoveTo, 1);
  wp28 -> AddPath(wp26, Path_Backflip, -1);
  wp28 -> AddPath(wp25, Path_MoveTo, 1);
  wp29 -> AddPath(wp31, Path_MoveTo, 1);
  wp29 -> AddPath(wp28, Path_MoveTo, -1);
  wp29 -> AddPath(wp30, Path_MoveTo, 1);
  wp30 -> AddPath(wp31, Path_MoveTo, -1);
  wp30 -> AddPath(wp29, Path_MoveTo, -1);
  wp30 -> AddPath(wp39, Path_Jump, 1);
  wp30 -> AddPath(wp51, Path_MoveTo, 1);
  wp31 -> AddPath(wp20, Path_MoveTo, -1);
  wp31 -> AddPath(wp30, Path_Jump, 1);
  wp31 -> AddPath(wp29, Path_Jump, 1);
  wp31 -> AddPath(wp55, Path_MoveTo, 1);
  wp32 -> AddPath(wp36, Path_MoveTo, -1);
  wp32 -> AddPath(wp49, Path_Jump, 1);
  wp32 -> AddPath(wp39, Path_MoveTo, 1);
  wp33 -> AddPath(wp36, Path_MoveTo, 1);
  wp33 -> AddPath(wp26, Path_MoveTo, 1);
  wp33 -> AddPath(wp42, Path_MoveTo, 1);
  wp33 -> AddPath(wp45, Path_MoveTo, 1);
  wp33 -> AddPath(wp42, Path_MoveTo, 0);
  wp33 -> AddPath(wp35, Path_MoveTo, 1);
  wp34 -> AddPath(wp3, Path_MoveTo, 1);
  wp35 -> AddPath(wp26, Path_MoveTo, -1);
  wp35 -> AddPath(wp36, Path_MoveTo, 1);
  wp35 -> AddPath(wp34, Path_MoveTo, 1);
  wp35 -> AddPath(wp33, Path_MoveTo, -1);
  wp36 -> AddPath(wp33, Path_Jump, -1);
  wp36 -> AddPath(wp35, Path_Jump, -1);
  wp36 -> AddPath(wp32, Path_MoveTo, 1);
  wp37 -> AddPath(wp38, Path_MoveTo, -1);
  wp37 -> AddPath(wp8, Path_Jump, -1);
  wp37 -> AddPath(wp4, Path_MoveTo, 1);
  wp37 -> AddPath(wp6, Path_MoveTo, 1);
  wp38 -> AddPath(wp37, Path_Jump, 1);
  wp38 -> AddPath(wp52, Path_MoveTo, -1);
  wp38 -> AddPath(wp51, Path_MoveTo, -1);
  wp39 -> AddPath(wp49, Path_Jump, -1);
  wp39 -> AddPath(wp32, Path_MoveTo, -1);
  wp39 -> AddPath(wp30, Path_MoveTo, -1);
  wp40 -> AddPath(wp47, Path_Jump, 1);
  wp40 -> AddPath(wp41, Path_MoveTo, -1);
  wp40 -> AddPath(wp50, Path_MoveTo, 1);
  wp41 -> AddPath(wp42, Path_MoveTo, -1);
  wp41 -> AddPath(wp40, Path_MoveTo, 1);
  wp41 -> AddPath(wp46, Path_Jump, 1);
  wp42 -> AddPath(wp33, Path_MoveTo, -1);
  wp42 -> AddPath(wp41, Path_MoveTo, 1);
  wp42 -> AddPath(wp45, Path_MoveTo, 1);
  wp42 -> AddPath(wp33, Path_MoveTo, 0);
  wp43 -> AddPath(wp8, Path_MoveTo, 1);
  wp43 -> AddPath(wp49, Path_MoveTo, -1);
  wp44 -> AddPath(wp45, Path_MoveTo, -1);
  wp44 -> AddPath(wp48, Path_Jump, 1);
  wp44 -> AddPath(wp48, Path_Backflip, 1);
  wp45 -> AddPath(wp33, Path_MoveTo, -1);
  wp45 -> AddPath(wp44, Path_MoveTo, 1);
  wp45 -> AddPath(wp42, Path_MoveTo, -1);
  wp46 -> AddPath(wp48, Path_MoveTo, 1);
  wp46 -> AddPath(wp41, Path_MoveTo, -1);
  wp47 -> AddPath(wp40, Path_Jump, -1);
  wp47 -> AddPath(wp50, Path_MoveTo, -1);
  wp48 -> AddPath(wp40, Path_MoveTo, -1);
  wp48 -> AddPath(wp43, Path_MoveTo, 1);
  wp49 -> AddPath(wp50, Path_Jump, -1);
  wp49 -> AddPath(wp50, Path_Backflip, -1);
  wp49 -> AddPath(wp32, Path_MoveTo, -1);
  wp49 -> AddPath(wp39, Path_MoveTo, 1);
  wp49 -> AddPath(wp43, Path_MoveTo, 1);
  wp50 -> AddPath(wp47, Path_Jump, 1);
  wp50 -> AddPath(wp32, Path_MoveTo, 1);
  wp50 -> AddPath(wp40, Path_MoveTo, -1);
  wp51 -> AddPath(wp52, Path_Jump, 1);
  wp51 -> AddPath(wp30, Path_MoveTo, -1);
  wp51 -> AddPath(wp39, Path_Jump, -1);
  wp51 -> AddPath(wp53, Path_MoveTo, 1);
  wp52 -> AddPath(wp6, Path_MoveTo, 1);
  wp52 -> AddPath(wp38, Path_Jump, 1);
  wp52 -> AddPath(wp38, Path_Jump, 1);
  wp52 -> AddPath(wp51, Path_MoveTo, -1);
  wp52 -> AddPath(wp39, Path_Jump, -1);
  wp52 -> AddPath(wp54, Path_MoveTo, 1);
  wp53 -> AddPath(wp13, Path_Jump, 1);
  wp53 -> AddPath(wp54, Path_Jump, 1);
  wp53 -> AddPath(wp55, Path_MoveTo, -1);
  wp54 -> AddPath(wp53, Path_MoveTo, -1);
  wp54 -> AddPath(wp52, Path_MoveTo, -1);
  wp54 -> AddPath(wp6, Path_MoveTo, 1);
  wp55 -> AddPath(wp22, Path_MoveTo, 1);
  wp55 -> AddPath(wp16, Path_MoveTo, -1);
  wp55 -> AddPath(wp53, Path_Jump, 1);
  wp55 -> AddPath(wp31, Path_Jump, -1);
}

/* Regelwähler */

public func ChooserFinished()
{
  Arena_ChooserFinished();

  var wp = FindObject(WPCH);
  if(wp)
  {
    // und begrenzte Waren...
    wp->RemoveWare();
    wp->AddWare(PIWP);  // Pistole
    wp->AddWare(PGWP);  // Pumpgun
    wp->AddWare(FTWP);  // Flammenwerfer
    wp->AddWare(MIWP);  // Minigun
    wp->AddWare(MEZL);  // Mezl
    wp->AddWare(KLAS);  // Laser
    wp->AddWare(KRFL);  // Waffenteile

    wp->AddWare(FLSH);  // Taschenlampe
    wp->AddWare(HARM);  // Rüstung
    wp->AddWare(MINE);  // Mine
    //wp->SortWare();
  }
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
  var rand = Random(8);
  if(!rand)   return [270,150];
  if(!--rand) return [170,280];
  if(!--rand) return [60,460];
  if(!--rand) return [380,620];
  if(!--rand) return [50,730];
  if(!--rand) return [600,680];
  if(!--rand) return [970,400];
  if(!--rand) return [1120,100];
}
