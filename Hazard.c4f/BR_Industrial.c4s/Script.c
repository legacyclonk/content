/*--Industrial --*/

#strict

static Goals;

protected func Initialize()
{
  SetSkyParallax(0,12,12,0,0,0,0);

  // Deko etc.
  CreateObject (AWRP, 750, 505, -1)->Set(747, 373);
  CreateObject (AWRP, 160, 535, -1)->Set(613, 51);
  CreateObject (AWRP, 1340, 535, -1)->Set(884, 52);
  CreateObject (SEGU, 748, 108, -1)->Arm(RGSA);
  CreateObject (LADR, 965, 108, -1)->Set(5);
  CreateObject (LADR, 539, 108, -1)->Set(5);
  CreateObject (LADR, 699, 230, -1)->Set(5);
  CreateObject (LADR, 802, 228, -1)->Set(5);
  CreateObject (LADR, 808, 145, -1)->Set(10);
  CreateObject (LADR, 696, 143, -1)->Set(10);
  CreateObject (LADR, 1467, 537, -1)->Set(48);
  CreateObject (LADR, 37, 537, -1)->Set(48);
  CreateObject (LADR, 864, 549, -1)->Set(19);
  CreateObject (LADR, 638, 550, -1)->Set(19);
  //CreateObject (OMSG, 85, 469, -1);
  //CreateObject (OMSG, 1450, 536, -1);
  CreateObject (GSTA, 749, 189, -1);
  CreateObject (GSBO, 704, 400, -1);
  CreateObject (GSBO, 802, 400, -1);
  CreateObject (GSBO, 1088, 431, -1);
  CreateObject (GSBO, 1097, 431, -1);
  CreateObject (GSBO, 374, 441, -1);
  CreateObject (GSBO, 365, 441, -1);
  //CreateObject (PLT2, 796, 401, -1);
  //CreateObject (PLT2, 706, 401, -1);
  CreateObject (JMPD, 1324, 330, -1)->Set(100, -40);
  CreateObject (JMPD, 177, 330, -1)->Set(100, 40);
  CreateObject (JMPD, 831, 320, -1)->Set(70, 10);
  CreateObject (JMPD, 670, 320, -1)->Set(70, -10);
  CreateObject (CON1, 1370, 189, -1);
  CreateObject (CON1, 1045, 420, -1);
  CreateObject (CON1, 130, 189, -1);
  CreateObject (CON1, 448, 420, -1);
  CreateObject (LFTP, 965, 205, -1)->DisableVertical();
  CreateObject (LFTP, 533, 203, -1)->DisableVertical();
  //CreateObject (MVNT, 750, 308, -1);
  //CreateObject (PSTR, 751, 66, -1);
  //CreateObject (PSTR, 1431, 318, -1);
  //CreateObject (PSTR, 70, 322, -1);
  //CreateObject (VENT, 1158, 310, -1);
  //CreateObject (VENT, 386, 303, -1);
  //CreateObject (HSGN, 750, 179, -1);
  //CreateObject (ESGN, 1395, 459, -1);
  //CreateObject (FIEX, 1416, 460, -1);
  CreateObject (FIEX, 1374, 460, -1);
  CreateObject (FIEX, 86, 457, -1);
  //CreateObject (FIEX, 128, 457, -1);
  //CreateObject (ESGN, 105, 457, -1);
  //CreateObject (SCR2, 750, 390, -1);
  CreateObject (RAI2, 816, 70, -1)->Set(7);
  CreateObject (RAI2, 546, 69, -1)->Set(7);
  CreateObject (RAI1, 880, 191, -1)->Set(3);
  CreateObject (RAI1, 560, 191, -1)->Set(3);
  //CreateObject (BART, 650, 400, -1);
  //CreateObject (BART, 824, 400, -1);
  //CreateObject (BART, 922, 410, -1);
  //CreateObject (BART, 555, 409, -1);
  CreateObject (ENGT, 1396, 401, -1);
  CreateObject (ENGT, 105, 401, -1);
  //CreateObject (ENGT, 905, 172, -1);
  //CreateObject (ENGT, 590, 170, -1);

  // Spawnpunkte
  PlaceSpawnpoint(STAP, 1384, 147);
  PlaceSpawnpoint(STAP, 1367, 147);
  PlaceSpawnpoint(PGWP, 1352, 147);
  PlaceSpawnpoint(KRFL, 1423, 251);
  PlaceSpawnpoint(KLAS, 1159, 182);
  PlaceSpawnpoint(KSLM, 1397, 401);
  PlaceSpawnpoint(GRAP, 1387, 462);
  PlaceSpawnpoint(GRAP, 1374, 463);
  PlaceSpawnpoint(GLWP, 1395, 462);
  PlaceSpawnpoint(GSAP, 1453, 111);
  PlaceSpawnpoint(GSAP, 1463, 110);
  PlaceSpawnpoint(FTWP, 1439, 110);
  PlaceSpawnpoint(ENAP, 1437, 321);
  PlaceSpawnpoint(ENAP, 1422, 322);
  PlaceSpawnpoint(ENWP, 1407, 322);
  PlaceSpawnpoint(HARM, 751, 279);
  PlaceSpawnpoint(NVDN, 911, 183);
  PlaceSpawnpoint(NVDN, 590, 185);
  PlaceSpawnpoint(STAP, 739, 524);
  PlaceSpawnpoint(STAP, 765, 524);
  PlaceSpawnpoint(MIWP, 751, 524);
  PlaceSpawnpoint(MINE, 938, 61);
  PlaceSpawnpoint(MINE, 561, 62);
  PlaceSpawnpoint(MIAP, 732, 61);
  PlaceSpawnpoint(MIAP, 768, 62);
  PlaceSpawnpoint(BZWP, 751, 61);
  PlaceSpawnpoint(KRFL, 84, 258);
  PlaceSpawnpoint(KLAS, 391, 179);
  PlaceSpawnpoint(KSLM, 105, 401);
  PlaceSpawnpoint(GRAP, 137, 464);
  PlaceSpawnpoint(GRAP, 124, 464);
  PlaceSpawnpoint(GLWP, 113, 463);
  PlaceSpawnpoint(GSAP, 61, 112);
  PlaceSpawnpoint(GSAP, 48, 112);
  PlaceSpawnpoint(FTWP, 35, 112);
  PlaceSpawnpoint(STAP, 116, 149);
  PlaceSpawnpoint(STAP, 133, 149);
  PlaceSpawnpoint(PGWP, 148, 148);
  PlaceSpawnpoint(ENAP, 63, 322);
  PlaceSpawnpoint(ENAP, 78, 322);
  PlaceSpawnpoint(ENWP, 93, 322);

  //Hier werden die Lampen platziert.
  CreateObject(BLGH,589,109,-1);
  CreateObject(BLGH,904,113,-1);
  CreateObject(BLGH,1303,385,-1);
  CreateObject(BLGH,200,385,-1);
  CreateObject(FLGH,136,399,-1)->SetRotation(-10);
  CreateObject(FLGH,1364,399,-1)->SetRotation(+10);
  CreateObject(FLGH,893,182,-1)->SetRotation(-90);
  CreateObject(FLGH,609,181,-1)->SetRotation(+90);
  CreateObject(CLGH,749,423,-1);
  CreateObject(CLGH,674,342,-1);
  CreateObject(CLGH,823,345,-1);
  CreateObject(CLGH,65,143,-1);
  CreateObject(CLGH,1435,143,-1);

  CreateWaypoints();

  // Die Bombe!
  CreateBomb(747, 390, RGB(200,200,200));

  Goals = CreateArray(3);
}

