/* Tutorial 1 */

#strict 2

func Initialize()
{
	// Always have some music
	Music("Frontend", 1);
	// Create lens flare
	CreateObject(LENS, 70, 50, NO_OWNER);
	// Place hut
	CreateConstruction(HUT2,580,180,NO_OWNER,100,1);
	// Create goal
	CreateObject(SCRG, 50,50, NO_OWNER);
	// Start script
	ScriptGo(true);
	// Evaluation dialog options
  SetNextMission("Tutorial.c4f\\Tutorial01.c4s", "$BtnRepeatRound$", "$BtnRepeatRoundDesc$");
	return true;
}

func InitializePlayer(int plr)
{
	GetCrew(plr)->SetPhysical("CanDig", 0, 2);
	GetCrew(plr)->SetPosition(320, -50);
	GetCrew(plr)->SetAction("Tumble");
	// Message positioning
	SetPlrShowControlPos(plr, SHOWCTRLPOS_TopLeft);
	SetTutorialMessagePos(MSG_Top | MSG_Left | MSG_WidthRel | MSG_XRel, 50, 50, 30);
	return true;
}

func Script1()
{
	Sound("Drop");
}

func Script5()
{
	Sound("Boing");
}

func Script10()
{
	TutorialMessage("$TxtThisisaclonk$");
}

func Script20()
{
	SetPlrShowControl(0,"____4_678_          __________");
	Sound("Ding");
}

func Script21()
{
  SetPlrShowControl(0,"____4_678_    4 678 ____4_678_");
  TutorialMessage("$TxtThosearethekeysusedt$");
}

func Script50()
{
  SetPlrShowControl(0,"____4_678_    4 678 __________");
  CreateObject(FLAG,40,-10,0);
  SetArrow(45, 210);
  TutorialMessage("$TxtJumponthehilltoyourl$");
}

func Script60()
{
  if(GetID(Contents(0,GetHiRank(0)))!=FLAG)
    { goto(60); return; }
  Sound("Applause");
  RemoveArrow();
}

func Script66()
{
	TutorialMessage("$TxtVeryGood$");
}

func Script72()
{
  var cabin = FindObject(HUT2);
  SetArrow(583, 120);
  TutorialMessage("$TxtNowmovetothecabinont$");
}

func Script80()
{
	var pHut = FindObject(HUT2);
  if(!FindObject(0,pHut->GetX()-80,pHut->GetY()-50,100,100,OCF_CrewMember)) { goto(80); return; }
  SetPlrShowControl(0,"____4_678_    4 678 ____4_____");
  TutorialMessage("$TxtStopexactlyinfrontof$");
  goto(110);
}

func Script110()
{
  if(!Contained(GetHiRank(0))) { goto(110); return; }
  RemoveArrow();
  SetPlrShowControl(0,"___34_678_   34 678 ___3______");
  Sound("Ding");
  TutorialMessage("$TxtUsethiskeytoputdownt$");
}

func Script120()
{
  if(GetBase(FindObject(HUT2))) { goto(120); return; }
  SetPlrShowControl(0,"___34_678_   34 678 __________");
  TutorialMessage("$TxtTheflagturnsthecabin$");
}

func Script148()
{
  if(!Contained(GetHiRank(0))) { goto(150); return; }
  SetPlrShowControl(0,"___34_678_   34 678 ___3___7__");
  TutorialMessage("$TxtUsethiskeytoexitthec$");
}

func Script149()
{
  if (Contained(GetHiRank(0))) { goto(149); return; }
}

func Script150()
{
  SetPlrShowControl(0,"___34_678_   34 678 __________");
  var pGold = CreateObject(GOLD,130,330);
  TutorialMessage("$TxtNowwalkbacktothevall$");
  SetArrow(220, 271);
}

func Script160()
{
  if(!FindObject(0,150,250,100,100,OCF_CrewMember)) { goto(160); return; }
  Sound("Ding");
  RemoveArrow();
  SetPlrShowControl(0,"___345678_   345678 _____5____");
  ResetPhysical(GetCrew());
  TutorialMessage("$TxtUsethiskeytodigdiago$");
}

func Script170()
{
  if(GetAction(GetHiRank(0)) != "Dig") { goto(170); return; }
  SetPlrShowControl(0,"___345678_   345678 ____4_678_");
  TutorialMessage("$TxtWhilediggingusethese$");
}

func Script200()
{
  if (GetID(Contents(0,GetHiRank(0)))==GOLD) { goto(205); return; }
  TutorialMessage("$TxtNowtrytodigoutthechu$");
  SetArrowToObj(FindObject(GOLD));
}

func Script205()
{
  if (GetID(Contents(0,GetHiRank(0)))!=GOLD) { goto(205); return; }
  Sound("Applause");
  RemoveArrow();
  SetPlrShowControl(0,"___345678_   345678 __________");
}

func Script206()
{
	TutorialMessage("$TxtWonderful$");
}

func Script210()
{
  TutorialMessage("$TxtTakethegoldintotheca$");
}

func Script215()
{
  if (!Contained(GetHiRank(0))) { goto(215); return; }
  TutorialMessage("$TxtVerygoodYouhavecompl$");
  FindObject(SCRG)->Fulfill();
  SetNextMission("Tutorial.c4f\\Tutorial02.c4s", "$BtnNextTutorial$", "$BtnNextTutorialDesc$");
}
