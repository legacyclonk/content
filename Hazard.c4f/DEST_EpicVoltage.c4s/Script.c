/*-- "Mal endlich 'ne gescheite Map mit Fahrzeugen!" --*/

#strict

static Flags;

protected func Initialize()
{
  //Gegenstände
  SetMap();
  PlaceWeapons();
	
	SetSkyParallax(0, 20, 12); 
  Flags = CreateArray(3); // Index 0 wird nicht benutzt
}

private func xpos(x,i)
{
	if(!i) var dir = 1;
	else dir = -1;
	return 3040 * i + dir * x;
}

protected func SetMap(i) {

	CreateObject(PSGN,580,510)->SetGraphics("HV");
	CreateObject(PSGN,2455,510)->SetGraphics("XPower");

	CreateObject(HA4K,1520,441,-1);
	CreateObject(HA4K,1520,601,-1);
	CreateObject(LADR,1521,697,-1)->Set(33);
	
	CreateObject(FLAK,1522,390,-1)->SetPhase(18);
	
	CreateObject(RAI1,354,240,-1)->SetRail([0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 4],1);
	CreateObject(RAI1,1002,430,-1)->SetRail([0, 1, 1, 1, 1, 1, 4],1);
	CreateObject(RAI1,1420,440,-1)->SetRail([0, 1, 1, 1, 4],1);
	CreateObject(RAI1,2521,240,-1)->SetRail([0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 4],1);
	CreateObject(RAI1,1942,430,-1)->SetRail([0, 1, 1, 1, 1, 1, 4],1);
	CreateObject(RAI1,1560,440,-1)->SetRail([0, 1, 1, 1, 4],1);
	
	for(var i=0; i<2; ++i)
	{
		//Rampen, fein säuberlich von links nach rechts.
		DrawMaterialQuad("Wall-Concrete1",xpos(340,i),522,xpos(370,i),510,xpos(370,i),522,xpos(355,i),522,true);
		DrawMaterialQuad("Wall-Concrete1",xpos(481,i),522,xpos(451,i),510,xpos(451,i),522,xpos(456,i),522,true);
		

		//Warps
		CreateObject(AWRP,xpos(580,i),615,-1)->Set(xpos(605,i),410);

		// Generatoren
		CreateObject(GNR2,xpos(420,i),700,-1)->TurnOn();
		
		//Basis, Türen und Lifts
		var a;
		var b;
		CreateObject(CRAT,xpos(560,i),430,-1);
		CreateObject(SLDR,xpos(270,i),490,-1);
		CreateObject(SLDR,xpos(275,i),410,-1);
		CreateObject(SLDR,xpos(1485,i),440,-1);
		CreateObject(SLDR,xpos(275,i),240,-1);
		CreateObject(SLDR,xpos(345,i),240,-1)->SetSwitchLock(i, true);
		CreateObject(SLDR,xpos(275,i),700,-1);
		
		CreateObject(RADR,xpos(240,i),150,-1);
		CreateObject(FENC,xpos(240,i),150,-1);
		
		CreateObject(HA4K,xpos(860,i),561,-1);
		CreateObject(HA4K,xpos(1040,i),431,-1);
		CreateObject(HA4K,xpos(470,i),411,-1);
		
		CreateObject(LADR,xpos(476,i),476,-1)->Set(8);
		CreateObject(LADR,xpos(566,i),670,-1)->Set(6);
		CreateObject(LADR,xpos(856,i),620,-1)->Set(7);
		
		CreateObject(SLDR,xpos(285,i),340,-1)->SetSwitchLock(i, true);
		CreateObject(HNG2,xpos(630,i),520,-1)->SetSwitchLock(i, true);
		
		CreateObject(LFTP,xpos(235,i),533,-1)->SetLimits(265,780);
		
		//Deko außerhalb der Basis
		CreateObject(STDR,xpos(1503,i),520,-1);
		CreateObject(LADR,xpos(1046,i),501,-1)->Set(10);
		CreateObject(FENC,xpos(1490,i),771,-1);
		CreateObject(WNDW,xpos(645,i),440)->SetDir(!i);
		CreateObject(WNDW,xpos(1398,i),531,-1)->SetDir(i);
		
		//Alarmlichter
		CreateObject(ALGH,xpos(420,i),290,-1)->SetR(180);
		CreateObject(ALGH,xpos(314,i),460,-1)->SetR(180);
		CreateObject(ALGH,xpos(542,i),460,-1)->SetR(180);
		CreateObject(ALGH,xpos(340,i),580,-1)->SetR(180);
		CreateObject(ALGH,xpos(480,i),580,-1)->SetR(180);

		/*--Selbstschussanlagen werden jetzt in ChooserFinished gebaut--*/

		//Waffen
		CreateObject(PRCA,xpos(605,i),340,-1)->SetPhase(26-i*13);
		
		// bäume
		CreateObject(TRE4,xpos(1136,i),415,-1);
		CreateObject(TRE4,xpos(901,i),563,-1);
		CreateObject(TRE4,xpos(1140,i),575,-1);
		CreateObject(TRE4,xpos(1172,i),583,-1);
		CreateObject(TRE4,xpos(1293,i),582,-1);
		CreateObject(TRE4,xpos(1314,i),592,-1);
		CreateObject(TRE4,xpos(1399,i),602,-1);
		CreateObject(TRE4,xpos(1356,i),605,-1);
	}
}

