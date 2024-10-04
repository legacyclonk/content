/*-- Fressfeind --*/

#strict

static alienengt;
static down;

protected func Initialize()
{
  // Himmel
  SetSkyAdjust(RGB(255,0,255), RGB(255,128,255));

CreateObject(TRB2,850,97,-1);
CreateObject(TRB1,949,148,-1);
AddEffect("Spezialzauber", CreateObject(CRAT,582,180,-1), 1, 0);
CreateObject(CRAT,644,208,-1);
CreateObject(TRB1,800,225,-1);
CreateObject(TRB3,1093,221,-1);
var obj0 = CreateObject(GRAS,49,245,-1);
obj0->SetR(-3);
obj0->SetCon(57);
var obj1 = CreateObject(GRAS,14,242,-1);
obj1->SetR(1);
obj1->SetCon(57);
CreateObject(TRB2,204,257,-1);
var obj2 = CreateObject(GRAS,283,255,-1);
obj2->SetCon(55);
var obj3 = CreateObject(GRAS,250,255,-1);
obj3->SetCon(73);
var obj4 = CreateObject(LKEY,486,243,-1);
var obj5 = CreateObject(HSGN,458,245,-1);
obj5->SetCon(50);
var obj6 = CreateObject(TELE,526,251,-1);
obj6->Deactivate();
CreateObject(TRB2,728,247,-1);
var obj7 = CreateObject(BONE,924,241,-1);
obj7->SetR(35);
var obj8 = CreateObject(BONE,928,241,-1);
obj8->SetR(45);
var obj9 = CreateObject(BONE,918,241,-1);
obj9->SetR(42);
CreateObject(BUSB,1214,222,-1);
CreateObject(SKUL,1301,219,-1);
var obj10 = CreateObject(HTB1,1377,226,-1);
obj10->SetR(75);
CreateObject(SKUL,1442,228,-1);
var obj11 = CreateObject(GRAS,121,261,-1);
obj11->SetR(2);
obj11->SetCon(50);
var crat = CreateObject(CRAT,631,499,-1);
crat->CreateContents(KRFL, 0, 2);
crat->CreateContents(HARM);
crat->CreateContents(MINE);
crat->CreateContents(ALN2, 0, 2);
AddEffect("Spezialzauber2", crat, 1, 0);
var obj12 = CreateObject(LADR,848,590,-1);
obj12->Set(13);
var obj13 = CreateObject(IDOL,450,523,-1);
obj13->SetR(105);
alienengt = CreateObject(ENGT,671,605,-1);
alienengt->SetCon(75);
var obj15 = CreateObject(LADR,817,818,-1);
obj15->Set(28);
var obj16 = CreateObject(ALGH,837,598,-1);
obj16->SetR(-90);
obj16->TurnOff();
CreateObject(GSBO,871,590,-1)->SetAction("Empty");
CreateObject(GSBO,860,590,-1);
var obj17 = CreateObject(LADR,981,710,-1);
obj17->Set(19);
CreateObject(CRAT,1301,590,-1);
var obj18 = CreateObject(PIPL,372,637,-1);
var obj19 = CreateObject(PMP2,422,637,-1);
obj19->Solid();
CreateObject(FIEX,451,630,-1);
var obj20 = CreateObject(TELE,528,641,-1);
obj20->SetTarget(obj6);
obj6->SetTarget(obj20);
CreateObject(LCKR,587,640,-1);
CreateObject(BLGH,569,609,-1);
CreateObject(LCKR,606,640,-1);
CreateObject(STDR,636,641,-1);
var obj21 = CreateObject(LADR,675,850,-1);
obj21->Set(27);
var obj22 = CreateObject(PIPL,372,687,-1);
var obj23 = CreateObject(CLGH,597,651,-1);
obj23->TurnOff();
var obj24 = CreateObject(PIPL,222,737,-1);
var obj25 = CreateObject(PIPL,272,737,-1);
CreateObject(VALV,322,716,-1);
var obj26 = CreateObject(PIPL,322,737,-1);
var obj27 = CreateObject(PIPL,372,737,-1);
var obj28 = CreateObject(VENT,429,726,-1);
obj28->SetCon(40);
CreateObject(SPVM,473,730,-1);
CreateObject(PLNT,500,718,-1);
CreateObject(TABL,509,729,-1);
CreateObject(BED2,576,729,-1);
CreateObject(BED2,614,729,-1);
CreateObject(STDR,646,730,-1);
var obj29 = CreateObject(ENGT,269,795,-1);
obj29->SetCon(75);
var obj30 = CreateObject(PIPL,322,787,-1);
CreateObject(GSTA,312,849,-1);
var obj31 = CreateObject(PIPL,322,837,-1);
var obj32 = CreateObject(LLGH,376,842,-1);
obj32->TurnOff();
var obj33 = CreateObject(OPTB,403,849,-1);
obj33->TurnOff();
var obj34 = CreateObject(FRAM,431,822,-1);
obj34->Set(3);
CreateObject(MONI,464,838,-1);
var obj35 = CreateObject(GADG,450,836,-1);
obj35->Set(3);
CreateObject(LTBL,454,849,-1);
CreateObject(GLST,496,850,-1);
var obj36 = CreateObject(LBDR,542,850,-1);
obj36->SetDir(DIR_Left);
obj36->UpdateSolidMask();
CreateObject(GLST,522,850,-1);
var obj37 = CreateObject(SLDR,695,850,-1);
CreateObject(_SPA,933,668,-1);
CreateObject(TANK,897,883,-1);
  var card = obj6->CreateContents(KCRD);
  card->SetKeyNumber(1);
obj4->Set(obj6, 1);
obj18->ConnectTo(Object(19), PIPE_Right);
obj18->ConnectTo(Object(22), PIPE_Down);
obj19->ConnectTo(Object(18), PIPE_Left);
obj22->ConnectTo(Object(18), PIPE_Up);
obj22->ConnectTo(Object(27), PIPE_Down);
obj24->ConnectTo(Object(25), PIPE_Right);
obj25->ConnectTo(Object(24), PIPE_Left);
obj25->ConnectTo(Object(26), PIPE_Right);
obj26->ConnectTo(Object(25), PIPE_Left);
obj26->ConnectTo(Object(27), PIPE_Right);
obj26->ConnectTo(Object(30), PIPE_Down);
obj27->ConnectTo(Object(26), PIPE_Left);
obj27->ConnectTo(Object(22), PIPE_Up);
obj30->ConnectTo(Object(26), PIPE_Up);
obj30->ConnectTo(Object(31), PIPE_Down);
obj31->ConnectTo(Object(30), PIPE_Up);
}

