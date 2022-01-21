/*-- Tiefsee --*/

#strict

static Flags;

func Initialize()
{
  Flags = CreateArray(5); // Index 0 wird nicht benutzt

  // Auffüller
  CreateObject(_REF, 1110, 1, -1);
  CreateObject(_REF, 1445, 65, -1);
  CreateObject(_REF, 1565, 135, -1);
  CreateObject(_REF, 1390, 12, -1);
  CreateObject(_REF, 590, 581, -1);
  // Deko
  CreateObject (SHEL, 378, 700, -1);
  CreateObject (SHEL, 1850, 521, -1);
  CreateObject (SHEL, 1770, 430, -1);
  CreateObject (IDOL, 671, 474, -1)->SetR(-45);
  CreateObject (SUBB, 439, 1171, -1)->SetR(45);
  CreateObject (PLNT, 135, 309, -1);
  CreateObject (CCP2, 436, 922, -1);
  CreateObject (CCP1, 401, 920, -1);
  CreateObject (LTBL, 246, 489, -1);
  CreateObject (BED2, 97, 179, -1)->SetCon(50);
  CreateObject (BED2, 56, 179, -1);
  CreateObject (BED2, 37, 99, -1);
  CreateObject (CON1, 29, 490, -1);
  CreateObject (CGLO, 2223, 479, -1);
  CreateObject (OTBL, 515, 179, -1);
  CreateObject (OTBL, 1874, 929, -1);
  CreateObject (TELE, 2338, 701, -1)->Deactivate();
  var pCrane = CreateObject (CRN1, 1850, 599, -1);
  pCrane->Set(15);
  CreateObject (CONS, 1957, 642, -1)->Set(pCrane);
  CreateObject (GADG, 2151, 1237, -1);
  CreateObject (MONI, 2132, 1225, -1);
  CreateObject (MPPJ, 30, 970, -1);
  CreateObject (OCHR, 510, 179, -1);
  CreateObject (OCHR, 2142, 1239, -1);
  CreateObject (PLT2, 74, 181, -1);
  CreateObject (PLT2, 2187, 1241, -1);
  CreateObject (GTBL, 82, 310, -1);
  CreateObject (GTBL, 2140, 1240, -1);
  CreateObject (GSBO, 198, 490, -1);
  CreateObject (GSBO, 190, 490, -1);
  CreateObject (GSBO, 2333, 850, -1);
  CreateObject (GSBO, 2327, 850, -1);
  CreateObject (GSTA, 2300, 849, -1);
  CreateObject (FLGH, 64, 841, -1)->SetRotation(20, 135);
  CreateObject (FLGH, 273, 991, -1)->SetRotation(0, 180);
  CreateObject (FLGH, 231, 180, -1)->SetRotation(48);
  CreateObject (FLGH, 2394, 692, -1)->SetRotation(-80);
  CreateObject (FLGH, 2482, 1014, -1)->SetRotation(-45,-90);
  CreateObject (CHAR, 54, 309, -1);
  CreateObject (CHAR, 179, 99, -1);
  CreateObject (CHAR, 2191, 1129, -1);
  CreateObject (LCKR, 117, 310, -1);
  CreateObject (LCKR, 133, 750, -1);
  CreateObject (LCKR, 118, 750, -1);
  CreateObject (LCKR, 95, 100, -1);
  CreateObject (LCKR, 112, 100, -1);
  CreateObject (LCKR, 2396, 1240, -1);
  CreateObject (LCKR, 2164, 420, -1);
  CreateObject (LCKR, 2290, 420, -1);
  CreateObject (LCKR, 2275, 420, -1);
  CreateObject (LCKR, 1910, 930, -1);
  CreateObject (LCKR, 1890, 937, -1)->SetR(92);
  CreateObject (LCKR, 1844, 931, -1)->SetR(-9);
  CreateObject (LCKR, 2254, 1130, -1);
  CreateObject (LCKR, 2240, 1130, -1);
  CreateObject (TABL, 203, 100, -1);
  CreateObject (TABL, 151, 749, -1);
  CreateObject (TABL, 2214, 1129, -1);
  CreateObject (TABL, 2367, 1239, -1);
  CreateObject (CRAT, 91, 490, -1);
  CreateObject (CRAT, 1735, 700, -1);
  CreateObject (CRAT, 1706, 700, -1);
  CreateObject (CRAT, 1864, 750, -1);
  CreateObject (CRAT, 1887, 723, -1);
  CreateObject (CRAT, 1895, 750, -1);
  CreateObject (CRAT, 1935, 703, -1);
  CreateObject (CRAT, 1984, 701, -1);
  CreateObject (CRAT, 1076, 710, -1);
  CreateObject (CRAT, 752, 601, -1);
  CreateObject (CRAT, 745, 630, -1);
  CreateObject (CRAT, 774, 630, -1);
  CreateObject (DEB1, 429, 486, -1)->SetR(-50);
  CreateObject (DEB1, 458, 491, -1)->SetR(20);
  CreateObject (DEB1, 407, 491, -1);
  CreateObject (DEB1, 1945, 724, -1)->SetR(173);
  CreateObject (DEB1, 1938, 709, -1);
  CreateObject (DEB1, 1975, 706, -1)->SetR(-13);
  CreateObject (DEB1, 1948, 712, -1)->SetR(-13);
  CreateObject (LADR, 315, 1181, -1)->Set(23);
  CreateObject (LADR, 476, 1025, -1)->Set(14);
  CreateObject (LADR, 375, 958, -1)->Set(14);
  CreateObject (LADR, 214, 942, -1)->Set(19);
  CreateObject (LADR, 86, 795, -1)->Set(6);
  CreateObject (LADR, 186, 737, -1)->Set(6);
  CreateObject (LADR, 338, 461, -1)->Set(10);
  CreateObject (LADR, 127, 142, -1)->Set(5);
  CreateObject (LADR, 596, 201, -1)->Set(12);
  CreateObject (LADR, 546, 215, -1)->Set(5);
  CreateObject (LADR, 17, 380, -1)->Set(26);
  CreateObject (LADR, 76, 427, -1)->Set(6);
  CreateObject (LADR, 2298, 1207, -1)->Set(10);
  CreateObject (LADR, 2029, 1201, -1)->Set(24);
  CreateObject (LADR, 1835, 974, -1)->Set(5);
  CreateObject (LADR, 1794, 969, -1)->Set(14);
  CreateObject (LADR, 1877, 851, -1)->Set(5);
  CreateObject (LADR, 2255, 456, -1)->Set(4);
  CreateObject (LADR, 2197, 455, -1)->Set(4);
  CreateObject (LADR, 2296, 524, -1)->Set(5);
  CreateObject (LADR, 2165, 667, -1)->Set(18);
  CreateObject (LADR, 1667, 730, -1)->Set(15);
  CreateObject (LADR, 2067, 881, -1)->Set(10);
  CreateObject (LADR, 2258, 934, -1)->Set(10);
  CreateObject (LADR, 2365, 1105, -1)->Set(24);
  CreateObject (LADR, 820, 548, -1)->Set(5);
  CreateObject (LADR, 876, 580, -1)->Set(9);
  CreateObject (LADR, 1035, 651, -1)->Set(8);
  CreateObject (LADR, 937, 746, -1)->Set(30);
  CreateObject (LADR, 1108, 821, -1)->Set(14);
  CreateObject (LADR, 1167, 749, -1)->Set(20);
  CreateObject (LADR, 1229, 762, -1)->Set(9);
  CreateObject (CRN1, 2181, 730, -1)->Set(18);
  CreateObject (LBDR, 305, 490, -1);
  CreateObject (LBDR, 574, 110, -1);
  CreateObject (SBBA, 314, 138, -1);
  CreateObject (SPVM, 114, 380, -1);
  CreateObject (SPVM, 1609, 610, -1);
  CreateObject (TOIC, 2479, 1240, -1);
  CreateObject (CLVM, 145, 380, -1);
  CreateObject (CLVM, 2211, 1240, -1);
  CreateObject (STDR, 135, 490, -1);
  CreateObject (STDR, 2197, 1060, -1);
  CreateObject (STDR, 2432, 1240, -1);
  CreateObject (STDR, 2068, 1239, -1);
  CreateObject (STDR, 2252, 1240, -1);
  CreateObject (SLDR, 456, 380, -1);
  CreateObject (SLDR, 2132, 700, -1);
  CreateObject (SLDR, 2135, 480, -1);
  CreateObject (LFTP, 285, 933, -1);
  CreateObject (LFTP, 235, 393, -1);
  CreateObject (LFTP, 425, 354, -1);
  CreateObject (LFTP, 2090, 394, -1);
  CreateObject (LFTP, 2145, 1153, -1);
  CreateObject (LFTP, 1955, 1253, -1);
  CreateObject (AIRL, 950, 420, -1)->SetLeft();
  CreateObject (AIRL, 521, 663, -1)->SetRight();
  CreateObject (AIRL, 791, 232, -1)->SetRight();
  CreateObject (AIRL, 810, 773, -1)->SetLeft();
  CreateObject (AIRL, 1200, 851, -1)->SetRight();
  CreateObject (AIRL, 1330, 782, -1)->SetRight();
  CreateObject (AIRL, 1591, 753, -1)->SetLeft();
  CreateObject (SCR4, 185, 368, -1);
  CreateObject (ALGH, 505, 358, -1)->SetR(180);
  CreateObject (ALGH, 695, 178, -1)->SetR(180);
  CreateObject (ALGH, 286, 28, -1)->SetR(180);
  CreateObject (ALGH, 319, 258, -1)->SetR(180);
  CreateObject (ALGH, 566, 868, -1)->SetR(180);
  CreateObject (ALGH, 330, 1238, -1)->SetR(180);
  CreateObject (ALGH, 392, 638, -1)->SetR(180);
  CreateObject (ALGH, 112, 828, -1)->SetR(180);
  CreateObject (ALGH, 1638, 718, -1)->SetR(180);
  CreateObject (ALGH, 2089, 358, -1)->SetR(180);
  CreateObject (ALGH, 1847, 1109, -1)->SetR(180);
  CreateObject (ALGH, 1739, 928, -1)->SetR(180);
  CreateObject (ALGH, 2144, 948, -1)->SetR(180);
  CreateObject (ALGH, 2424, 998, -1)->SetR(180);
  CreateObject (ALGH, 1790, 548, -1)->SetR(180);
  CreateObject (ALGH, 2266, 558, -1)->SetR(180);
  var pRail = CreateObject (RAI1, 377, 850, -1);
  pRail->SetRail([1,3,1,3,1,3,1,3]);
  pRail->AdjustPart(9,2);
  CreateObject (SCR3, 511, 162, -1)->SetClrModulation(RGB(0,200,0));
  CreateObject (BLGH, 78, 279, -1);
  CreateObject (LLGH, 215, 484, -1);
  CreateObject (BLGH, 423, 230, -1);
  CreateObject (CLGH, 71, 21, -1);
  CreateObject (CLGH, 158, 641, -1);
  CreateObject (BLGH, 370, 892, -1);
  CreateObject (LCOM, 512, 111, -1);
  CreateObject (LCOM, 527, 111, -1);
  CreateObject (FRAM, 487, 100, -1);
  CreateObject (FRAM, 552, 101, -1);
  CreateObject (MVNT, 132, 709, -1);
  CreateObject (SCR2, 1274, 715, -1)->RemoveFrame();
  CreateObject (PSTR, 72, 91, -1);
  CreateObject (FENC, 439, 976, -1);
  CreateObject (FENC, 379, 976, -1);
  CreateObject (STRP, 236, 457, -1)->Set(100, RGB(0,200,0));
  CreateObject (FENC, 319, 976, -1);
  CreateObject (FENC, 259, 976, -1);
  CreateObject (ESGN, 299, 437, -1);
  CreateObject (FIEX, 298, 480, -1);
  CreateObject (FENC, 286, 140, -1);
  var pScreen = CreateObject (SCR3, 180, 850, -1);
  pScreen->SetClrModulation(65535);
  pScreen->RemoveFrame();
  pScreen = CreateObject (SCR3, 180, 961, -1);
  pScreen->SetClrModulation(65535);
  pScreen->RemoveFrame();
  CreateObject (RAI2, 36, 311, -1)->SetRail([1,1,1,2,1,1,1,2,1,1,1]);
  CreateObject (MVNT, 161, 432, -1)->SetCon(50);
  CreateObject (ENGT, 520, 45, -1)->SetCon(75);
  CreateObject (RAI1, 375, 180, -1)->SetRail([1,3,2,2,2,3,1]);
  CreateObject (HSGN, 274, 1136, -1);
  CreateObject (FENC, 362, 493, -1);
  CreateObject (CLGH, 2076, 1001, -1);
  CreateObject (CLGH, 2225, 371, -1);
  CreateObject (FRAM, 1866, 916, -1);
  CreateObject (CLGH, 1872, 881, -1);
  var pPipe = CreateObject (PMP2, 2224, 693, -1)->Up(2);
  pPipe->Right(0,0,1)->Down(2);
  CreateObject (FENC, 2173, 659, -1);
  CreateObject (FENC, 2233, 660, -1);
  CreateObject (FENC, 2350, 663, -1);
  CreateObject (FENC, 2350, 704, -1);
  CreateObject (FENC, 2233, 701, -1);
  CreateObject (FENC, 2173, 700, -1);
  CreateObject (FIEX, 1986, 642, -1);
  CreateObject (BLGH, 1812, 682, -1);
  CreateObject (FRAM, 2113, 1219, -1);
  pScreen = CreateObject (SCR3, 180, 882, -1);
  pScreen->SetClrModulation(65535);
  pScreen->RemoveFrame();
  pScreen = CreateObject (SCR1, 180, 930, -1);
  pScreen->SetClrModulation(65535);
  pScreen->RemoveFrame();
  CreateObject (LBGH, 2109, 723, -1);
  CreateObject (RAI1, 2037, 812, -1);
  CreateObject (RAI1, 2014, 812, -1);
  CreateObject (RAI1, 1990, 812, -1);
  CreateObject (RAI2, 2043, 892, -1)->SetRailLength(5);
  CreateObject (HSGN, 2105, 853, -1)->SetClrModulation(9856050);
  CreateObject (FENC, 2338, 851, -1);
  CreateObject (FENC, 2290, 851, -1);
  pPipe = CreateObject (PIPL, 2373, 895, -1)->Left(2)->Up();
  pPipe->Up(2);
  pPipe->Left()->Up()->Right(2)->Up();
  CreateObject (WSHB, 2450, 1241, -1);
  CreateObject (VENT, 2427, 1101, -1)->SetCon(45);
  CreateObject (STRP, 2188, 1213, -1)->Set(80);
  CreateObject (STRP, 2161, 1229, -1)->Set(140);
  CreateObject (CLGH, 2144, 1161, -1);
  CreateObject (GAT2, 876, 766, -1);
  CreateObject (GAT2, 1127, 845, -1);
  CreateObject (CLGH, 855, 501, -1);
  CreateObject (BLGH, 1200, 735, -1);
  CreateObject (CLGH, 1100, 551, -1);
  CreateObject (ESGN, 1985, 627, -1);
  CreateObject (FIEX, 2179, 1124, -1);
  CreateObject (VENT, 1136, 670, -1)->SetCon(30);
  CreateObject (FENC, 823, 631, -1);
  CreateObject (FENC, 760, 631, -1);
  CreateObject (VENT, 2186, 854, -1)->SetCon(30);
  CreateObject (HNG2, 2495, 821, -1);
  CreateObject (STLC, 0, 0, -1);
  CreateObject (DPIP, 0, 0, -1);
  CreateObject (STLC, 0, 0, -1);
  CreateObject(SWD1, 1370, 1080, -1);
  CreateObject(SWD1, 660, 1180, -1);
  CreateObject(SWD1, 1360, 470, -1);
  CreateObject(SWD2, 1610, 530, -1);
  CreateObject(SWD1, 740, 470, -1);
  CreateObject(SWD2, 10, 720, -1);
  // Waffen & Ausrüstung
  PlaceSpawnpoint(ENWP, 100, 170);
  PlaceSpawnpoint(ENAP, 80, 170);
  PlaceSpawnpoint(PGWP, 280, 380);
  PlaceSpawnpoint(STAP, 300, 380);
  PlaceSpawnpoint(KRFL, 770, 190);
  PlaceSpawnpoint(AIRT, 1070, 660);
  PlaceSpawnpoint(GLWP, 1200, 680);
  PlaceSpawnpoint(GRAP, 1180, 680);
  PlaceSpawnpoint(HARM, 1170, 1200);
  PlaceSpawnpoint(ENWP, 200, 740);
  PlaceSpawnpoint(ENAP, 220, 740);
  PlaceSpawnpoint(PGWP, 590, 1010);
  PlaceSpawnpoint(STAP, 610, 1010);
  PlaceSpawnpoint(KSLM, 290, 1170);
  PlaceSpawnpoint(ENWP, 2170, 1230);
  PlaceSpawnpoint(ENAP, 2150, 1230);
  PlaceSpawnpoint(PGWP, 2070, 880);
  PlaceSpawnpoint(STAP, 2090, 880);
  PlaceSpawnpoint(KLAS, 1900, 800);
  PlaceSpawnpoint(ENWP, 2170, 410);
  PlaceSpawnpoint(ENAP, 2150, 410);
  PlaceSpawnpoint(PGWP, 1610, 600);
  PlaceSpawnpoint(DRSU, 1980, 640);
  PlaceSpawnpoint(MEDI, 1090, 350);
  PlaceSpawnpoint(MINE, 660, 550);
  PlaceSpawnpoint(MINE, 1430, 840);
  PlaceSpawnpoint(GRAP, 760, 680);
  PlaceSpawnpoint(GRAP, 1540, 270);
  PlaceSpawnpoint(ENAP, 1100, 20);
  PlaceSpawnpoint(STAP, 990, 930);
  PlaceSpawnpoint(MIWP, 160, 590);
  PlaceSpawnpoint(STAP, 180, 590);
  PlaceSpawnpoint(GGWP, 410, 1190);
  PlaceSpawnpoint(ENAP, 430, 1190);
  PlaceSpawnpoint(BZWP, 1780, 1260);
  PlaceSpawnpoint(MIAP, 1800, 1260);
  PlaceSpawnpoint(GGWP, 1580, 640);
  PlaceSpawnpoint(ENAP, 1560, 640);
  PlaceBonusSpawnpoint([AMPB,BSKB,HELB,INVB,KAMB,RPFB], 740, 530);
	
	aHivePos = [[[550, 940], [120, 910], [330, 880]], [[2400, 1100], [2080, 1100], [2200, 900]], [[2300, 600], [2050, 600], [1800, 700]], [[300, 333], [280, 100]], [[1100, 800], [850, 600], [1000, 600]]];
	
}