public func SetWares(object wp1)
{
	wp1->RemoveWare();
  wp1->AddWare(PIWP,-1);
  wp1->AddWare(PGWP,-1);
  wp1->AddWare(FTWP,-1);
  wp1->AddWare(GLWP,-1);
  wp1->AddWare(KRFL,-1);
  wp1->AddWare(KSLM,-1);
  wp1->AddWare(HARM,-1);
  wp1->AddWare(MINE,-1);
  wp1->AddWare(MEZL,-1);
  wp1->AddWare(ENWP,-1);
  wp1->AddWare(MIWP,-1);
  wp1->AddWare(BZWP,-1);
  wp1->AddWare(KRFL,-1);
  wp1->AddWare(AIRS,-1);
  wp1->AddWare(MEDI,-1);
  wp1->AddWare(HSHD,-1);
  wp1->AddWare(JTPK,-1);
  wp1->AddWare(DRSU,-1);
  wp1->AddWare(KLAS,-1);
}

/* Regelwähler */

public func ChooserFinished()
{
	Arena_ChooserFinished();

	CreateObject(CCP1,1465,520,-1)->Set(5);
	
	var wp = FindObject(WPCH);
	if(wp)
	{
		SetWares(wp);
		
		//Waffenautomaten + Füllung
		var wp3 = CreateObject(WPVM,1575,520,-1);

		wp3->AddWare(FTWP,-1);
		wp3->AddWare(KSLM,-1);
		wp3->AddWare(MINE,-1);
		wp3->AddWare(MEZL,-1);
		wp3->AddWare(ENWP,-1);
		wp3->AddWare(AIRS,-1);
		wp3->AddWare(NVDN,-1);
		wp3->AddWare(HSHD,-1);
		wp3->AddWare(MEDI,-1);
	}
	
	if(FindObject(GCTF))
	{
		for(var i=0; i<GetPlayerCount(); ++i)
		{
			var iTeam = GetPlayerTeam(i);
			// Flagge setzen (wenn nicht schon erledigt
			if(!Flags[iTeam])
			{
				Flags[iTeam] = true;
				InitTeamFlag(iTeam);
			}
		}
	}
	else
	{
		ScriptGo(1);
	}
	
	for(var i=0; i<2; ++i)
	{
		CreateObject(SWTH,xpos(1460,i),580,-1);
		
		if(wp)
		{
			//Steuerelemente
			CreateObject(CCP1,xpos(315,i),520,-1)->Set(2+i*2);
			CreateObject(CCP1,xpos(305,i),240,-1)->Set(1+i*2);
			
			CreateObject(TANK,xpos(410,i),510,-1);
			CreateObject(SHTL,xpos(463,i),245,-1);
			
			var wp1 = CreateObject(WPVM,xpos(405,i),410,-1);
			SetWares(wp1);
		}
		else
		{
			PlaceVehicleSpawnpoint(TANK,xpos(410,i),500);
			PlaceVehicleSpawnpoint(SHTL,xpos(463,i),235);
			
			var gas2 = FindObject(GAS2);
			if(gas2)
			{
				gas2->AddTarget(FindObject(GNR2,xpos(400,i),650), false, 1000, false, i+1);
				gas2->AddTarget(FindObject(HNG2,xpos(630,i),500), true, 500, false, i+1);
			}
			
			// sentry guns if not ctf...
			if(!FindObject(GCTF))
			{
				var turret1 = CreateObject(SEGU,xpos(237,i),188,-1);
				turret1->TurnOn();
				turret1->Arm(RGSA);
				turret1->SetTeam(i+1);
				if(gas2) gas2->AddTarget(turret1, true, 350, false, i+1);

				var turret2 = CreateObject(SEGU,xpos(408,i),462,-1);
				turret2->TurnOn();
				turret2->Arm(RGSA);
				turret2->SetTeam(i+1);
				if(gas2) gas2->AddTarget(turret2, true, 350, false, i+1);
			}
		}
	}
}


/* Relaunch */

