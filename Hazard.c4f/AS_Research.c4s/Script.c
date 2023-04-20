/*-- AS-Research --*/

#strict

static Door;
static Turret;
static Target;
static X;
static Y;

func Initialize() {
   SetSkyParallax(0,16,12);

	// Startpositionen
	X=CreateArray(2);
	Y=CreateArray(2);

	X[AS_GetAttackerTeam()]=40;
	Y[AS_GetAttackerTeam()]=655;
	X[AS_GetDefenderTeam()]=1240;
	Y[AS_GetDefenderTeam()]=530;

}


public func PlaceScenObjects() {

   if(!FindObject(WPCH) && !FindObject(IGIB))
   {
     PlaceSpawnpoint(GLWP,135,665,200);
     PlaceSpawnpoint(PIWP,160,525,200);
     PlaceSpawnpoint(ENWP,110,525,200);
     PlaceSpawnpoint(GLWP,1205,535,200);
     PlaceSpawnpoint(PIWP,1115,535,200);
     PlaceSpawnpoint(ENWP,1155,535,200);
     PlaceSpawnpoint(FTWP,1075,535,200);
   }

   if(!FindObject(NOAM) && !FindObject(IGIB))
   {
     PlaceSpawnpoint(GRAP,115,665,200);
     PlaceSpawnpoint(STAP,140,525,200);
     PlaceSpawnpoint(ENAP,90,525,200);
     PlaceSpawnpoint(GRAP,1185,535,200);
     PlaceSpawnpoint(STAP,1095,535,200);
     PlaceSpawnpoint(GSAP,1055,535,200);
     PlaceSpawnpoint(ENAP,1135,535,200);  
   }

   if(FindObject(WPCH))
   {
     AddWares(CreateObject(WPVM,1055,670,-1));
     AddWares(CreateObject(WPVM,1595,950,-1));
     AddWares(CreateObject(WPVM,240,1010,-1));
   }
   
   // Warps
   
   CreateObject(AWRP,1894,153,-1)->Set(1390,990);
   CreateObject(AWRP,1503,203,-1)->Set(1612,110);
   CreateObject(AWRP,1196,674,-1)->Set(860,290);
   CreateObject(AWRP,1226,953,-1)->Set(920,940);
   CreateObject(AWRP,2003,1145,-1)->Set(1333,930);

   // Serialize
   
	CreateObject(LADR,457,608,-1)->Set(12);
	CreateObject(SBBA,665,539,-1);
	CreateObject(STDR,816,541,-1);
	CreateObject(CRAT,725,600,-1);
	CreateObject(CRAT,294,660,-1);
	CreateObject(CRAT,328,660,-1);
	CreateObject(CRAT,364,660,-1);
	CreateObject(CRAT,949,262,-1);
	CreateObject(CRAT,956,290,-1);

	CreateObject(CRAT,1044,382,-1);
	CreateObject(CRAT,1044,410,-1);


	CreateObject(CRAT,1975,850,-1);
	CreateObject(CRAT,2004,850,-1);
	
	var door1 = CreateObject(STDR,407,661,-1);
	var door2 = CreateObject(STDR,237,671,-1);
	door1->Lock();
	door2->Lock();
	door1->SetMaxDamage(200);
	door2->SetMaxDamage(200);
	//CreateObject(CRAT,449,671,-1);
	//CreateObject(CRAT,482,670,-1);
	CreateObject(SBBA,651,671,-1);
	CreateObject(SBBA,663,660,-1);
	CreateObject(SBBA,678,671,-1);
	CreateObject(SBBA,828,669,-1);
	CreateObject(SBBA,896,671,-1);
	CreateObject(LBDR,2003,921,-1);

	var obj0 = CreateObject(LADR,1327,134,-1);
	obj0->Set(10);
	var obj1 = CreateObject(JMPD,899,133,-1);
	obj1->Set(80,60);
	CreateObject(ESGN,986,128,-1);
	CreateObject(FIEX,987,146,-1);
	var obj5 = CreateObject(LADR,1016,342,-1);
	obj5->Set(25);
	var obj6 = CreateObject(VENT,1097,146,-1);
	obj6->SetCon(50);
	CreateObject(CRAT,1178,150,-1);
	CreateObject(CRAT,1206,150,-1);
	CreateObject(CRAT,1270,150,-1);
	var obj7 = CreateObject(SBBA,1427,141,-1);
	obj7->SetDir(0);
	var obj8 = CreateObject(SBBA,1441,149,-1);
	obj8->SetDir(0);
	var obj9 = CreateObject(SBBA,1416,149,-1);
	obj9->SetDir(0);
	var obj10 = CreateObject(VENT,1492,146,-1);
	obj10->SetCon(50);
	var obj11 = CreateObject(LADR,1556,345,-1);
	obj11->Set(25);
	var obj12 = CreateObject(LADR,1243,253,-1);
	obj12->Set(10);
	var obj13 = CreateObject(FRAM,1219,183,-1);
	obj13->Set(1);
	var obj14 = CreateObject(FRAM,1274,186,-1);
	obj14->Set(5);
	var obj15 = CreateObject(FRAM,1342,184,-1);
	obj15->Set(2);
	var obj16 = CreateObject(LADR,1376,336,-1);
	obj16->Set(20);
	var obj17 = CreateObject(FRAM,1412,185,-1);
	obj17->Set(1);
	var obj18 = CreateObject(OPTB,1187,259,-1);
	obj18->TurnOff();
	CreateObject(ESGN,1157,237,-1);
	CreateObject(SCR3,1241,215,-1);
	CreateObject(MONI,1284,248,-1);
	CreateObject(MONI,1271,248,-1);
	var obj19 = CreateObject(GADG,1348,247,-1);
	obj19->Set(1);
	var obj20 = CreateObject(GADG,1334,245,-1);
	obj20->Set(2);
	var obj21 = CreateObject(GADG,1322,247,-1);
	obj21->Set(1);
	var obj22 = CreateObject(GADG,1428,246,-1);
	obj22->Set(3);
	CreateObject(MONI,1446,248,-1);
	CreateObject(CNS2,1403,260,-1);
	CreateObject(SCR3,1447,214,-1);
	CreateObject(GLST,1478,260,-1);
	CreateObject(RADR,771,321,-1);
	CreateObject(HSGN,1100,295,-1);
	CreateObject(FIEX,1158,256,-1);
	CreateObject(GSBO,1227,260,-1);
	CreateObject(GSBO,1217,260,-1);
	CreateObject(LTBL,1280,259,-1);
	CreateObject(LTBL,1335,259,-1);
	CreateObject(LTBL,1436,259,-1);
	CreateObject(GSBO,1498,260,-1);
	var obj23 = CreateObject(PSTR,1168,361,-1);
	obj23->Set(3,1);
	CreateObject(ESGN,1404,347,-1);
	CreateObject(CRAT,1490,342,-1);
	CreateObject(CRAT,1455,342,-1);
	var obj24 = CreateObject(LADR,777,510,-1);
	obj24->Set(18);
	var obj25 = CreateObject(PSTR,891,372,-1);
	obj25->Set(0,2);
	CreateObject(CRAT,920,370,-1);
	CreateObject(CRAT,962,370,-1);
	var obj26 = CreateObject(JMPD,1123,372,-1);
	obj26->Set(110,-10);
	CreateObject(GSTA,1224,369,-1);
	CreateObject(GSTA,1288,369,-1);
	CreateObject(GSBO,1338,370,-1);
	CreateObject(GSBO,1327,370,-1);
	CreateObject(CRAT,1440,370,-1);
	CreateObject(FIEX,1405,364,-1);
	CreateObject(CRAT,1499,370,-1);
	CreateObject(CRAT,1469,370,-1);
	CreateObject(LCKR,749,540,-1);
	CreateObject(LCKR,732,540,-1);
	var obj43 = CreateObject(LADR,796,638,-1);
	obj43->Set(6);
	CreateObject(CRAT,864,600,-1);
	CreateObject(ENGT,1051,623,-1);
	CreateObject(SPVM,1104,670,-1);
	CreateObject(SCR3,1592,903,-1);
	CreateObject(SCR3,1713,904,-1);
	CreateObject(CRAT,599,953,-1);
	var obj56 = CreateObject(LADR,846,1163,-1);
	obj56->Set(28);
	CreateObject(SPVM,1375,950,-1);
	CreateObject(LCKR,1400,950,-1);
	CreateObject(LCKR,1415,950,-1);
	CreateObject(PLT2,1464,951,-1);
	CreateObject(PLT2,1545,951,-1);
	var obj57 = CreateObject(LADR,1506,1055,-1);
	obj57->Set(15);
	CreateObject(LCKR,1629,950,-1);
	CreateObject(LCKR,1645,950,-1);
	CreateObject(PLNT,1697,938,-1);
	CreateObject(OTBL,1696,949,-1);
	CreateObject(CCP1,1739,950,-1);
	var obj58 = CreateObject(LADR,1815,1031,-1);
	obj58->Set(15);
	CreateObject(SPVM,95,1010,-1);
	CreateObject(PLNT,135,998,-1);
	CreateObject(ENGT,147,976,-1);
	CreateObject(LCKR,194,1010,-1);
	CreateObject(LCKR,178,1010,-1);
	CreateObject(CRAT,577,980,-1);
	CreateObject(CRAT,605,980,-1);
	CreateObject(CRAT,636,980,-1);
	var obj59 = CreateObject(PSTR,687,969,-1);
	obj59->Set(0,0);
	CreateObject(CLVM,735,980,-1);
	CreateObject(MVNT,1945,1024,-1);
	CreateObject(BOOM,2028,959,-1);
	CreateObject(TABL,135,1009,-1);
	var obj60 = CreateObject(LADR,585,1160,-1);
	obj60->Set(15);
	var obj61 = CreateObject(PSTR,1584,1024,-1);
	obj61->Set(3,0);
	CreateObject(CRAT,1660,1052,-1);
	CreateObject(GSTA,1769,1059,-1);
	CreateObject(SCR3,90,1062,-1);
	CreateObject(ESGN,140,1084,-1);
	CreateObject(FIEX,140,1104,-1);
	CreateObject(TABL,193,1099,-1);
	CreateObject(CHAR,173,1099,-1);
	CreateObject(SCR2,204,1088,-1);
	CreateObject(STDR,255,1101,-1);
	CreateObject(VENT,428,1116,-1);
	CreateObject(PLT2,616,1081,-1);
	CreateObject(CCP1,646,1080,-1);
	CreateObject(CCP1,681,1080,-1);
	CreateObject(CGLO,763,1080,-1);
	CreateObject(CLVM,1333,1080,-1);
	CreateObject(GLST,1570,1080,-1);
	var obj62 = CreateObject(RAI1,1839,1060,-1);
	obj62->SetRail([0, 1, 1, 1, 4],1);
	CreateObject(GSBO,1869,1060,-1);
	CreateObject(GSBO,1860,1060,-1);
	CreateObject(CRAT,741,1156,-1);
	CreateObject(PLNT,645,1167,-1);
	CreateObject(GTBL,659,1180,-1);
	CreateObject(PLNT,668,1167,-1);
	CreateObject(LCKR,780,1180,-1);
	CreateObject(LCKR,796,1180,-1);
	CreateObject(CRAT,740,1181,-1);
}