/* Relaunch */

public func ChooserFinished()
{
	// Erste Alienkolonie erzeugen
	CastObjects(ALN2, 10, 10, 1300, 180);
	CastObjects(ALN4, 11, 10, 1300, 180);
	PlaceBubbles(10, 1100, 150, 350, 100);
	PlaceBubbles(7, 1000, 560, 280, 70);
	// Waffen
	PlaceWeapons();
	ScriptGo(1);
	
	if(FindObject(WPCH))
	{
		var wp;
		wp = CreateObject(WPVM, 470,640);
		AddWares(wp);
		
		wp = FindObject(WPCH);
		AddWares(wp);
	}
	
	Arena_ChooserFinished();
}

private func AddWares(object wp)
{
	wp->RemoveWare();
	wp->AddWare(PIWP,-1);
	wp->AddWare(GLWP,-1);
	wp->AddWare(ENWP,-1);
	wp->AddWare(FTWP,-1);
	wp->AddWare(NLGT,-1);
	wp->AddWare(MEDI,-1);
}

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
	return [140,250];
}

/* Alienzeugs */

protected func PlaceBubbles(int iNumber, int iX, int iY, int iWdt, int iHgt)
{
  for(var i=0,j,x ; i < iNumber ; i++)
  {
    x = Random(iWdt);
    j = 0;
    while(GBackSemiSolid(iX + x, iY + iHgt - j) && j < iHgt) j++;
    if(j < iHgt) var bubble = CreateObject(HBBL, iX+x, iY+iHgt-j, -1);
    // blöder Workaround
    if(bubble) while(GBackSemiSolid(bubble->GetX(), bubble->GetY())) bubble->SetPosition(bubble->GetX(), bubble->GetY()-1);
  }
}

