/*-- Space --*/

#strict

static Flags;

protected func Initialize()
{
  // Hier kommt die Sonne!
  var s2 = CreateObject(SUN_, LandscapeWidth()/3, LandscapeHeight()/2, -1);
  var s1 = CreateObject(SUN_, LandscapeWidth()/3, LandscapeHeight()/2, -1);

  s1->SetClrModulation(RGBa(255,100,0,0));
  s2->SetObjectBlitMode(1);
  s2->SetClrModulation(RGBa(255,255,0,0));
  s2->SetRotationSpeed(12);
  s2->SetGrowPos(13000);
  
  SetSkyParallax(30,30);
  
  // Deko
  CreateObject(SCR3, 1020, 555, -1)->SetClrModulation(RGB(255,255,0));
  CreateObject(SCR3, 480, 555, -1)->SetClrModulation(RGB(128,0,128));
//  CreateObject(SUN_, 750, 575, -1);
  // Jumppads
  CreateObject(JMPD, 67, 730, -1)->Set(67, 5);
  CreateObject(JMPD, 1433, 730, -1)->Set(67, -5);
  CreateObject(JMPD, 250, 370, -1)->Set(58, 25);
  CreateObject(JMPD, 1250, 370, -1)->Set(58, -25);
  CreateObject(JMPD, 460, 180, -1)->Set(65, -45);
  CreateObject(JMPD, 1040, 180, -1)->Set(65, 45);
  CreateObject(JMPD, 510, 390, -1)->Set(58, 25, 90);
  CreateObject(JMPD, 990, 390, -1)->Set(58, -25, -90);
  // Struktur
  CreateObject(LADR, 190, 500, -1)->Set(11);
  CreateObject(LADR, 1310, 500, -1)->Set(11);
  // Waffen & Ausrüstung
  PlaceSpawnpoint(PGWP, 100, 350);
  PlaceSpawnpoint(STAP, 110, 350);
  PlaceSpawnpoint(PGWP, 1400, 350);
  PlaceSpawnpoint(STAP, 1390, 350);
  PlaceSpawnpoint(KLAS, 150, 500);
  PlaceSpawnpoint(KLAS, 1350, 500);
  PlaceSpawnpoint(ENAP, 160, 500);
  PlaceSpawnpoint(ENAP, 1340, 500);
  PlaceSpawnpoint(STAP, 1390, 350);
  PlaceSpawnpoint(JTPK, 750, 350);
  PlaceSpawnpoint(GSAP, 735, 350);
  PlaceSpawnpoint(GSAP, 765, 350);
  PlaceSpawnpoint(MIAP, 670, 90);
  PlaceSpawnpoint(MIAP, 830, 90);
  PlaceSpawnpoint(BZWP, 750, 20);
  PlaceSpawnpoint(GLWP, 90, 110);
  PlaceSpawnpoint(GLWP, 1410, 110);
  PlaceSpawnpoint(GRAP, 110, 110);
  PlaceSpawnpoint(GRAP, 1390, 110);
  PlaceSpawnpoint(GRAP, 660, 270);
  PlaceSpawnpoint(GRAP, 840, 430);
  PlaceSpawnpoint(STAP, 660, 430);
  PlaceSpawnpoint(STAP, 840, 270);
  PlaceSpawnpoint(KRFL, 480, 550);
  PlaceSpawnpoint(KRFL, 1020, 550);
  PlaceSpawnpoint(GGWP, 460, 710);
  PlaceSpawnpoint(GGWP, 1040, 710);
  PlaceSpawnpoint(ENAP, 620, 710);
  PlaceSpawnpoint(ENAP, 880, 710);
  PlaceSpawnpoint(HARM, 90, 720);
  PlaceSpawnpoint(HARM, 1410, 720);
  PlaceSpawnpoint(ENWP, 460, 210);
  PlaceSpawnpoint(ENWP, 1040, 210);
  PlaceSpawnpoint(ENAP, 500, 210);
  PlaceSpawnpoint(ENAP, 1000, 210);
  PlaceSpawnpoint(MINE, 750, 530);
  // Flaggen
  Flags = CreateArray();
  
  // Alarm-lights
  
  CreateObject(ALGH,1022,199,-1)->SetR(180);
  CreateObject(ALGH,1022,538,-1)->SetR(180);
  CreateObject(ALGH,1416,378,-1)->SetR(180);
  
  CreateObject(ALGH,478,199,-1)->SetR(180);
  CreateObject(ALGH,478,538,-1)->SetR(180);
  CreateObject(ALGH,84,378,-1)->SetR(180);
  
  
  // Lights
  CreateObject(BLGH,1385,628,-1);
  CreateObject(BLGH,1020,374,-1);
  CreateObject(CLGH,1162,641,-1);
  CreateObject(CLGH,1262,434,-1);
  CreateObject(FLGH,1351,508,-1)->SetRotation(30);
  CreateObject(FLGH,995,518,-1)->SetRotation(-50);
  CreateObject(FLGH,991,241,-1)->SetRotation(50,180);
  
  CreateObject(BLGH,115,628,-1);
  CreateObject(BLGH,480,374,-1);
  CreateObject(CLGH,338,641,-1);
  CreateObject(CLGH,238,434,-1);
  CreateObject(FLGH,149,508,-1)->SetRotation(-30);
  CreateObject(FLGH,505,518,-1)->SetRotation(50);
  CreateObject(FLGH,509,241,-1)->SetRotation(-50,180);
  
  // Vents
  CreateObject(VENT,120,700,-1)->DoCon(-70);
  CreateObject(VENT,1380,700,-1)->DoCon(-70);

  // Waypoints
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
    wp->AddWare(ENWP);  // Energiegewehr
    wp->AddWare(GLWP);  // Granatenwerfer
    wp->AddWare(GGWP);  // Partikelkanone
    wp->AddWare(KLAS);  // Laser
    wp->AddWare(FLSH);  // Taschenlampe
    wp->AddWare(JTPK);  // Jetpack
    wp->AddWare(HSHD);  // Schild
    wp->AddWare(DRSU);  // Drone
    wp->AddWare(LMIN);  // Lasermine
    wp->AddWare(SMIN);  // Schwarmmine
    //wp->SortWare();
  }
}