protected func InitializePlayer(int iPlr, int iX, int iY, object pBase, int iTeam)
{
	if(!FindObject(CHOS))
	{
		if(FindObject(GCTF))
		{
			// Flagge setzen (wenn nicht schon erledigt
			if(!Flags[iTeam])
			{
				Flags[iTeam] = true;
				InitTeamFlag(iTeam);
			}
		}
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
	if(iTeam == 1) return [RandomX(310,450), 400];
	if(iTeam == 2) return [RandomX(2590,2730), 400];
}

/*--Flaggenkrams--*/
public func FlagReturned(int Team, object pClonk)
{
	Alarm(false, Team);
}

public func FlagCaptured(int Team, int EnemyTeam, object pClonk)
{
	Alarm(true, Team);
}

protected func InitTeamFlag(int iTeam)
{
  var x,y,color;
  if(iTeam == 1)
  {
    x = 410;
    y = 700;
    color = RGB(255,255,0);
  }
  if(iTeam == 2)
  {
    x = 2630;
    y = 700;
    color = RGB(0,255,255);
  }
  CreateFlag(iTeam, x, y, GetTeamColor(iTeam));
}

public func Alarm(bool alarm, int Team)
{
	var x, width = LandscapeWidth()/2;
	if(Team == 1) x = 0;
	if(Team == 2) x = LandscapeWidth()/2;

	var alarmobj;
	while(alarmobj = FindObject(ALGH,x,0,width,LandscapeHeight(),0,0,0,0,alarmobj))
	{
		if(alarm) alarmobj -> TurnOn();
		else      alarmobj -> TurnOff();
	}
}

/* Muni und Bonus */

public func PlaceWeapons()
{
	PlaceSpawnpoint(STAP,1452,429);
	PlaceSpawnpoint(GRAP,1590,429);
	PlaceBonusSpawnpoint([MNYB],1520,699,1500);
	PlaceSpawnpoint(MINE,1520,429);
	PlaceSpawnpoint(GSAP,1570,589);
	PlaceSpawnpoint(GGWP,1505,760);
	PlaceSpawnpoint(ENAP,1535,760);
	PlaceSpawnpoint(JTPK,1590,590);
	PlaceSpawnpoint(HSHD,1470,510);
	PlaceSpawnpoint(KLAS,1335,740);
	PlaceSpawnpoint(SMIN,1705,740);
	
	for(var i=0; i<2; ++i)
	{
		PlaceSpawnpoint(MEZL,xpos(305,i),140);
		PlaceSpawnpoint(MIAP,xpos(370,i),229);
		PlaceSpawnpoint(GLWP,xpos(295,i),399);
		PlaceSpawnpoint(GRAP,xpos(315,i),399);
		PlaceSpawnpoint(MIAP,xpos(335,i),399);
		PlaceSpawnpoint(ENAP,xpos(355,i),399);
		PlaceSpawnpoint(ENWP,xpos(445,i),399);
		PlaceSpawnpoint(GRAP,xpos(335,i),509);
		PlaceSpawnpoint(MIWP,xpos(580,i),690);
		PlaceSpawnpoint(STAP,xpos(550,i),690);
		PlaceSpawnpoint(MEDI,xpos(330,i),330);
		PlaceSpawnpoint(AIRS,xpos(530,i),220);
		PlaceSpawnpoint(HARM,xpos(600,i),420);
		PlaceSpawnpoint(PGWP,xpos(565,i),510);
		PlaceSpawnpoint(STAP,xpos(595,i),510);
		PlaceSpawnpoint(LMIN,xpos(290,i),590);
		PlaceSpawnpoint(KRFL,xpos(470,i),330);
		PlaceSpawnpoint(KSLM,xpos(510,i),420);
		PlaceSpawnpoint(MIAP,xpos(1060,i),419);
		PlaceSpawnpoint(BZWP,xpos(1080,i),420);
		PlaceSpawnpoint(DRSU,xpos(1070,i),510);
		PlaceSpawnpoint(FTWP,xpos(1220,i),550);
		PlaceSpawnpoint(GSAP,xpos(1250,i),550);

	}
}

protected func Script10() {
	
	var alarm1 = false;
	var alarm2 = false;
  
	for(var obj in FindObjects(Find_InRect(0,0,600,LandscapeHeight()),Find_OCF(OCF_CrewMember)))
	{
		if(GetPlayerTeam(GetOwner(obj)) != 1) alarm1 = true;
	}
	for(var obj in FindObjects(Find_InRect(2440,0,600,LandscapeHeight()),Find_OCF(OCF_CrewMember)))
	{
		if(GetPlayerTeam(GetOwner(obj)) != 2) alarm2 = true;
	}
	
	Alarm(alarm1,1);
	Alarm(alarm2,2);
  goto(0);
}