protected func CreateWaypoints()
{
  for(var i=0 ; i < 2 ; i++)
  {
    if(!i) var dir = 1;
    else dir = -1;

  if(!i)
  {
    var wp1 = CreateWP(1500*i+dir*750,480);
    var wp5 = CreateWP(1500*i+dir*750,60);
    var wp16 = CreateWP(1500*i+dir*750,387);
    wp1 -> AddPath(wp16, Path_MoveTo, -1);
  }
  var wp2 = CreateWP(1500*i+dir*725,517);
  var wp3 = CreateWP(1500*i+dir*661,518);
  var wp4 = CreateWP(1500*i+dir*728,178);
  var wp6 = CreateWP(1500*i+dir*66,318);
  var wp7 = CreateWP(1500*i+dir*127,294);
  var wp8 = CreateWP(1500*i+dir*178,313);
  var wp9 = CreateWP(1500*i+dir*572,175);
  var wp10 = CreateWP(1500*i+dir*200,177);
  var wp11 = CreateWP(1500*i+dir*65,454);
  var wp12 = CreateWP(1500*i+dir*62,525);
  var wp13 = CreateWP(1500*i+dir*159,517);
  var wp14 = CreateWP(1500*i+dir*625,55);
  var wp15 = CreateWP(1500*i+dir*510,398);
  var wp17 = CreateWP(1500*i+dir*669,301);
  var wp18 = CreateWP(1500*i+dir*576,398);
  var wp19 = CreateWP(1500*i+dir*661,387);
  var wp20 = CreateWP(1500*i+dir*61,179);
  wp2 -> AddPath(wp3, Path_MoveTo);
  wp2 -> AddPath(wp1, Path_Jump);
  wp3 -> AddPath(wp19, Path_MoveTo);
  wp3 -> AddPath(wp2, Path_MoveTo);
  wp4 -> AddPath(wp4, Path_Jump);
  wp4 -> AddPath(wp14, Path_Jump);
  wp4 -> AddPath(wp9, Path_Backflip, 0,0,0, 1);
  wp4 -> AddPath(wp16, Path_MoveTo);
  wp5 -> AddPath(wp14, Path_MoveTo);
  wp6 -> AddPath(wp7, Path_MoveTo);
  wp6 -> AddPath(wp11, Path_MoveTo);
  wp6 -> AddPath(wp20, Path_MoveTo);
  wp7 -> AddPath(wp6, Path_MoveTo);
  wp7 -> AddPath(wp8, Path_MoveTo);
  wp7 -> AddPath(wp15, Path_Jump);
  wp8 -> AddPath(wp9, Path_MoveTo);
  wp9 -> AddPath(wp10, Path_Lift);
  wp9 -> AddPath(wp16, Path_MoveTo);
  wp10 -> AddPath(wp9, Path_Lift);
  wp10 -> AddPath(wp20, Path_MoveTo);
  wp11 -> AddPath(wp6, Path_MoveTo);
  wp11 -> AddPath(wp12, Path_MoveTo);
  wp11 -> AddPath(wp15, Path_MoveTo);
  wp12 -> AddPath(wp11, Path_MoveTo);
  wp12 -> AddPath(wp13, Path_MoveTo);
  wp13 -> AddPath(wp14, Path_MoveTo);
  wp14 -> AddPath(wp15, Path_MoveTo);
  wp14 -> AddPath(wp16, Path_MoveTo);
  wp14 -> AddPath(wp5, Path_MoveTo);
  wp14 -> AddPath(wp4, Path_MoveTo);
  wp15 -> AddPath(wp15, Path_MoveTo);
  wp15 -> AddPath(wp15, Path_MoveTo);
  wp15 -> AddPath(wp18, Path_MoveTo);
  wp15 -> AddPath(wp11, Path_MoveTo);
  wp16 -> AddPath(wp17, Path_Jump);
  wp16 -> AddPath(wp19, Path_MoveTo);
  wp17 -> AddPath(wp9, Path_MoveTo);
  wp18 -> AddPath(wp19, Path_Jump);
  wp18 -> AddPath(wp15, Path_MoveTo);
  wp19 -> AddPath(wp16, Path_MoveTo);
  wp19 -> AddPath(wp18, Path_Jump);
  wp19 -> AddPath(wp3, Path_MoveTo);
  wp20 -> AddPath(wp10, Path_MoveTo);
  wp20 -> AddPath(wp6, Path_MoveTo);
  if(!i)
  {
    var spec1 = wp2; var spec2 = wp4;
  }
  else
  {
    wp2->AddPath(spec1, Path_MoveTo, 0,0,0, 1);
    wp4->AddPath(spec2, Path_MoveTo, 0,0,0, 1);
  }
  }
}

protected func InitTeamGoal(int iTeam)
{
  var x,y,dir;
  if(iTeam == 1)
  {
    x = 126;
    y = 331;
    dir = DIR_Right;
  }
  if(iTeam == 2)
  {
    x = 1374;
    y = 331;
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
  if(iTeam == 1) return [65,320];
  if(iTeam == 2) return [1435,320];
}
