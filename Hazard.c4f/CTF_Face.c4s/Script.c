/*-- Face --*/

#strict

static Flags;

func Initialize()
{
  // Objekte
  CreateObject (FLGH, 1335, 316, -1)->SetRotation(0,180);
  CreateObject (FLGH, 165, 316, -1)->SetRotation(0,180);
  CreateObject (LADR, 1335, 450, -1)->Set(13);
  CreateObject (LADR, 165, 450, -1)->Set(13);
  var liftp = CreateObject (LFTP, 805, 274, -1);
  liftp->DisableVertical();
  liftp->ContactLeft();
  liftp = CreateObject (LFTP, 695, 274, -1);
  liftp->DisableVertical();
  liftp->ContactRight();
  //CreateObject (STLH, 1014, 460, -1);
  //CreateObject (STLH, 486, 460, -1);
  CreateObject (CLGH, 1237, 193, -1);
  CreateObject (CLGH, 263, 193, -1);
  CreateObject (BLGH, 750, 450, -1);
  CreateObject (BLGH, 750, 320, -1);
  CreateObject (HSGN, 750, 375, -1)->SetCon(75);
  CreateObject (FENC, 1261, 260, -1);
  CreateObject (FENC, 1201, 260, -1);
  CreateObject (FENC, 239, 260, -1);
  CreateObject (FENC, 299, 260, -1);
  CreateObject (ENGT, 1306, 385, -1)->SetCon(50);
  CreateObject (ENGT, 194, 385, -1)->SetCon(50);
  // Warps
  CreateObject(AWRP, 260, 450, -1)->Set(210, 250);
  CreateObject(AWRP, 170, 120, -1)->Set(210, 250);
  CreateObject(AWRP, 170, 280, -1)->Set(200, 490);
  CreateObject(AWRP, 170, 220, -1)->Set(230, 90);
  CreateObject(AWRP, 110, 480, -1)->Set(230, 90);

  CreateObject(AWRP, 1240, 450, -1)->Set(1290, 250);
  CreateObject(AWRP, 1330, 120, -1)->Set(1290, 250);
  CreateObject(AWRP, 1330, 280, -1)->Set(1300, 490);
  CreateObject(AWRP, 1330, 220, -1)->Set(1270, 90);
  CreateObject(AWRP, 1390, 480, -1)->Set(1270, 90);
  // Spawnpunkte
  PlaceSpawnpoint(GLWP, 280, 610);
  PlaceSpawnpoint(GRAP, 300, 610);
  PlaceSpawnpoint(ENWP, 240, 490);
  PlaceSpawnpoint(ENAP, 260, 490);
  PlaceSpawnpoint(STAP, 220, 340);
  PlaceSpawnpoint(PGWP, 240, 340);
  PlaceSpawnpoint(STAP, 260, 340);
  PlaceSpawnpoint(BZWP, 300, 250);
  PlaceSpawnpoint(MIAP, 280, 250);
  PlaceSpawnpoint(GGWP, 300, 80);
  PlaceSpawnpoint(ENAP, 280, 80);
  PlaceSpawnpoint(MEDI, 110, 50);
  PlaceSpawnpoint(KRFL, 160, 50);
  PlaceSpawnpoint(MINE, 360, 400);

  PlaceSpawnpoint(KLAS, 750, 250);

  PlaceSpawnpoint(GLWP, 1220, 610);
  PlaceSpawnpoint(GRAP, 1200, 610);
  PlaceSpawnpoint(ENWP, 1260, 490);
  PlaceSpawnpoint(ENAP, 1240, 490);
  PlaceSpawnpoint(STAP, 1280, 340);
  PlaceSpawnpoint(PGWP, 1260, 340);
  PlaceSpawnpoint(STAP, 1240, 340);
  PlaceSpawnpoint(BZWP, 1200, 250);
  PlaceSpawnpoint(MIAP, 1220, 250);
  PlaceSpawnpoint(GGWP, 1200, 80);
  PlaceSpawnpoint(ENAP, 1220, 80);
  PlaceSpawnpoint(MEDI, 1390, 50);
  PlaceSpawnpoint(KRFL, 1340, 50);
  PlaceSpawnpoint(MINE, 1140, 400);
  
  PlaceSpawnpoint(MEZL, 750, 417);
  PlaceSpawnpoint(GSAP, 750, 447);

	PlaceVehicleSpawnpoint(TANK,105,550);
	PlaceVehicleSpawnpoint(TANK,1395,550);
	
  SetSkyParallax(0,150,20);

  Flags = CreateArray(3);
  CreateWaypoints();
  return(1);
}

/* Regelwähler */

public func ChooserFinished()
{
  Arena_ChooserFinished();

  // Bei Waffenwahl werden alle Nicht-Munispawns entfernt
  var wp = FindObject(WPCH);
  if(wp)
  {
    // und begrenzte Waren...
    wp->RemoveWare();
    wp->AddWare(PIWP);  // Pistole
    wp->AddWare(PGWP);  // Pumpgun
    wp->AddWare(GLWP);  // Granatenwerfer
    wp->AddWare(MIWP);  // Minigun
    wp->AddWare(GGWP);  // Partikelkanone
    wp->AddWare(BZWP);  // Bazooka
    wp->AddWare(KLAS);  // Laser
    wp->AddWare(KRFL);  // Waffenteile
    wp->AddWare(FLSH);  // Taschenlampe
    wp->AddWare(MINE);  // Lasermine
    //wp->SortWare();
  }
}