func PreResetAssault()
{
  X[AS_GetAttackerTeam()]=40;
	Y[AS_GetAttackerTeam()]=655;
	X[AS_GetDefenderTeam()]=1240;
	Y[AS_GetDefenderTeam()]=530;
}

func ResetAssault()
{
	// new objects
	PlaceScenObjects();

	// and initialize!
	CreateASStuff();
}

func CreateASStuff()
{
	Door=CreateArray(9);
	Turret=CreateArray(6);
	Target=CreateArray(5);

	// turret des Angreiferteams
	var turret=CreateObject(SEGU,170,590,-1);
	turret->Arm(RGSA);
	turret->TurnOn();
	turret->SetTeam(AS_GetAttackerTeam());

	//Target 1
	Target[0] = CreateObject(CCP2,905,585,-1);
	Target[0]->SetName("Computer 1");

	Door[0]=CreateObject(SEDR,1160,670,-1);

	Door[1]=CreateObject(SEDR,800,370,-1);

	Door[2]=CreateObject(SEDR,990,290,-1);
	
	Door[7] = CreateObject(SEDR,973,410,-1);
	
	Door[8] = CreateObject(SEDR,1007,410,0);

	Turret[0]=CreateObject(SEGU,600,515,-1);

	//Target 2
	Target[1] = CreateObject(CCP2,1230,65,-1);

	Door[3]=CreateObject(SEDR,1759,150,-1);
	Door[3]->Lock();

	Turret[1]=CreateObject(SEGU,1100,60,-1);

	Turret[2]=CreateObject(SEGU,1415,65,-1);

	//Target 3
	Target[2] = CreateObject(CCP2,2030,945,-1);
	
	Door[4]=CreateObject(SEDR,1300,950,-1);

	Turret[3]=CreateObject(SEGU,1850,980,-1);

	//Target 4
	Target[3] = CreateObject(CCP1,720,1065,-1);

	Door[5]=CreateObject(SEDR,270,1100,-1);

	Door[6]=CreateObject(SEDR,60,1010,-1);

	Turret[4]=CreateObject(SEGU,555,1035,-1);

	// Hauptziel - Target 5
	Target[4] = CreateObject(LBPC,95,1095,-1);
	Target[4]->SetName("$MainComp$");

	Turret[5]=CreateObject(SEGU,210,1060,-1);

	// Türen schließen
	for(var door in Door)
		door->Lock();

	var assault = FindObject(GAS_);
		
	// Ziele
	for(var i = 0; i < 4; i++)
	{
		Target[i]->SetName(Format("Computer %d", i+1));
		assault->AddTarget(Target[i], 0, 200);
	}
	assault->AddTarget(Target[4], 0, 500);

	for(var turret in Turret)
	{
		turret->Arm(MGSA);
		turret->TurnOn();
		turret->SetTeam(AS_GetDefenderTeam());
		assault->AddTarget(turret, 1,150);
	}
  
  // wealth reset
  for(var i = 0; i<GetPlayerCount(); ++i)
  {
    SetWealth(GetPlayerByIndex(i),90);
  }
}

