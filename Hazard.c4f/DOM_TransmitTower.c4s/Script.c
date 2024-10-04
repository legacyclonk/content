/*-- Domination --*/

#strict

func Initialize()
{
  CreateObject (CCP1, 96, 990, -1);
  CreateObject (GLST, 890, 990, -1);
  CreateObject (FLGH, 600, 679, -1)->SetRotation(50);
  CreateObject (FLGH, 402, 679, -1)->SetRotation(-50);
  CreateObject (GSBO, 500, 410, -1);
  CreateObject (CRAT, 307, 850, -1);
  CreateObject (CRAT, 692, 850, -1);
  CreateObject (CRAT, 501, 680, -1);
  CreateObject (LCKR, 43, 920, -1);
  CreateObject (GSTA, 79, 319, -1);
  CreateObject (GSTA, 910, 319, -1);
  CreateObject (JMPD, 19, 569, -1)->Set(90, 50, 90);
  CreateObject (JMPD, 302, 649, -1)->Set(60, -40, -90);
  CreateObject (JMPD, 25, 320, -1)->Set(80, 30);
  CreateObject (JMPD, 975, 320, -1)->Set(80, -30);
  CreateObject (JMPD, 981, 569, -1)->Set(90, -50, -90);
  CreateObject (JMPD, 698, 649, -1)->Set(60, 40, 90);
  CreateObject (JMPD, 25, 850, -1)->Set(100, 30);
  CreateObject (JMPD, 975, 850, -1)->Set(100, -30);
  CreateObject (LADR, 265, 210, -1)->Set(8);
  CreateObject (LADR, 140, 280, -1)->Set(9);
  CreateObject (LADR, 735, 210, -1)->Set(8);
  CreateObject (LADR, 860, 280, -1)->Set(9);
  CreateObject (LADR, 500, 380, -1)->Set(15);
  CreateObject (LADR, 635, 270, -1)->Set(16);
  CreateObject (LADR, 365, 270, -1)->Set(16);
  CreateObject (LADR, 565, 645, -1)->Set(48);
  CreateObject (LADR, 435, 645, -1)->Set(48);
  CreateObject (LADR, 825, 990, -1)->Set(19);
  CreateObject (LADR, 175, 990, -1)->Set(19);
  CreateObject (LADR, 630, 930, -1)->Set(11);
  CreateObject (LADR, 370, 930, -1)->Set(11);
  CreateObject (LFTP, 225, 860, -1)->SetLimits(396,846);
  CreateObject (LFTP, 775, 860, -1)->SetLimits(396,846);
  CreateObject (CLGH, 500, 870, -1);
  CreateObject (BLGH, 950, 760, -1);
  CreateObject (BLGH, 50, 760, -1);
  CreateObject (CLGH, 79, 860, -1);
  CreateObject (CLGH, 921, 860, -1);
  CreateObject (LBGH, 500, 160, -1);
  CreateObject (BLGH, 880, 120, -1);
  CreateObject (BLGH, 120, 120, -1);
  CreateObject (FENC, 691, 390, -1);
  CreateObject (FENC, 751, 390, -1);
  CreateObject (FENC, 811, 390, -1);
  CreateObject (FENC, 310, 390, -1);
  CreateObject (FENC, 250, 390, -1);
  CreateObject (FENC, 190, 390, -1);
  CreateObject (FIEX, 954, 911, -1);
  CreateObject (ESGN, 953, 890, -1);
  CreateObject (STRP, 505, 901, -1)->Set(50, RGB(45,76,154), 1);
  CreateObject (SCR2, 501, 755, -1)->RemoveFrame();
  CreateObject (RAI2, 286, 501, -1)->SetRail([1,2,1,2,1,2,1,2,1,2,1,2,1,1,1,2,1,2,1,2,1,2,1,2,1,1,1,2,1,2,1,2,1,2,1,2,1,2,1]);
  CreateObject (RAI1, 665, 680, -1)->SetRailLength(4);
  CreateObject (RAI1, 257, 680, -1)->SetRailLength(4);
  CreateObject (HSGN, 501, 590, -1);
  CreateObject (MVNT, 500, 454, -1)->SetCon(50);
  CreateObject (VENT, 500, 234, -1)->SetCon(50);
  // Spawnpunkte
  PlaceSpawnpoint(ENWP, 90, 80);
  PlaceSpawnpoint(ENAP, 110, 80);
  PlaceSpawnpoint(KSLM, 60, 140);
  PlaceSpawnpoint(MINE, 220, 80);
  PlaceSpawnpoint(PGWP, 70, 310);
  PlaceSpawnpoint(STAP, 90, 310);
  PlaceSpawnpoint(GRAP, 210, 200);
  PlaceSpawnpoint(ENWP, 100, 910);
  PlaceSpawnpoint(ENAP, 120, 910);
  PlaceSpawnpoint(KRFL, 100, 980);
  PlaceSpawnpoint(PGWP, 280, 670);
  PlaceSpawnpoint(STAP, 300, 670);
  PlaceSpawnpoint(LMIN, 50, 670);
  PlaceSpawnpoint(GRAP, 290, 810);
  PlaceSpawnpoint(STAP, 370, 340);

  PlaceSpawnpoint(GLWP, 500, 490);
  PlaceSpawnpoint(MEDI, 480, 490);
  PlaceSpawnpoint(GRAP, 520, 490);
  PlaceSpawnpoint(BZWP, 500, 920);
  PlaceSpawnpoint(MIAP, 480, 920);
  PlaceSpawnpoint(MIAP, 520, 920);
  PlaceSpawnpoint(MIWP, 500, 50);
  PlaceSpawnpoint(STAP, 435, 60);
  PlaceSpawnpoint(STAP, 565, 60);
  PlaceSpawnpoint(HARM, 500, 400);
  PlaceSpawnpoint(KLAS, 500, 640);

  PlaceSpawnpoint(ENWP, 910, 80);
  PlaceSpawnpoint(ENAP, 890, 80);
  PlaceSpawnpoint(KSLM, 940, 140);
  PlaceSpawnpoint(MINE, 780, 80);
  PlaceSpawnpoint(PGWP, 930, 310);
  PlaceSpawnpoint(STAP, 910, 310);
  PlaceSpawnpoint(GRAP, 790, 200);
  PlaceSpawnpoint(ENWP, 900, 910);
  PlaceSpawnpoint(ENAP, 880, 910);
  PlaceSpawnpoint(KRFL, 900, 980);
  PlaceSpawnpoint(PGWP, 720, 670);
  PlaceSpawnpoint(STAP, 700, 670);
  PlaceSpawnpoint(LMIN, 950, 670);
  PlaceSpawnpoint(GRAP, 710, 810);
  PlaceSpawnpoint(STAP, 630, 340);

  PlaceSpawnpoint(LMIN, 500, 710);
  PlaceSpawnpoint(SMIN, 90, 490);
  PlaceSpawnpoint(SMIN, 980, 490);
	
  PlaceSpawnpoint(DRSU, 100, 590);
  PlaceSpawnpoint(DRSU, 885, 590);
	
  PlaceSpawnpoint(MEDI, 500, 150);
  PlaceSpawnpoint(MEDI, 500, 850);
	
  PlaceBonusSpawnpoint([AMPB,BSKB,HSTB,HSTB,HELB,RPFB], 775, 290);
  PlaceBonusSpawnpoint([AMPB,BSKB,HSTB,HSTB,HELB,RPFB], 225, 290);
  // Das Wichtigste:
  CreateDominationPoint(500, 150, 1);
  CreateDominationPoint(500, 485, 5);
  CreateDominationPoint(500, 850, 4);

  CreateWaypoints();
  
  // Sky
  SetSkyParallax(0,20,20);
  
  return(1);
}