/* Flaggen */

protected func InitTeamFlag(int iTeam)
{
  var x,y,color;
  if(iTeam == 1)
  {
    x = 180;
    y = 500;
    color = RGB(255,0,0);
  }
  if(iTeam == 2)
  {
    x = 1320;
    y = 500;
    color = RGB(0,0,255);
  }
  CreateFlag(iTeam, x, y, color);
}

/* Relaunch */

protected func InitializePlayer(int iPlr, int iX, int iY, object pBase, int iTeam)
{
  // Flagge setzen (wenn nicht schon erledigt
  if(!Flags[iTeam])
  {
    Flags[iTeam] = true;
    InitTeamFlag(iTeam);
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
  if(iTeam == 1)
  {
    if(Random(3)) return [30,480];
    else          return [30,25];
  }
  if(iTeam == 2)
  {
    if(Random(3)) return [1470,480];
    else          return [1470,25];
  }
}

/* KI */

protected func CreateWaypoints()
{
  for(var i=0 ; i < 2 ; i++)
  {
    if(!i) var dir = 1;
    else dir = -1;

  var wp1 = CreateWP(1500*i+dir*344,489);
  var wp2 = CreateWP(1500*i+dir*735,248);
  var wp3 = CreateWP(1500*i+dir*400,120);
  var wp4 = CreateWP(1500*i+dir*91,49);
  var wp5 = CreateWP(1500*i+dir*177,48);
  var wp6 = CreateWP(1500*i+dir*244,89);
  var wp7 = CreateWP(1500*i+dir*320,78);
  var wp8 = CreateWP(1500*i+dir*181,103);
  var wp9 = CreateWP(1500*i+dir*181,203);
  var wp10 = CreateWP(1500*i+dir*227,251);
  var wp11 = CreateWP(1500*i+dir*318,248);
  var wp12 = CreateWP(1500*i+dir*181,264);
  var wp13 = CreateWP(1500*i+dir*273,434);
  var wp14 = CreateWP(1500*i+dir*118,447);
  var wp15 = CreateWP(1500*i+dir*341,338);
  var wp16 = CreateWP(1500*i+dir*302,339);
  var wp17 = CreateWP(1500*i+dir*186,339);
  var wp18 = CreateWP(1500*i+dir*180,490);
//  var wp19 = CreateWP(1500*i+dir*792,447);
  var wp20 = CreateWP(1500*i+dir*707,447);
  var wp21 = CreateWP(1500*i+dir*516,529);
  var wp22 = CreateWP(1500*i+dir*290,609);
  var wp23 = CreateWP(1500*i+dir*31,489);

  wp1 -> AddPath(wp18);
//  wp1 -> AddPath(wp18, Path_Backflip);
  wp1 -> AddPath(wp16, Path_Jump);
  wp1 -> AddPath(wp21);
  wp2 -> AddPath(wp21);
  wp2 -> AddPath(wp11, Path_Lift);
  wp2 -> AddPath(wp1, 0, 0, 30);
  wp3 -> AddPath(wp20);
  wp3 -> AddPath(wp7);
  wp3 -> AddPath(wp2, Path_Jump, 0, 40);
  wp4 -> AddPath(wp5);
  wp5 -> AddPath(wp6);
  wp5 -> AddPath(wp4);
  wp6 -> AddPath(wp8);
  wp6 -> AddPath(wp7);
  wp6 -> AddPath(wp5, Path_Jump);
  wp7 -> AddPath(wp6);
  wp7 -> AddPath(wp3);
  wp8 -> AddPath(wp10);
  wp9 -> AddPath(wp6);
  wp10 -> AddPath(wp12);
  wp10 -> AddPath(wp9, Path_Jump);
  wp10 -> AddPath(wp11);
  wp11 -> AddPath(wp10);
  wp11 -> AddPath(wp21);
  wp11 -> AddPath(wp2, Path_Lift);
  wp12 -> AddPath(wp18);
  wp13 -> AddPath(wp10);
  wp14 -> AddPath(wp6);
  wp15 -> AddPath(wp16);
  wp15 -> AddPath(wp21);
  wp16 -> AddPath(wp1);
  wp16 -> AddPath(wp15);
  wp16 -> AddPath(wp17);
  wp17 -> AddPath(wp18);
  wp17 -> AddPath(wp16);
  wp18 -> AddPath(wp13);
  wp18 -> AddPath(wp17, Path_Jump);
  wp18 -> AddPath(wp1);
//  wp18 -> AddPath(wp1, Path_Backflip);
  wp20 -> AddPath(wp21);
  wp21 -> AddPath(wp22);
  wp21 -> AddPath(wp1);
  wp21 -> AddPath(wp20);
  wp22 -> AddPath(wp21);
  wp22 -> AddPath(wp23);
  wp23 -> AddPath(wp22);
  wp23 -> AddPath(wp14, Path_Backflip);

  if(!i) { var spec1 = wp20; var spec2 = wp2; }
  else
  {
    wp20 -> AddPath(spec1, Path_Jump, -1);
    spec1 -> AddPath(wp20, Path_Jump, 1);
    wp2 -> AddPath(spec2);
    spec2 -> AddPath(wp2);
  }
  }
}
