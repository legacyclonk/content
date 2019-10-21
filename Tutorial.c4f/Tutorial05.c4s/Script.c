/* Tutorial 2 */

#strict 2

static constructor_clnk;
static valley_clnk;
static catapult_clnk;
static valley_cata;
static hill_cata;
static valley_wood;
static the_elevator;

func Initialize()
{
	// Always have some music
	Music("Frontend", 1);
	// Create lens flare
	if (!ObjectCount(LENS)) CreateObject(LENS, 400, 100);
	// Fix vines (are not correctly saved in this Objects.txt)
	var pVine;
	for (pVine in FindObjects(Find_ID(VINE)))
		pVine->SetPosition(pVine->GetX(), pVine->GetY() - 35);
	// Create goal
	if (!ObjectCount(SCRG)) CreateObject(SCRG);
	// Evaluation dialog options
  SetNextMission("Tutorial.c4f\\Tutorial05.c4s", "$BtnRepeatRound$", "$BtnRepeatRoundDesc$");
}

func InitializePlayer(int plr)
{
	// Create buildings
	the_elevator = CreateConstruction(ELEV,165,110,0,80,1);
	valley_cata = CreateObject(CATA,240,387,0);
	hill_cata = CreateObject(CATA,540,247,0);
	valley_wood = CreateObject(WOOD,280,390,NO_OWNER);
	CreateObject(METL,285,390,NO_OWNER);
	// Remember clonks
	constructor_clnk = GetCrew(plr,0);
	valley_clnk = GetCrew(plr,1);
	catapult_clnk = GetCrew(plr,2);
	// Move valley and catapult clonk
	Exit(valley_clnk,230,390);
	Exit(catapult_clnk,520,250);
	// Prevent constructor and catapult clonks from falling down
	var hr = AddEffect("StayNearElev", constructor_clnk, 1, 30, 0, 0, 0, 0, 0, 0);
	AddEffect("StayNearCata", catapult_clnk, 1, 30);
	// Message and control positioning
	SetPlrShowControlPos(plr, SHOWCTRLPOS_Top);
	SetTutorialMessagePos(MSG_Bottom | MSG_Left | MSG_XRel | MSG_WidthRel, 10, -10, 35);
	// Start script
	ScriptGo(1);
	return true;
}

func Script1()
{
	SetCommand(constructor_clnk,"Build",FindObject(ELEV));
}

func Script20()
{
	TutorialMessage("$TxtThematerialneededtoc$");
	wait(10);
}

func Script34()
{
	Sound("Ding");
	SetPlrShowControl(0,"0_2_______0123456789__2_______");
	TutorialMessage("$TxtUsecommandselectleft$");
	SetArrowToObj(valley_clnk);
	wait(15);
}

func Script35()
{
	if (GetCursor(0)!=valley_clnk) return(goto(35));
}

func Script36()
{
	SetPlrShowControl(0,"0_2345678_0123456789________8_");
	TutorialMessage("$TxtWiththisclonkcollect$");
	SetArrowToObj(valley_wood);
	wait(15);
}

func Script37()
{
	if (!Contents(0,valley_clnk)) return(goto(37));
	SetPlrShowControl(0,"0_2345678_0123456789_______7__");
	TutorialMessage("$TxtNowstopinfrontofthec$");
	SetArrowToObj(valley_cata);
	wait(15);
}

func Script38()
{
	if (GetAction(valley_clnk) != "Push") return(goto(38));
	RemoveArrow();
	if (GetDir(GetActionTarget(0,valley_clnk))==DIR_Right) return(goto(50));
	SetPlrShowControl(0,"0_2345678_0123456789________8_");
	TutorialMessage("$TxtTurnthecatapulttothe$");
	wait(10);
}

func Script39()
{
	if (GetDir(GetActionTarget(0,valley_clnk))!=DIR_Right) return(goto(39));
}

func Script40()
{
	SetPlrShowControl(0,"0_2345678_0123456789_______7__");
	if (GetComDir(valley_clnk)!=COMD_Stop) return(goto(40));
	SetPlrShowControl(0,"0_2345678_0123456789__________");
	goto(50);
}

func Script50()
{
	if (GetX(GetActionTarget(0,valley_clnk))<260) return(goto(60));
	TutorialMessage("$TxtYoupushedthecatapult$");
	SetPlrShowControl(0,"0_2345678_0123456789______5___");
	wait(15);
}

func Script51() { goto(38); }

func Script60()
{
	SetPlrShowControl(0,"0_2345678_0123456789___3______");
	TutorialMessage("$TxtHitthrowtoloadthecat$");
	wait(15);
}

func Script61()
{
	if (!Contents(0,GetActionTarget(0,valley_clnk))) return(goto(61));
	SetPlrShowControl(0,"0_2345678_0123456789___3___7__");
	TutorialMessage("$TxtHitthrowagaintofiret$");
	wait(15);
}

func Script62()
{
	if (FindObject(WOOD,460,150,180,140)||FindObject(METL,460,150,180,140)) return(goto(63));
	goto(62);
}

func Script63()
{
	SetPlrShowControl(0,"0_2345678_0123456789__2_______");
	TutorialMessage("$TxtUsecommandselectright2$");
	SetArrowToObj(catapult_clnk);
	wait(15);
}

func Script64()
{
	if (GetCursor(0)!=catapult_clnk) return(goto(64));
	RemoveArrow();
	SetPlrShowControl(0,"0_2345678_0123456789__________");
	TutorialMessage("$TxtCollectthematerialyo$");
	wait(15);
}