static aHivePos;

public func HivePos(int iAlien) {
	var aBase = aHivePos[iAlien % GetLength(aHivePos)];
	return aBase[(iAlien / GetLength(aHivePos)) % GetLength(aBase)];
}

public func GoalChosen()
{
	if(FindObject(GAH_))
	{
		var alar = CreateObject(ALAR);
		alar->SetMaxAliens(14);
		// alle kommen ins gleiche Team
		for(var i=0; i<GetPlayerCount(); ++i)
		{
			SetPlayerTeam (GetPlayerByIndex(i), 1); 
			SetWealth(i, 0);
			
			// Leute die schon falsch platziert wurden...
			var pos = RelaunchPosition(1);
			var iX = pos[0];
			var iY = pos[1];
			var pCrew = GetCursor(i);
			if(Contained(pCrew))
				SetPosition(iX, iY, Contained(pCrew));
			else
				SetPosition(iX, iY, pCrew);
		}
	}
}

public func InitCTF()
{
  // Warps zur Basis
  CreateObject(AWRP, 810, 627, -1)->Set(80,300);
  CreateObject(AWRP, 996, 590, -1)->Set(420,840);
  CreateObject(AWRP, 1080, 760, -1)->Set(1880,909);
  CreateObject(AWRP, 1328, 680, -1)->Set(1718,662); 
	
	for(var i=0; i<10; ++i)
		PlaceAnimal(FISH);
	
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

public func InitAlienhunt()
{
  // Tore und Konsolen
  var pObj, pObj2, pGate, pGate2;
  
  //CreateObject(HNG1, 700, 728, -1) -> SetObjDrawTransform(895, 0, 0, 0, 1000);
  pObj = CreateObject(CNS2, 2030, 1240, -1);
  pObj2 = CreateObject(CONS, 215, 680, -1);
  pGate = CreateObject(_HG2, 700, 728, -1);
  pGate2 = CreateObject(_HG1, 255, 1188, -1);
  if(Random(2)) {
  	pObj -> Set(pGate);
  	pObj2 -> Set(pGate2);
  }
  else {
  	pObj -> Set(pGate2);
  	pObj2 -> Set(pGate);
  }
  CreateObject(CONS, 1635, 740, -1) -> Set(CreateObject(HNG1, 1480, 538, -1));
  //CreateObject(HNG1, 255, 1188, -1) -> SetObjDrawTransform(350, 0, 0, 0, 1000);
  
  
  // Tür und Schalter
  pObj = CreateObject(SWTH, 971, 530, -1);
  pGate = CreateObject(SEDR, 1011, 530, -1);
  pGate -> Lock();
  pObj -> SetPhase(1);
  pObj -> Set(pGate);

	for(var i=0; i<10; ++i)
		PlaceAnimal(ALN6);

	var pAlien;
	for(pAlien in FindObjects(Find_Or(
																		Find_And(
																							Find_ID(ALN6), 
																							Find_Or(
																											Find_InRect(350, 0, 860-350, 160), 
																											Find_InRect(2240, 0, LandscapeWidth()-2240, 900), 
																											Find_InRect(1820, 420, 2050-1820, 520-420), 
																											Find_InRect(1494, 790, 1650-1494, 860-790), 
																											Find_InRect(0, 1060, 200, LandscapeHeight()-1060), 
																											Find_InRect(330, 670, 580-330, 840-670)
																							)
																		), 
																		Find_And(
																							Find_Func("IsAlien"), 
																							Find_InRect(1685, 60, 2124-1685, 220-60)
																		)
														)))
		pAlien -> RemoveObject();

	if(FindObject(WPCH))
	{
		var wp;
		wp = CreateObject(WPVM, 675,230);
		AddWares(wp);
		wp = CreateObject(WPVM, 840,630);
		AddWares(wp);
	}
		
  ScriptGo(1);
}

private func AddWares(object wp)
{
	// und begrenzte Waren...
	wp->RemoveWare();
	wp->AddWare(PIWP,-1);  // Pistole
	wp->AddWare(ENWP,-1);  // Energiegewehr
	wp->AddWare(PGWP,-1);  // Pumpgun
	wp->AddWare(GGWP,-1);  // Partikelkanone
	wp->AddWare(BZWP,-1);  // Bazooka
	wp->AddWare(MIWP,-1);  // Minigun
	wp->AddWare(KRFL,-1);  // Waffenteile
	wp->AddWare(KSLM,-1);  // Schleim
	wp->AddWare(FLSH,-1);  // Taschenlampe
	wp->AddWare(AIRT,-1);  // Luft!
	wp->AddWare(HARM,-1);  // Rüstung
	wp->AddWare(DRSU,-1);  // Drone
	wp->AddWare(MEDI,-1);  // Medipack
  //wp->SortWare();
}

/* Regelwähler */

public func ChooserFinished()
{
  Arena_ChooserFinished();

  var wp = FindObject(WPCH);
  if(wp)
  {
		AddWares(wp);
  }
	
	if(FindObject(GAH_))
	{
		InitAlienhunt();
	}
	else if(FindObject(GCTF))
	{
		InitCTF();
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
		else if(FindObject(GAH_))
		{
			// alle kommen ins gleiche Team
			SetPlayerTeam (iPlr, 1); 
			iTeam = 1;
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
  if(iTeam == 1)  return [30, 80];
  if(iTeam == 2)  return [2400,1220];
  if(iTeam == 3)  return [120,720];
  if(iTeam == 4)  return [2280, 400];
}

/* Flaggenkrams */

protected func InitTeamFlag(int iTeam)
{
  var x,y,color;
  if(iTeam == 1)
  {
    x = 285;
    y = 140;
    color = RGB(0,200,0);
  }
  if(iTeam == 2)
  {
    x = 2435;
    y = 1130;
    color = RGB(0,0,255);
  }
  if(iTeam == 3)
  {
    x = 120;
    y = 970;
    color = 65535;
  }
  if(iTeam == 4)
  {
    x = 2275;
    y = 700;
    color = RGB(255,0,0);
  }
  CreateFlag(iTeam, x, y, color);
}

public func FlagReturned(team, clonk)
{
  Alarm(false, team);
}

public func FlagCaptured(team, enemyteam, clonk)
{
  Alarm(true, team);
}

public func BaseRect(int team, &x, &y, &width, &height)
{
  if(team == 1) {
    x = 0;
    y = 0;
    width = 800;
    height = 520;
  }
  if(team == 3) {
    x = 0;
    y = 600;
    width = 700;
    height = 680;
  }
  if(team == 4) {
    x = 1500;
    y = 300;
    width = 1000;
    height = 440;
  }
  if(team == 2) {
    x = 1500;
    y = 760;
    width = 1000;
    height = 540;
  }
}

public func Alarm(bool alarm, int team)
{
  // welches Team an welcher Position ist, muss noch angepasst werden

  var x,y,width,height;
  
	BaseRect(team,x,y,width,height);

  // nach Alarmleuchten und Bildschirmen gucken
  var pLightObj;
  for(var pLightObj in FindObjects(Find_Or(Find_ID(ALGH),Find_ID(SCR1),Find_ID(SCR3)),Find_InRect(x,y,width,height)))
  {
    // Alarm an und ausmachen, 
    // Bildschirme rot/blau färben
    if(alarm)
    {
      if(GetID(pLightObj) == ALGH)
        pLightObj -> TurnOn();
      else 
        pLightObj -> SetClrModulation(RGB(255,0,0));
    }
    else
    {
      if(GetID(pLightObj) == ALGH)
        pLightObj -> TurnOff();
      else 
        pLightObj -> SetClrModulation(RGB(0,255,255));
    }
  }
}

protected func Script10() {
	var x,y,width,height,alarm;
  
  for(var i = 4; i > 0; i--) {
		BaseRect(i,x,y,width,height);
		alarm = ObjectCount2(Find_InRect(x,y,width,height), Find_Func("IsAlien"));

		Alarm(!!alarm, i);
	}
  goto(0);
}