/*-- Fabrik --*/

#strict

func Initialize()
{
  // Deko
  CreateObject(CON1, 958, 740, -1);
  CreateObject(CON1, 650, 750, -1);
  CreateObject(CON1, 758, 750, -1);
  CreateObject(CON1, 480, 180, -1);
  CreateObject(CON1, 530, 180, -1);
  CreateObject(CON1, 495, 144, -1);
  CreateObject(CRN1, 335, 68, -1)->Set(8);
  CreateObject(CRN1, 680, 68, -1)->Set(10);
  //CreateObject(ENGT, 240, 160, -1);
  //CreateObject(HSGN, 820, 430, -1);
  //CreateObject(GSTA, 160, 450, -1);
  //CreateObject(VENT, 200, 410, -1)->SetCon(25);
  //CreateObject(CLVM, 240, 740, -1);
  //CreateObject(SPVM, 200, 740, -1);
  //CreateObject(GAT2, 540, 720, -1);
  
  CreateObject(GSTA, 278, 590, -1);
  CreateObject(GSTA, 228, 590, -1);
  CreateObject(GSTA, 178, 590, -1);
  
  CreateObject(LBGH, 250, 610, -1);
  CreateObject(LBGH, 730, 660, -1);
  CreateObject(LBGH, 520, 460, -1);
  CreateObject(LBGH, 680, 320, -1);
  CreateObject(LBGH, 450, 200, -1)->Set(150);
  CreateObject(LBGH, 730, 60, -1);
  CreateObject(LBGH, 350, 60, -1);
  
  CreateObject(FENC, 150, 593, -1);
  CreateObject(FENC, 210, 593, -1);
  CreateObject(FENC, 270, 593, -1);
  CreateObject(FENC, 330, 593, -1);
  
  //CreateObject(ENGT, 155, 660, -1)->SetClrModulation(RGB(190,185,185));
  
  //CreateObject(BART, 610, 640, -1)->SetRail([1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1]);

  CreateObject(LADR, 255, 410, -1)->Set(9);
  CreateObject(LADR, 195, 130, -1)->Set(9);
  CreateObject(LADR, 460, 330, -1)->Set(5);
  CreateObject(LADR, 825, 100, -1)->Set(7);
  CreateObject(LADR, 570, 520, -1)->Set(6);

  // Jumppads
  CreateObject(JMPD, 986, 716, -1)->Set(75, -25);
  CreateObject(JMPD, 490, 750, -1)->Set(80, 25);
  CreateObject(JMPD, 482, 750, -1)->Set(100, -25);
  CreateObject(JMPD, 618, 650, -1)->Set(99, 35);
  CreateObject(JMPD, 906, 500, -1)->Set(80, -45, -90);
  CreateObject(JMPD, 886, 650, -1)->Set(80, -15);
  CreateObject(JMPD, 834, 520, -1)->Set(60, 25, 90);
  CreateObject(JMPD, 986, 450, -1)->Set(120, -75);
  CreateObject(JMPD, 350, 600, -1)->Set(100, 60);
  CreateObject(JMPD, 466, 540, -1)->Set(75, -20, -90);
  CreateObject(JMPD, 386, 460, -1)->Set(100, 35);
  CreateObject(JMPD, 124, 360, -1)->Set(70, 15);
  CreateObject(JMPD, 825, 510, -1)->Set(110, 10);
  CreateObject(JMPD, 254, 300, -1)->Set(85, 32);
  CreateObject(JMPD, 750, 310, -1)->Set(85, 15);
  CreateObject(JMPD, 726, 450, -1)->Set(80, 45);
  CreateObject(JMPD, 494, 450, -1)->Set(80, 15);
  // Waffen & Ausrüstung
  PlaceSpawnpoint(PGWP, 240, 730);
  PlaceSpawnpoint(STAP, 220, 730);
  PlaceSpawnpoint(GRAP, 510, 730);
  PlaceSpawnpoint(GLWP, 710, 630);
  PlaceSpawnpoint(GRAP, 730, 630);
  PlaceSpawnpoint(KSLM, 940, 700);
  PlaceSpawnpoint(BZWP, 940, 430);
  PlaceSpawnpoint(MIAP, 960, 430);
  PlaceSpawnpoint(GLWP, 200, 170);
  PlaceSpawnpoint(GRAP, 220, 170);
  PlaceSpawnpoint(KLAS, 970, 270);
  PlaceSpawnpoint(DRSU, 950, 340);
  PlaceSpawnpoint(ENWP, 600, 430);
  PlaceSpawnpoint(ENAP, 620, 430);
  PlaceSpawnpoint(MINE, 160, 420);
  PlaceSpawnpoint(GGWP, 200, 250);
  PlaceSpawnpoint(ENAP, 180, 250);
  PlaceSpawnpoint(KLAS, 490, 110);
  PlaceSpawnpoint(MEDI, 680, 170);
  PlaceSpawnpoint(MIWP, 930, 170);
  PlaceSpawnpoint(STAP, 910, 170);
  PlaceSpawnpoint(HARM, 710, 745);
  PlaceSpawnpoint(JTPK, 150, 580);
  PlaceSpawnpoint(GSAP, 170, 580);
  PlaceSpawnpoint(KRFL, 140, 510);
  PlaceSpawnpoint(ENAP, 230, 340);
  PlaceSpawnpoint(GRAP, 670, 290);
  PlaceSpawnpoint(STAP, 610, 170);
  PlaceBonusSpawnpoint([AEXB], 330, 40, 2000);
  PlaceBonusSpawnpoint([AMPB], 700, 40, 2000);
  PlaceBonusSpawnpoint([BSKB,HSTB,HELB,RPFB,INVB], 70, 510);
  
  // Reek
  CreateObject(FNNL,906,51,-1)->SetCon(200);
  CreateObject(FNNL,976,169,-1)->SetCon(150);
  CreateObject(FNNL,486,338,-1)->SetCon(150);
  CreateObject(FNNL,346,392,-1)->SetCon(250);
  CreateObject(FNNL,47,346,-1)->SetCon(150);
  CreateObject(FNNL,136,114,-1)->SetCon(200);
  CreateObject(FNNL,196,38,-1)->SetCon(150);
  
  // Sky
  SetSkyParallax(0,15,15);
  // Wegpunkte
  CreateWaypoints();
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
    wp->AddWare(GLWP);  // Granatenwerfer
    wp->AddWare(MIWP);  // Minigun
    wp->AddWare(BZWP);  // Bazooka
    wp->AddWare(KLAS);  // Laser
    wp->AddWare(KRFL);  // Waffenteile
    wp->AddWare(FLSH);  // Taschenlampe
    wp->AddWare(JTPK);  // Jetpack
    wp->AddWare(HARM);  // Rüstung
    wp->AddWare(HSHD);  // Schild
    wp->AddWare(DRSU);  // Drone
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
  if(!rand)   return [50,720];
  if(!--rand) return [610,690];
  if(!--rand) return [820,610];
  if(!--rand) return [260,570];
  if(!--rand) return [480,500];
  if(!--rand) return [970,330];
  if(!--rand) return [850,160];
  if(!--rand) return [390,140];
}