protected func PlaceEggs(int iNumber, int iX, int iY, int iWdt, int iHgt, id idAlien)
{
  for(var i=0,j,x ; i < iNumber ; i++)
  {
    x = Random(iWdt);
    j = 0;
    while(!GBackSolid(iX + x, iY + j) && iY + j < iY + iHgt) j++;
    CreateObject(HBBL, iX+x, iY+j, -1)->Set(idAlien);
  }
}

/* Waffen und Muni */

public func PlaceWeapons()
{
	PlaceSpawnpoint(GRAP, 70, 230);
	PlaceSpawnpoint(STAP, 60, 230);
	PlaceSpawnpoint(GLWP, 590, 630);
	PlaceSpawnpoint(GRAP, 600, 630);
	PlaceSpawnpoint(ENWP, 410, 720);
	PlaceSpawnpoint(ENAP, 420, 720);
	PlaceSpawnpoint(NLGT, 260, 790);
	PlaceSpawnpoint(MEDI, 330, 840);
}

/* Script */

static bombardment;

protected func Script2()
{
  SetDarkness();
  Sound("RadioStatic");
  Message("@$Intro$");
}

protected func Script15()
{
  Sound("RadioStatic");
  Message("$Intro2$");
  CreateObject(AIRS, 160, 250, -1);
  CreateObject(AIRS, 160, 250, -1);
}

protected func Script25()
{
  if(!bombardment)
  {
    if(GetLength(FindObjects(Find_InRect(0,0, LandscapeWidth(),300), Find_Func("IsAlien"))) < 15)
    {
      CastObjects(ALN2, 2, 10, 1300, 180);
      PlaceBubbles(3, 1100, 150, 350, 100);
      PlaceEggs(5, 1100, 150, 350, 100, ALN2);
    }
    return(goto(16));
  }
}

protected func Script26()
{
  Sound("RadioStatic");
  Message("$Bombs$");
}

protected func Script42()
{
  if(GetLength(FindObjects(Find_InRect(0,0, LandscapeWidth(),300), Find_Func("IsAlien"))))
    return(goto(41));
  Sound("RadioStatic");
  Message("$Keycard$");
}

protected func Script45()
{
  var box = CreateObject(SBOX, 130, 250, -1);
  FindObject(KCRD)->Enter(box);

  CreateObject(HCON, 1280, 810, -1);
  PlaceBubbles(10, 560, 460, 230, 80);
  PlaceBubbles(10, 1010, 550, 340, 60);
  CreateObject(ALN1, 1301, 560, -1);
  CastObjects(ALN2, 7, 10, 1301, 560);
  CastObjects(ALN4, 2, 10, 1301, 560);
  
}

protected func Script47()
{
  if(!GetLength(FindObjects(Find_InRect(400,570, 220,70), Find_OCF(OCF_CrewMember)))) return(goto(46));
  SetDarkness(8);
  down = true;
}

protected func Script49()
{
  if(!GetLength(FindObjects(Find_InRect(650,570, 40,80), Find_OCF(OCF_CrewMember)))) return(goto(48));
  alienengt->DoDamage(100);
  alienengt->CreateObject(ALN2, 0,0, -1)->SetXDir(-10);
  ScriptGo();
}

global func FxSpezialzauberStop(object target, int no, int reason)
{
	if(reason == 3)
	{
		PlaceSpawnpoint(FTWP, 575,180, 2000);
		PlaceSpawnpoint(GSAP, 585,180, 2000);
	}
}

global func FxSpezialzauber2Stop(object target, int no, int reason)
{
	if(reason == 3)
	{
		CastObjects(ALN2, 7, 10, 690, 450);
		CastObjects(ALN2, 2, 10, 690, 450);
	}
}