/* Flaggen */

protected func InitTeamFlag(int iTeam)
{
  var x,y,color;
  if(iTeam == 1)
  {
    x = 90;
    y = 420;
    color = RGB(128,0,128);
  }
  if(iTeam == 2)
  {
    x = 1410;
    y = 420;
    color = RGB(255,255,0);
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
  if(iTeam == 1) return [80,350];
  if(iTeam == 2) return [1420,350];
}

public func FlagReturned(team, clonk)
{
  Alarm(false, team);
}

public func FlagCaptured(team, enemyteam, clonk)
{
  Alarm(true, team);
}

public func Alarm(bool alarm, int team)
{
  var x, width = LandscapeWidth()/2;
  if(team == 1) x = 0;
  if(team == 2) x = LandscapeWidth()/2;
  
  var alarmobj;
  while(alarmobj = FindObject(ALGH,x,0,width,LandscapeHeight(),0,0,0,0,alarmobj))
  {
    if(alarm) alarmobj -> TurnOn();
    else      alarmobj -> TurnOff();
  }
}

/* KI */

protected func CreateWaypoints()
{
  for(var i=0 ; i < 2 ; i++)
  {
    if(!i) var dir = 1;
    else dir = -1;

    var wp1 = CreateWP(1500*i+dir*90, 350);
    var wp2 = CreateWP(1500*i+dir*200, 410);
    var wp3 = CreateWP(1500*i+dir*30, 325);
    var wp4 = CreateWP(1500*i+dir*240, 360);
    var wp5 = CreateWP(1500*i+dir*90, 410);
    var wp6 = CreateWP(1500*i+dir*170, 500);
    var wp7 = CreateWP(1500*i+dir*260, 500);
    var wp8 = CreateWP(1500*i+dir*340, 610);
    var wp9 = CreateWP(1500*i+dir*280, 670);
    var wp10= CreateWP(1500*i+dir*130, 720);
    var wp11= CreateWP(1500*i+dir*70, 720);
    var wp12= CreateWP(1500*i+dir*415, 550);
    var wp13= CreateWP(1500*i+dir*315, 270);
    var wp14= CreateWP(1500*i+dir*405, 210);
    var wp15= CreateWP(1500*i+dir*455, 170);
    var wp16= CreateWP(1500*i+dir*145, 110);
    var wp17= CreateWP(1500*i+dir*20, 110);
    var wp18= CreateWP(1500*i+dir*560, 210);
    var wp19= CreateWP(1500*i+dir*670, 95);
    var wp20= CreateWP(1500*i+dir*680, 270);
    var wp21= CreateWP(1500*i+dir*635, 430);
    var wp22= CreateWP(1500*i+dir*670, 430);
    var wp23= CreateWP(1500*i+dir*515, 510);
    var wp24= CreateWP(1500*i+dir*535, 555);
    var wp25= CreateWP(1500*i+dir*460, 720);
    var wp26= CreateWP(1500*i+dir*640, 575);
    var wp27= CreateWP(1500*i+dir*620, 720);

    if(!i)
    {
      var wp28= CreateWP(750, 355);
      var wp29= CreateWP(750, 655);
      var spec1 = wp18;
      var spec2 = wp19;
      var spec3 = wp20;
      var spec4 = wp26;
      var jtpk1 = Jetpack_Right;
      var jtpk2 = Jetpack_UpLeft;
    }
    else
    {
      var jtpk1 = Jetpack_Left;
      var jtpk2 = Jetpack_UpRight;
    }

    wp1 -> AddPath(wp2);
    wp1 -> AddPath(wp3);
    wp1 -> AddPath(wp4, Path_Jump, 1*dir);
    wp2 -> AddPath(wp1, Path_Jump,-1*dir);
    wp2 -> AddPath(wp4, Path_Jump, 1*dir);
    wp2 -> AddPath(wp5);
    wp2 -> AddPath(wp6, Path_MoveTo);
    wp2 -> AddPath(wp12);
    wp3 -> AddPath(wp1);
    wp3 -> AddPath(wp5);
    wp4 -> AddPath(wp14);
    wp5 -> AddPath(wp2);
    wp5 -> AddPath(wp3);
    wp6 -> AddPath(wp2, Path_Jump);
    wp6 -> AddPath(wp7);
    wp6 -> AddPath(wp10);
    wp7 -> AddPath(wp6);
    wp7 -> AddPath(wp8);
    wp7 -> AddPath(wp12, Path_Backflip);
    wp8 -> AddPath(wp7, Path_Jump,-1*dir);
    wp8 -> AddPath(wp12, Path_Jump, 1*dir);
    wp8 -> AddPath(wp25);
    wp9 -> AddPath(wp8, Path_Jump, 1*dir);
    wp9 -> AddPath(wp10, Path_Jump,-1*dir);
    wp9 -> AddPath(wp25);
    wp10-> AddPath(wp9, Path_Jump, 1*dir);
    wp10-> AddPath(wp11);
    wp11-> AddPath(wp6);
    wp12-> AddPath(wp2, Path_Jump,-1*dir, 45, jtpk2); // Jetpack!
    wp12-> AddPath(wp8);
    wp12-> AddPath(wp24);
    wp13-> AddPath(wp2);
    wp13-> AddPath(wp14, Path_Jump, 1*dir);
    wp14-> AddPath(wp3, Path_Backflip,-1*dir);
    wp14-> AddPath(wp13);
    wp14-> AddPath(wp15, Path_Jump, 1*dir);
    wp14-> AddPath(wp18);
    wp15-> AddPath(wp16);
    wp16-> AddPath(wp8);
    wp16-> AddPath(wp14, 0, 1*dir, 50, jtpk1); // Jetpack
    wp16-> AddPath(wp17);
    wp17-> AddPath(wp16);
    wp18-> AddPath(wp14);
    wp18-> AddPath(wp19, Path_Jump, 1*dir);
    wp18-> AddPath(wp20);
    //wp18
    wp19-> AddPath(wp18);
    //wp19-> AddPath(wp, Path_Jump, 1*dir);
    wp20-> AddPath(wp18, Path_Jump,-1*dir);
    wp20-> AddPath(wp28);
    //wp20
    wp21-> AddPath(wp20, Path_Jump,-1*dir);
    wp21-> AddPath(wp22);
    wp21-> AddPath(wp24);
    wp22-> AddPath(wp21);
    wp22-> AddPath(wp28, Path_Jump, 1*dir);
    wp23-> AddPath(wp21, Path_Jump, 1*dir);
    wp23-> AddPath(wp26);
    wp24-> AddPath(wp12);
    wp24-> AddPath(wp23);
    wp24-> AddPath(wp26);
    wp25-> AddPath(wp8, Path_Jump,-1*dir);
    wp25-> AddPath(wp9, Path_Jump,-1*dir);
    wp25-> AddPath(wp29, Path_Jump, 1*dir, 20, jtpk1); // Jetpack
    wp25-> AddPath(wp27, Path_Jump, 1*dir);
    wp26-> AddPath(wp23, Path_Jump,-1*dir);
    wp26-> AddPath(wp24);
    wp26-> AddPath(wp29);
    wp27-> AddPath(wp25, Path_Jump,-1*dir);
    wp27-> AddPath(wp29, Path_Jump, 1*dir);
    wp28-> AddPath(wp20, Path_Jump, 1*dir);
    wp28-> AddPath(wp22);
    wp29-> AddPath(wp25, Path_Backflip,-1*dir);
    wp29-> AddPath(wp26, Path_Jump,-1*dir);
    wp29-> AddPath(wp27);
    if(i)
    {
      spec1 -> AddPath(wp20, Path_Backflip, 1);
      spec2 -> AddPath(wp19, Path_Jump, 1);
      spec3 -> AddPath(wp18, Path_Jump, 1, 20, Jetpack_Right); // Jetpack
      spec3 -> AddPath(wp20, Path_Jump, 1);
      spec4 -> AddPath(wp26, Path_Backflip, 1);
      wp18 -> AddPath(spec3, Path_Backflip,-1);
      wp19 -> AddPath(spec2, Path_Jump,-1);
      wp20 -> AddPath(spec1, Path_Jump,-1, 20, Jetpack_Left); // Jetpack
      wp20 -> AddPath(spec3, Path_Jump,-1);
      wp26 -> AddPath(spec4, Path_Backflip,-1);
    }
  }
}