/* KI */

protected func CreateWaypoints()
{
  var wp1 = CreateWP(326,579);
  var wp2 = CreateWP(330,38);
  var wp3 = CreateWP(698,37);
  var wp4 = CreateWP(190,169);
  var wp5 = CreateWP(304,171);
  var wp6 = CreateWP(495,99);
  var wp7 = CreateWP(758,170);
  var wp8 = CreateWP(958,170);
  var wp9 = CreateWP(832,169);
  var wp10 = CreateWP(750,288);
  var wp11 = CreateWP(799,288);
  var wp12 = CreateWP(712,288);
  var wp13 = CreateWP(646,287);
  var wp14 = CreateWP(521,431);
  var wp15 = CreateWP(937,268);
  var wp16 = CreateWP(949,338);
  var wp17 = CreateWP(729,428);
  var wp18 = CreateWP(496,431);
  var wp19 = CreateWP(969,267);
  var wp20 = CreateWP(686,430);
  var wp21 = CreateWP(985,430);
  var wp22 = CreateWP(932,429);
  var wp23 = CreateWP(389,438);
  var wp24 = CreateWP(83,509);
  var wp25 = CreateWP(113,437);
  var wp26 = CreateWP(484,511);
  var wp27 = CreateWP(622,630);
  var wp28 = CreateWP(883,630);
  var wp29 = CreateWP(835,629);
  var wp30 = CreateWP(669,629);
  var wp31 = CreateWP(982,697);
  var wp32 = CreateWP(936,696);
  var wp33 = CreateWP(709,727);
  var wp34 = CreateWP(229,728);
  var wp35 = CreateWP(62,728);
  var wp36 = CreateWP(133,580);
  var wp37 = CreateWP(445,727);
  var wp38 = CreateWP(477,728);
  var wp39 = CreateWP(498,727);
  var wp40 = CreateWP(534,727);
  var wp41 = CreateWP(234,579);
  var wp42 = CreateWP(140,509);
  var wp43 = CreateWP(362,576);
  var wp44 = CreateWP(293,438);
  var wp45 = CreateWP(233,339);
  var wp46 = CreateWP(125,335);
  var wp47 = CreateWP(202,251);
  var wp48 = CreateWP(308,280);
  var wp49 = CreateWP(258,281);
  var wp50 = CreateWP(384,169);
  wp1 -> AddPath(wp41, Path_MoveTo, -1);
  wp1 -> AddPath(wp43, Path_MoveTo, 1);
  wp1 -> AddPath(wp37, Path_Jump, 1);
  wp2 -> AddPath(wp6, Path_MoveTo, 1);
  wp2 -> AddPath(wp4, Path_MoveTo, -1);
  wp3 -> AddPath(wp6, Path_MoveTo, -1);
  wp3 -> AddPath(wp9, Path_MoveTo, 1);
  wp4 -> AddPath(wp5, Path_MoveTo, 1);
  wp4 -> AddPath(wp45, Path_MoveTo, 1);
  wp4 -> AddPath(wp35, Path_Jump, -1);
  wp5 -> AddPath(wp50, Path_Jump, 1);
  wp5 -> AddPath(wp4, Path_MoveTo, -1);
  wp6 -> AddPath(wp50, Path_MoveTo, -1);
  wp6 -> AddPath(wp7, Path_MoveTo, 1);
  wp6 -> AddPath(wp3, Path_Jump, 1, 10, 3);
  wp6 -> AddPath(wp2, Path_Jump, -1, 10, 2);
  wp7 -> AddPath(wp9, Path_Jump, 1);
  wp7 -> AddPath(wp6, Path_MoveTo, -1);
  wp7 -> AddPath(wp32, Path_MoveTo, 1);
  wp8 -> AddPath(wp9, Path_MoveTo, -1);
  wp8 -> AddPath(wp19, Path_MoveTo, 1);
  wp9 -> AddPath(wp8, Path_MoveTo, 1);
  wp9 -> AddPath(wp7, Path_Jump, -1);
  wp10 -> AddPath(wp8, Path_MoveTo, 1);
  wp11 -> AddPath(wp12, Path_Jump, -1);
  wp11 -> AddPath(wp16, Path_Backflip, 1);
  wp11 -> AddPath(wp10, Path_MoveTo, -1);
  wp12 -> AddPath(wp13, Path_MoveTo, -1);
  wp12 -> AddPath(wp11, Path_Jump, 1);
  wp12 -> AddPath(wp10, Path_MoveTo, 1);
  wp13 -> AddPath(wp14, Path_Jump, -1);
  wp13 -> AddPath(wp12, Path_MoveTo, 1);
  wp13 -> AddPath(wp44, Path_Jump, -1, 40, 1);
  wp13 -> AddPath(wp37, Path_Backflip, -1);
  wp14 -> AddPath(wp18, Path_MoveTo, -1);
  wp14 -> AddPath(wp37, Path_Jump, -1);
  wp14 -> AddPath(wp20, Path_MoveTo, 1);
  wp15 -> AddPath(wp30, Path_MoveTo, -1);
  wp15 -> AddPath(wp11, Path_Backflip, -1);
  wp16 -> AddPath(wp19, Path_Jump, -1);
  wp17 -> AddPath(wp16, Path_MoveTo, 1);
  wp18 -> AddPath(wp13, Path_MoveTo, 1);
  wp19 -> AddPath(wp15, Path_MoveTo, -1);
  wp20 -> AddPath(wp48, Path_MoveTo, -1, 1, 10);
  wp20 -> AddPath(wp17, Path_MoveTo, 1);
  wp20 -> AddPath(wp14, Path_MoveTo, -1);
  wp20 -> AddPath(wp14, Path_MoveTo, -1);
  wp20 -> AddPath(wp29, Path_Jump, 1);
  wp21 -> AddPath(wp20, Path_MoveTo, -1);
  wp22 -> AddPath(wp21, Path_MoveTo, 1);
  wp22 -> AddPath(wp19, Path_Jump, -1);
  wp22 -> AddPath(wp30, Path_Backflip, -1);
  wp23 -> AddPath(wp13, Path_MoveTo, 1);
  wp24 -> AddPath(wp25, Path_MoveTo, 1);
  wp24 -> AddPath(wp35, Path_MoveTo, -1);
  wp25 -> AddPath(wp44, Path_MoveTo, 1);
  wp25 -> AddPath(wp24, Path_MoveTo, -1, -1, 5);
  wp25 -> AddPath(wp35, Path_MoveTo, -1);
  wp26 -> AddPath(wp41, Path_Backflip, -1);
  wp26 -> AddPath(wp40, Path_MoveTo, 1);
  wp27 -> AddPath(wp20, Path_MoveTo, 1);
  wp28 -> AddPath(wp22, Path_MoveTo, 1);
  wp29 -> AddPath(wp30, Path_MoveTo, -1);
  wp29 -> AddPath(wp28, Path_MoveTo, 1);
  wp29 -> AddPath(wp32, Path_Jump, 1);
  wp30 -> AddPath(wp29, Path_MoveTo, 1);
  wp30 -> AddPath(wp27, Path_MoveTo, -1);
  wp30 -> AddPath(wp40, Path_Jump, -1);
  wp31 -> AddPath(wp29, Path_MoveTo, -1);
  wp32 -> AddPath(wp33, Path_MoveTo, -1);
  wp32 -> AddPath(wp31, Path_MoveTo, 1);
  wp33 -> AddPath(wp40, Path_MoveTo, -1);
  wp33 -> AddPath(wp32, Path_MoveTo, 1);
  wp34 -> AddPath(wp37, Path_MoveTo, 1);
  wp34 -> AddPath(wp35, Path_MoveTo, -1);
  wp35 -> AddPath(wp34, Path_MoveTo, 1);
  wp35 -> AddPath(wp36, Path_Jump, 1);
  wp36 -> AddPath(wp35, Path_MoveTo, -1);
  wp36 -> AddPath(wp41, Path_MoveTo, 1);
  wp37 -> AddPath(wp34, Path_MoveTo, -1);
  wp37 -> AddPath(wp40, Path_Jump, 1);
  wp37 -> AddPath(wp38, Path_MoveTo, 1);
  wp38 -> AddPath(wp41, Path_MoveTo, -1);
  wp39 -> AddPath(wp30, Path_MoveTo, 1);
  wp40 -> AddPath(wp33, Path_MoveTo, 1);
  wp40 -> AddPath(wp37, Path_Jump, -1);
  wp40 -> AddPath(wp39, Path_MoveTo, -1);
  wp41 -> AddPath(wp36, Path_MoveTo, -1);
  wp41 -> AddPath(wp42, Path_Jump, 10, 2, 1);
  wp41 -> AddPath(wp1, Path_MoveTo, 1);
  wp42 -> AddPath(wp41, Path_MoveTo, 1);
  wp43 -> AddPath(wp44, Path_MoveTo, -1);
  wp44 -> AddPath(wp45, Path_Jump, -1);
  wp44 -> AddPath(wp25, Path_MoveTo, -1);
  wp44 -> AddPath(wp23, Path_MoveTo, 1);
  wp45 -> AddPath(wp44, Path_MoveTo, 1);
  wp45 -> AddPath(wp46, Path_MoveTo, -1);
  wp46 -> AddPath(wp47, Path_MoveTo, 1);
  wp47 -> AddPath(wp48, Path_MoveTo, 1);
  wp47 -> AddPath(wp45, Path_MoveTo, 1, 1, 10);
  wp47 -> AddPath(wp14, Path_Jump, 1, 10, 1);
  wp48 -> AddPath(wp49, Path_MoveTo, -1);
  wp48 -> AddPath(wp47, Path_Jump, -1);
  wp48 -> AddPath(wp20, Path_MoveTo, 1, 1, 10);
  wp49 -> AddPath(wp50, Path_MoveTo, 1);
  wp50 -> AddPath(wp6, Path_MoveTo, 1);
  wp50 -> AddPath(wp48, Path_MoveTo, -1);
  wp50 -> AddPath(wp5, Path_Jump, -1);
}