public func CreateWaypoints()
{
  var wp1 = CreateWP(279,140);
  var wp2 = CreateWP(344,140);
  var wp3 = CreateWP(388,260);
  var wp4 = CreateWP(108,80);
  var wp5 = CreateWP(22,140);
  var wp6 = CreateWP(204,200);
  var wp7 = CreateWP(100,308);
  var wp8 = CreateWP(404,341);
  var wp9 = CreateWP(278,380);
  var wp10 = CreateWP(184,381);
  var wp11 = CreateWP(411,670);
  var wp12 = CreateWP(324,670);
  var wp13 = CreateWP(301,490);
  var wp14 = CreateWP(52,590);
  var wp15 = CreateWP(174,590);
  var wp16 = CreateWP(501,920);
  var wp17 = CreateWP(275,668);
  var wp18 = CreateWP(200,850);
  var wp19 = CreateWP(140,980);
  var wp20 = CreateWP(99,910);
  var wp21 = CreateWP(499,839);
  var wp22 = CreateWP(103,838);
  var wp23 = CreateWP(721,140);
  var wp24 = CreateWP(656,140);
  var wp25 = CreateWP(612,260);
  var wp26 = CreateWP(500,490);
  var wp27 = CreateWP(892,80);
  var wp28 = CreateWP(978,140);
  var wp29 = CreateWP(796,200);
  var wp30 = CreateWP(900,308);
  var wp31 = CreateWP(596,341);
  var wp32 = CreateWP(722,380);
  var wp33 = CreateWP(816,381);
  var wp34 = CreateWP(589,670);
  var wp35 = CreateWP(676,670);
  var wp36 = CreateWP(699,490);
  var wp37 = CreateWP(948,590);
  var wp38 = CreateWP(826,590);
  var wp39 = CreateWP(725,668);
  var wp40 = CreateWP(803,850);
  var wp41 = CreateWP(860,980);
  var wp42 = CreateWP(901,910);
  var wp43 = CreateWP(893,838);
  var wp44 = CreateWP(497,50);
  var wp45 = CreateWP(501,140);
  wp1 -> AddPath(wp2, Path_MoveTo, 1);
  wp1 -> AddPath(wp6, Path_MoveTo, -1);
  wp2 -> AddPath(wp3, Path_MoveTo, 1);
  wp2 -> AddPath(wp1, Path_MoveTo, -1);
  wp2 -> AddPath(wp44, Path_Jump, 1);
  wp2 -> AddPath(wp45, Path_MoveTo, 1);
  wp3 -> AddPath(wp8, Path_MoveTo, 1);
  wp3 -> AddPath(wp2, Path_Jump, -1);
  wp3 -> AddPath(wp25, Path_MoveTo, 1);
  wp4 -> AddPath(wp5, Path_MoveTo, -1);
  wp4 -> AddPath(wp6, Path_MoveTo, 1);
  wp5 -> AddPath(wp4, Path_Jump, 1);
  wp5 -> AddPath(wp6, Path_MoveTo, 1);
  wp6 -> AddPath(wp9, Path_MoveTo, 1);
  wp6 -> AddPath(wp5, Path_Jump, -1);
  wp6 -> AddPath(wp1, Path_MoveTo, 1);
  wp7 -> AddPath(wp6, Path_Jump, 1);
  wp8 -> AddPath(wp9, Path_MoveTo, -1);
  wp8 -> AddPath(wp3, Path_Jump, 1);
  wp8 -> AddPath(wp26, Path_MoveTo, 1);
  wp8 -> AddPath(wp11, Path_MoveTo, 1);
  wp9 -> AddPath(wp8, Path_MoveTo, 1);
  wp9 -> AddPath(wp10, Path_Jump, -1);
  wp9 -> AddPath(wp17, Path_MoveTo, -1);
  wp9 -> AddPath(wp15, Path_MoveTo, -1);
  wp9 -> AddPath(wp18, Path_MoveTo, -1);
  wp10 -> AddPath(wp7, Path_Jump, -1);
  wp10 -> AddPath(wp9, Path_Jump, 1);
  wp10 -> AddPath(wp15, Path_MoveTo, -1);
  wp10 -> AddPath(wp17, Path_MoveTo, 1);
  wp10 -> AddPath(wp18, Path_MoveTo, 1);
  wp11 -> AddPath(wp18, Path_MoveTo, -1);
  wp11 -> AddPath(wp12, Path_Jump, -1);
  wp11 -> AddPath(wp26, Path_Jump, 1);
  wp11 -> AddPath(wp8, Path_Jump, 1);
  wp11 -> AddPath(wp3, Path_Jump, 1);
  wp11 -> AddPath(wp34, Path_MoveTo, 1);
  wp12 -> AddPath(wp17, Path_MoveTo, -1);
  wp12 -> AddPath(wp11, Path_Jump, 1);
  wp13 -> AddPath(wp14, Path_Backflip, -1);
  wp13 -> AddPath(wp15, Path_Jump, -1);
  wp13 -> AddPath(wp26, Path_MoveTo, 1);
  wp13 -> AddPath(wp17, Path_MoveTo, -1);
  wp14 -> AddPath(wp15, Path_MoveTo, 1);
  wp14 -> AddPath(wp13, Path_Jump, -1);
  wp15 -> AddPath(wp17, Path_MoveTo, 1);
  wp15 -> AddPath(wp14, Path_MoveTo, -1);
  wp15 -> AddPath(wp10, Path_MoveTo, 1);
  wp15 -> AddPath(wp9, Path_MoveTo, 1);
  wp16 -> AddPath(wp18, Path_MoveTo, -1);
  wp16 -> AddPath(wp40, Path_MoveTo, -1);
  wp17 -> AddPath(wp12, Path_MoveTo, 1);
  wp17 -> AddPath(wp15, Path_MoveTo, 1);
  wp17 -> AddPath(wp9, Path_MoveTo, 1);
  wp17 -> AddPath(wp9, Path_MoveTo, 1);
  wp17 -> AddPath(wp10, Path_MoveTo, -1);
  wp17 -> AddPath(wp18, Path_MoveTo, -1);
  wp18 -> AddPath(wp20, Path_MoveTo, -1);
  wp18 -> AddPath(wp19, Path_MoveTo, -1);
  wp18 -> AddPath(wp21, Path_MoveTo, 1);
  wp18 -> AddPath(wp16, Path_MoveTo, 1);
  wp18 -> AddPath(wp22, Path_MoveTo, -1);
  wp18 -> AddPath(wp17, Path_MoveTo, 1);
  wp18 -> AddPath(wp15, Path_MoveTo, -1);
  wp19 -> AddPath(wp18, Path_MoveTo, 1);
  wp20 -> AddPath(wp18, Path_MoveTo, 1);
  wp21 -> AddPath(wp18, Path_MoveTo, -1);
  wp21 -> AddPath(wp40, Path_MoveTo, -1);
  wp22 -> AddPath(wp17, Path_Jump, -1);
  wp23 -> AddPath(wp24, Path_MoveTo, -1);
  wp23 -> AddPath(wp29, Path_MoveTo, 1);
  wp24 -> AddPath(wp25, Path_MoveTo, -1);
  wp24 -> AddPath(wp23, Path_MoveTo, 1);
  wp24 -> AddPath(wp45, Path_MoveTo, -1);
  wp24 -> AddPath(wp44, Path_Jump, -1);
  wp25 -> AddPath(wp31, Path_MoveTo, -1);
  wp25 -> AddPath(wp24, Path_Jump, 1);
  wp25 -> AddPath(wp3, Path_MoveTo, -1);
  wp26 -> AddPath(wp36, Path_MoveTo, 1);
  wp26 -> AddPath(wp31, Path_Jump, 1);
  wp26 -> AddPath(wp25, Path_Jump, 1);
  wp26 -> AddPath(wp34, Path_MoveTo, 1);
  wp26 -> AddPath(wp13, Path_MoveTo, -1);
  wp26 -> AddPath(wp8, Path_Jump, -1);
  wp26 -> AddPath(wp3, Path_Jump, -1);
  wp26 -> AddPath(wp11, Path_MoveTo, -1);
  wp27 -> AddPath(wp28, Path_MoveTo, 1);
  wp27 -> AddPath(wp29, Path_MoveTo, -1);
  wp28 -> AddPath(wp27, Path_Jump, -1);
  wp28 -> AddPath(wp29, Path_MoveTo, -1);
  wp29 -> AddPath(wp32, Path_MoveTo, -1);
  wp29 -> AddPath(wp28, Path_Jump, 1);
  wp29 -> AddPath(wp23, Path_MoveTo, -1);
  wp30 -> AddPath(wp29, Path_Jump, -1);
  wp31 -> AddPath(wp32, Path_MoveTo, 1);
  wp31 -> AddPath(wp25, Path_Jump, -1);
  wp31 -> AddPath(wp26, Path_MoveTo, -1);
  wp31 -> AddPath(wp34, Path_MoveTo, -1);
  wp32 -> AddPath(wp31, Path_MoveTo, 1);
  wp32 -> AddPath(wp33, Path_Jump, 1);
  wp32 -> AddPath(wp39, Path_MoveTo, 1);
  wp32 -> AddPath(wp38, Path_MoveTo, 1);
  wp32 -> AddPath(wp40, Path_MoveTo, 1);
  wp33 -> AddPath(wp30, Path_Jump, 1);
  wp33 -> AddPath(wp32, Path_Jump, -1);
  wp33 -> AddPath(wp38, Path_MoveTo, 1);
  wp33 -> AddPath(wp39, Path_MoveTo, -1);
  wp33 -> AddPath(wp40, Path_MoveTo, -1);
  wp34 -> AddPath(wp40, Path_MoveTo, 1);
  wp34 -> AddPath(wp35, Path_Jump, 1);
  wp34 -> AddPath(wp26, Path_Jump, -1);
  wp34 -> AddPath(wp31, Path_Jump, -1);
  wp34 -> AddPath(wp25, Path_Jump, -1);
  wp34 -> AddPath(wp11, Path_MoveTo, -1);
  wp35 -> AddPath(wp39, Path_MoveTo, 1);
  wp35 -> AddPath(wp34, Path_Jump, -1);
  wp36 -> AddPath(wp39, Path_MoveTo, 1);
  wp36 -> AddPath(wp37, Path_Backflip, 1);
  wp36 -> AddPath(wp38, Path_Jump, 1);
  wp36 -> AddPath(wp26, Path_MoveTo, -1);
  wp37 -> AddPath(wp38, Path_MoveTo, -1);
  wp37 -> AddPath(wp36, Path_Jump, 1);
  wp38 -> AddPath(wp39, Path_MoveTo, -1);
  wp38 -> AddPath(wp37, Path_MoveTo, 1);
  wp38 -> AddPath(wp33, Path_MoveTo, -1);
  wp38 -> AddPath(wp32, Path_MoveTo, -1);
  wp39 -> AddPath(wp35, Path_MoveTo, -1);
  wp39 -> AddPath(wp38, Path_MoveTo, -1);
  wp39 -> AddPath(wp32, Path_MoveTo, -1);
  wp39 -> AddPath(wp32, Path_MoveTo, -1);
  wp39 -> AddPath(wp33, Path_MoveTo, 1);
  wp39 -> AddPath(wp40, Path_MoveTo, 1);
  wp40 -> AddPath(wp42, Path_MoveTo, 1);
  wp40 -> AddPath(wp41, Path_MoveTo, 1);
  wp40 -> AddPath(wp21, Path_MoveTo, -1);
  wp40 -> AddPath(wp16, Path_MoveTo, -1);
  wp40 -> AddPath(wp43, Path_MoveTo, 1);
  wp40 -> AddPath(wp39, Path_MoveTo, -1);
  wp40 -> AddPath(wp38, Path_MoveTo, 1);
  wp41 -> AddPath(wp40, Path_MoveTo, -1);
  wp42 -> AddPath(wp40, Path_MoveTo, -1);
  wp43 -> AddPath(wp39, Path_Jump, 1);
  wp44 -> AddPath(wp2, Path_MoveTo, 1);
  wp44 -> AddPath(wp24, Path_MoveTo, -1);
  wp45 -> AddPath(wp24, Path_MoveTo, 1);
  wp45 -> AddPath(wp2, Path_MoveTo, -1);
}

/* Regelwähler */

public func ChooserFinished()
{
  Arena_ChooserFinished();
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
  var rand = Random(3);
  if(iTeam == 1)
  {
    if(!rand--)      return [70,80];
    else if(!rand--) return [30,910];
    else             return [70,590];
  }
  if(iTeam == 2)
  {
    if(!rand--)      return [930,80];
    else if(!rand--) return [970,910];
    else             return [930,590];
  }
}

