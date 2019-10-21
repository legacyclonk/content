#strict 2

// Gespeicherte Positionen für den Relaunch
static iBlnX, iBlnY, iPlrX, iPlrY, iCounter;

func Initialize()
{
	// Always have some music
	Music("Frontend", 1);
	// Create lens flare
	CreateObject(LENS, 100, 100, NO_OWNER);
	// Fix vines (wrong offset when saved in Objects.txt for the first time)
	var pVine;
	for (pVine in FindObjects(Find_ID(VINE)))
		pVine->SetPosition(pVine->GetX(), pVine->GetY() - 60);
	// Create flag-checker effect
	AddEffect("CheckForFlag", 0, 1, 10);
	// Create goal
	CreateObject(SCRG);
	// Start the script
	ScriptGo(true);
	// Evaluation dialog options
  SetNextMission("Tutorial.c4f\\Tutorial02.c4s", "$BtnRepeatRound$", "$BtnRepeatRoundDesc$");
}

func CreateMaterial(int iPlr)
{
  CreateObject(FLAG,590,285,iPlr);
  CreateObject(LOAM,500,295,iPlr);
  CreateObject(LOAM,510,295,iPlr);
  CreateObject(LOAM,515,295,iPlr);
  CreateObject(LOAM,515,295,iPlr);  
}

func InitializePlayer(int iPlr)
{
	// Message positioning
	SetPlrShowControlPos(iPlr, SHOWCTRLPOS_TopLeft);
	SetTutorialMessagePos(MSG_Top | MSG_Left | MSG_WidthRel | MSG_XRel, 50, 50, 30);
	
  SetPlrShowControl(0,"___345678_   345678 __________");  
  
  // Positionen speichern
  iBlnX = FindObject(BALN)->GetX(); iBlnY = FindObject(BALN)->GetY();
  iPlrX = FindObject(CLNK)->GetX(); iPlrY = FindObject(CLNK)->GetY();
  iCounter = 0;
  
  CreateMaterial(iPlr);
}

func Script1()
{
  SetPlrShowControl(0,"___345678_   345678 _______7__");  
  TutorialMessage("$TxtGrabthehotairballoon$");
  wait(15);
}
func Script2()
{
  if (GetHiRank(0)->GetAction() != "Push") return(goto(2));
  if (GetHiRank(0)->GetActionTarget()->GetID()!=BALN) return(goto(2));
  SetPlrShowControl(0,"___345678_   345678 ____5__7__");  
  TutorialMessage("$TxtNowfloatupwiththebal$");
}
func Script3()
{
  if (!FindObject(0,450,250,260,70,OCF_Living)) return(goto(3));
  if (GetHiRank(0)->GetAction() != "Push") return(goto(5));
  SetPlrShowControl(0,"___345678_   345678 _______7__");  
  TutorialMessage("$TxtLetgooftheballoonwit$");
  wait(20);
}
func Script4()
{
  if (GetHiRank(0)->GetAction() == "Push") return(goto(4));
}
func Script5()
{
  SetPlrShowControl(0,"___345678_   345678 __________");  
  TutorialMessage("$TxtTheskyislandinthecen$");
  // Positionen speichern
  iBlnX = FindObject(BALN)->GetX(); iBlnY = FindObject(BALN)->GetY();
  iPlrX = FindObject(CLNK)->GetX(); iPlrY = FindObject(CLNK)->GetY();
  iCounter = 20;
}
	
func Script20()
{
  if(iCounter < 20)
  {
    // Positionen speichern
    iBlnX = FindObject(BALN)->GetX(); iBlnY = FindObject(BALN)->GetY();
    iPlrX = FindObject(CLNK)->GetX(); iPlrY = FindObject(CLNK)->GetY();
    iCounter = 20;
  }
  SetPlrShowControl(0,"___345678_   345678 __________");  
  if(GetHiRank(0)->Contents(0))
  {
    if (GetHiRank(0)->Contents(0)->GetID()==FLAG) return(goto(30));
    if (GetHiRank(0)->Contents(0)->GetID()==LOAM) return(goto(40));
  }
  TutorialMessage("$TxtPickuponeoftheloamch$");
  wait(7);
  if (!FindObject(_AR1))
  	SetArrowToObj(FindObject(LOAM));
}