public func ChooserFinished()
{
  PlaceScenObjects();
  
  Arena_ChooserFinished();
  
  // Bei Waffenwahl werden alle Nicht-Munispawns entfernt
  var wp = FindObject(WPCH);
  if(wp)
  {
    AddWares(wp);
  }
  //AS_Rdy=true;
  CreateASStuff();
}

public func AddWares(object wp)
{
  wp->RemoveWare();
  wp->AddWare(PIWP,-1);  // Pistole
  wp->AddWare(GLWP,-1);  // Granatenwerfer
  wp->AddWare(ENWP,-1);  // Energy rifle
  wp->AddWare(MIWP,-1);  // minigun
  wp->AddWare(FTWP,-1);  // fire gun
  wp->AddWare(LMIN,-1);  // Lasermine
  wp->AddWare(SMIN,-1);  // swarm mine
  wp->AddWare(MINE,-1);  // mine
  wp->AddWare(MEDI,-1);  // medipack
  wp->AddWare(NVDN,-1);  // novodekan
  wp->AddWare(HARM,-1);  // armor
  wp->AddWare(DRSU,-1);  // drone
  wp->AddWare(JTPK,-1);  // jetpack
  wp->AddWare(KLAS,-1);  // Laser
  wp->AddWare(KSLM,-1);  // slime
}