func Script65()
{
	if (!Contents(0,catapult_clnk)) return(goto(65));
	SetPlrShowControl(0,"0_2345678_0123456789_______7__");
	TutorialMessage("$TxtNowgrabtheothercatap$");
	SetArrowToObj(hill_cata);
	wait(15);
}

func Script66()
{
	if (GetAction(catapult_clnk) != "Push") return(goto(66));
 	RemoveArrow();
	if (GetDir(GetActionTarget(0,catapult_clnk))==DIR_Left) return(goto(80));
	SetPlrShowControl(0,"0_2345678_0123456789______6___");
	TutorialMessage("$TxtTurnthecatapultaroun$");
	wait(10);
}

func Script67()
{
	if (GetDir(GetActionTarget(0,catapult_clnk))!=DIR_Left) return(goto(67));
}

func Script68()
{
	SetPlrShowControl(0,"0_2345678_0123456789_______7__");
	if (GetComDir(catapult_clnk)!=COMD_Stop) return(goto(68));
	SetPlrShowControl(0,"0_2345678_0123456789__________");
	goto(80);
}

func Script80()
{
	SetPlrShowControl(0,"0_2345678_0123456789___3______");
	TutorialMessage("$TxtHitthrowtoloadthecat$");
	wait(15);
}

func Script81()
{
	if (!Contents(0,GetActionTarget(0,catapult_clnk))) return(goto(81));
	SetPlrShowControl(0,"0_2345678_0123456789___3___7__");
	TutorialMessage("$TxtFlingthematerialupto$");
	wait(20);
}

func Script82()
{
	if (FindObject(WOOD,0,0,220,140)||FindObject(METL,0,0,220,140)) return(goto(83));
	goto(82);
}

func Script83()
{
	SetPlrShowControl(0,"0_2345678_0123456789__2_______");
	TutorialMessage("$TxtUsingtheselectioncom$");
	SetArrowToObj(constructor_clnk);
	wait(10);
}

func Script84()
{
	if (GetCursor(0)!=constructor_clnk) return(goto(84));
 	RemoveArrow();
	SetPlrShowControl(0,"0_2345678_0123456789__________");
	TutorialMessage("$TxtCollectthematerialyo$");
	wait(10);
}

func Script85()
{
	if (!Contents(0,constructor_clnk)) return(goto(85));
	SetPlrShowControl(0,"0_2345678_0123456789_______6__");
	TutorialMessage("$TxtWithadoublestopatthe$");
 	SetArrowToObj(the_elevator);
	wait(10);
}

func Script110()
{
	RemoveArrow();
	SetPlrShowControl(0,"0_2345678_0123456789__________");
	TutorialMessage("$TxtInthesamewaytranspor$");
}

func Script150()
{
	if (!FindObject(ELEC)) goto(150);
}

func Script151()
{
	Sound("Applause");
}

func Script160()
{
	SetPlrShowControl(0,"0_2345678_012345678 _______7__");
	TutorialMessage("$TxtGrabtheelevatorcasew$");
	wait(15);
}

func Script161()
{
	if (GetAction(constructor_clnk) != "Push") return(goto(161));
	if (GetID(GetActionTarget(0,constructor_clnk))!=ELEC) return(goto(161));
	SetPlrShowControl(0,"0_2345678_012345678 _____5____");
	TutorialMessage("$TxtHitdigtwicequicklyto$");
}

func Script162()
{
	if (GetAction(FindObject(ELEC))!="Drill") return(goto(162));
	RemoveEffect("StayNearElev",constructor_clnk,0,0);
	RemoveEffect("StayNearCata",catapult_clnk,0,0);
	SetPlrShowControl(0,"0_2345678_012345678 __________");
}

func Script170()
{
	TutorialMessage("");
	goto(190);
}

func Script200()
{
	TutorialMessage("$TxtNowmakeallclonksletg$");
	// Cheat: auto-ungrab all clonks
	var pClonk;
	for (pClonk in FindObjects(Find_ID(CLNK)))
		pClonk->SetAction("Walk");
}

func Script210()
{
	if (GetY(valley_clnk)<350) return(goto(210));
	if (GetY(catapult_clnk)<350) return(goto(210));
	if (GetY(constructor_clnk)<350) return(goto(210));
	if (GetAction(valley_clnk)!="Walk") return(goto(210));
	if (GetAction(catapult_clnk)!="Walk") return(goto(210));
	if (GetAction(constructor_clnk)!="Walk") return(goto(210));
	SetPlrShowControl(0,"012345678_012345678 _1________");
	TutorialMessage("$TxtHitselectiontoggletw$");
	wait(20);
}

func Script211()
{
	if (GetSelectCount(0)<3) return(goto(211));
	SetPlrShowControl(0,"012345678_012345678 __________");
	TutorialMessage("$TxtNowmoveallclonksback$");
}

func Script212()
{
	if (!Contained(valley_clnk)) return(goto(212));
	if (!Contained(catapult_clnk)) return(goto(212));
	if (!Contained(constructor_clnk)) return(goto(212));
	TutorialMessage("$TxtVeryGood$");
	FindObject(SCRG)->Fulfill();
	SetNextMission("Tutorial.c4f\\Tutorial06.c4s", "$BtnNextTutorial$", "$BtnNextTutorialDesc$");
}

global func FxStayNearElevTimer(pTarget, iEffectNumber)
{
	if(pTarget->GetX()>300 || pTarget->GetY()>200)
	{
		pTarget->SetPosition(100,60);
		Sound("Oops");
	}
}

global func FxStayNearCataTimer(pTarget, iEffectNumber)
{
	if(pTarget->GetX()<380 || pTarget->GetY()>340)
	{
		pTarget->SetPosition(500,200);
		Sound("Oops");
	}
}