func Script21()
{	
  if(GetHiRank(0)->Contents(0))
  {
    if (GetHiRank(0)->Contents(0)->GetID()==FLAG) return(goto(30));
    if (GetHiRank(0)->Contents(0)->GetID()==LOAM) return(goto(40));
  }
  goto(21);
}

func Script30()
{
  TutorialMessage("$TxtPleasedroptheflagfor$");
  SetPlrShowControl(0,"___345678_   345678 ___3______");  
  wait(15);
}
func Script31()
{
  if (GetID(Contents(0,GetHiRank(0)))==FLAG) return(goto(31));
  goto(20);
}
func Script40()
{
  SetPlrShowControl(0,"___345678_   345678 ______67__");  
  TutorialMessage("$TxtNowmovetotheveryleft$");
  wait(15);
  SetArrow(465,295);
}
func Script41()
{
  if (!FindObject(0,460,280,30,30,OCF_Living)) return(goto(41));
  SetPlrShowControl(0,"___345678_   345678 _____5____");
  TutorialMessage("$TxtHitdigtwicequicklyto$");
  wait(15);
  RemoveArrow();
}
func Script42()
{
  if (GetHiRank(0)->GetMenu()==2) return(goto(60));
  if (GetHiRank(0)->GetMenu()==LMMS) return(goto(60));
  if (GetHiRank(0)->GetAction() == "Dig") return(goto(50));
  return(goto(42));
}
func Script50()
{
  Sound("Error");
  SetPlrShowControl(0,"___345678_   345678 _______7__");
  TutorialMessage("$TxtThatwasnotquickenoug$");
  wait(15);
}
func Script51()
{
  if (GetHiRank(0)->GetAction()=="Dig") return(goto(51));
}
func Script52()
{
  SetPlrShowControl(0,"___345678_   345678 _____5____");
  TutorialMessage("$TxtClickdigtwicemorequi$");
  wait(20);
  return(goto(42));
}
func Script60()
{
  Sound("Ding");
  SetPlrShowControl(0,"___345678_   345678 ___3__6___");
  TutorialMessage("$TxtChoosethebridgediago$");
}
func Script61()
{
  if (GetHiRank(0)->GetAction() == "Bridge") return(goto(70));
  if (!GetHiRank(0)->GetMenu()) return(goto(20));
  return(goto(61));
}
func Script70()
{
  SetPlrShowControl(0,"___345678_   345678 __________");
}
func Script80()
{
  TutorialMessage("$TxtInthiswaybuildthreed$");
}
func Script81()
{
  if (!FindObject(0,360,240,85,50,OCF_Living)) return(goto(81));
}

func Script82()
{
	iCounter = 82;  	
  TutorialMessage("$TxtOnceyouarecloseenoug$");
}
func Script83()
{
  if (!GetHiRank()->Contained()) return(goto(83));
  if (!GetHiRank(0)->FindContents(FLAG)) return(goto(83));
  SetPlrShowControl(0,"___345678_   345678 ___3______");
  TutorialMessage("$TxtUsecommandthrowtoput$");
  wait(15);
}
func Script84()
{
  if (FindObject(HUT3)->GetBase()) return(goto(84));
  Sound("Applause");
  TutorialMessage("$TxtExcellent$");
  FindObject(SCRG)->Fulfill();
  SetNextMission("Tutorial.c4f\\Tutorial03.c4s", "$BtnNextTutorial$", "$BtnNextTutorialDesc$");
  return(1);
}

func RelaunchPlayer(iPlr)
{
	// Kommentar
	Sound("Oops");
	// Altes Material entfernen und neu erzeugen
  RemoveAll(FLAG); RemoveAll(LOAM); RemoveAll(BALN);
  CreateMaterial(iPlr);
  CreateObject(BALN, iBlnX, iBlnY, iPlr);
  // Neuen Clonk erzeugen, falls nötig
  if (!FindObject(CLNK)->GetAlive())
 	{
	  var pClonk = CreateObject(CLNK, iPlrX, iPlrY, iPlr);
	  MakeCrewMember(pClonk, iPlr);
	  SetCursor(iPlr, pClonk);
	}
  // Scriptcounter zurück setzen
  goto(iCounter);
}

global func FxCheckForFlagTimer(pTarget, iEffectNumber) {
	// Lost the flag?
	if (!ObjectCount(FLAG))
		// Relaunch!
		GameCall("RelaunchPlayer", GetPlayerByIndex());
	return(1);
}