func OnTarget1Destruction()
{
	Door[0]->Open();
	Door[1]->Open();
	Door[2]->Open();
	Door[7]->Open();
	Door[8]->Open();
	if(Turret[0])
		Turret[0]->TurnOff();

	X[AS_GetAttackerTeam()]=1240;
	Y[AS_GetAttackerTeam()]=530;
	X[AS_GetDefenderTeam()]=1730;
	Y[AS_GetDefenderTeam()]=45;

	for(var obj in FindObjects(Find_ID(SPNP), Find_InRect(0,170, 520,500)))
		obj->RemoveObject();

	// Spawn 3
	if(!FindObject(IGIB))
	{
		if(!FindObject(WPCH))
		{
			PlaceSpawnpoint(LMIN,1710,55,200);
			PlaceSpawnpoint(GLWP,1695,55,200);
			PlaceSpawnpoint(ENWP,1645,55,200);
			PlaceSpawnpoint(FTWP,1605,55,200);
			PlaceSpawnpoint(KLAS,1555,55,200);
			PlaceSpawnpoint(MEDI,1300,75,1000);
		}
		
		if(!FindObject(NOAM))
		{
			PlaceSpawnpoint(GRAP,1675,55,200);
			PlaceSpawnpoint(ENAP,1625,55,200);
			PlaceSpawnpoint(GSAP,1585,55,200);
			PlaceSpawnpoint(STAP,1530,255,500);
			PlaceSpawnpoint(GRAP,1510,255,500);
		}
	}
}

func OnTarget2Destruction()
{
	Door[3]->Open();
	if(Turret[1])
		Turret[1]->~TurnOff();
	if(Turret[2])
		Turret[2]->~TurnOff();

	X[AS_GetAttackerTeam()]=1730;
	Y[AS_GetAttackerTeam()]=45;
	X[AS_GetDefenderTeam()]=1800;
	Y[AS_GetDefenderTeam()]=835;

	for(var obj in FindObjects(Find_ID(SPNP), Find_InRect(1050,500, 200,50)))
		obj->RemoveObject();
	
	// Alle Minenspawns des vorherigen Spawnpoints löschen
	for(var obj in FindObjects(Find_ID(SPNP), Find_InRect(1550,0, 200,100))) {
		if(obj->Contents(0))
			if(obj->Contents(0)->~IsMine())
				obj->RemoveObject();
	}
	
	// Spawn 4
	if(!FindObject(IGIB))
	{
		if(!FindObject(WPCH))
		{
			PlaceSpawnpoint(PGWP,1770,795,200);
			PlaceSpawnpoint(KLAS,1730,795,400);
			PlaceSpawnpoint(GLWP,1765,845,200);
			PlaceSpawnpoint(ENWP,1725,845,200);
			PlaceSpawnpoint(MEDI,1850,915,1000);
		}
		if(!FindObject(NOAM))
		{
			PlaceSpawnpoint(GRAP,1745,845,200);
			PlaceSpawnpoint(STAP,1430,945,500);
			PlaceSpawnpoint(GRAP,1410,945,500);
			PlaceSpawnpoint(ENAP,1705,845,200);
			PlaceSpawnpoint(STAP,1750,795,200);
		}
	}
}

func OnTarget3Destruction()
{
	Door[4]->Open();
	if(Turret[3])
		Turret[3]->~TurnOff();

	X[AS_GetAttackerTeam()]=1800;
	Y[AS_GetAttackerTeam()]=835;
	X[AS_GetDefenderTeam()]=365;
	Y[AS_GetDefenderTeam()]=885;

	for(var obj in FindObjects(Find_ID(SPNP), Find_InRect(1550,0, 200,100)))
		obj->RemoveObject();
	
	// Spawn 5
	if(!FindObject(IGIB))
	{
		if(!FindObject(WPCH))
		{
			PlaceSpawnpoint(HARM,200,845,400);
			PlaceSpawnpoint(PGWP,370,895,200);
			PlaceSpawnpoint(GLWP,300,895,200);
			PlaceSpawnpoint(ENWP,240,895,200);
			PlaceSpawnpoint(SMIN,400,895,500);
			PlaceSpawnpoint(MEDI,710,595,1000);
			PlaceSpawnpoint(MEDI,240,1095,500);
			PlaceSpawnpoint(MINE,795,1070,500);
			PlaceSpawnpoint(MIWP,320,845,1000);
			PlaceSpawnpoint(KLAS,220,845,400);
		}
		if(!FindObject(NOAM))
		{
			PlaceSpawnpoint(STAP,300,845,1000);
			PlaceSpawnpoint(STAP,350,895,200);
			PlaceSpawnpoint(GRAP,280,895,200);
			PlaceSpawnpoint(ENAP,220,895,200);
			PlaceSpawnpoint(STAP,730,595,500);
			PlaceSpawnpoint(GRAP,750,595,500);
			PlaceSpawnpoint(STAP,180,1005,500);
			PlaceSpawnpoint(GRAP,200,1005,500);
		}
	}
}

func OnTarget4Destruction()
{
	Door[5]->Open();
	Door[6]->Open();

	X[AS_GetAttackerTeam()]=1800;
	Y[AS_GetAttackerTeam()]=835;
	X[AS_GetDefenderTeam()]=365;
	Y[AS_GetDefenderTeam()]=885;
}

public func RelaunchPosition(int iTeam)
{
  return [X[iTeam],Y[iTeam]];
}

// Man beachte die simple Genialität meiner Lösung.
// Und ich hasse dich jetzt, du hast mir meine Zeit gestohlen.
func ResetContents(obj)
{

	//obj->~StopAiming();
	var con, cnt;
	while(con = Contents(cnt++,obj))
	{
		RemoveObject(con);
		ClearScheduleCall(obj,"Contents(0)->~Selection(this());");
	}
}

protected func InitializePlayer(int iPlr, int iX, int iY, object pBase, int iTeam)
{
  for(var i=0, pCrew ; pCrew = GetCrew(iPlr, i) ; i++)
    RelaunchPlayer(iPlr, pCrew, -1, iTeam);

  DoScoreboardShow(1,iPlr);
}

public func RelaunchPlayer(int iPlr, object pCrew, int iKiller, int iTeam)
{
  Arena_RelaunchPlayer(iPlr, pCrew, iKiller, iTeam);
}

