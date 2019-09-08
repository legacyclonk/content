#strict

static sailboatX, sailboatY;

Initialize:
	// Have some music
	Music("Pizza Strings", 1);
	MusicLevel(30);
	// Rain
	Sound("Rain", 1, 0, 10, 0, 1);
	// Gamma
  SetGamma(RGB(0,0,0),RGB(100,128,100),RGB(200,255,200));
  // Objects
  CreateObject(CRYS,610,120,-1);
  sailboatX = 540; sailboatY = 322;
  var pSailboat = CreateObject(SLBS, sailboatX, sailboatY, -1);
	AddEffect("FixPosition", pSailboat, 1, 1);
	// Glint
	CreateObject(EGLN);
	// Goal
	CreateObject(SCRG);
	// Start script
  ScriptGo(1);
	// Evaluation dialog options
  SetNextMission("Tutorial.c4f\\Tutorial07.c4s", "$BtnRepeatRound$", "$BtnRepeatRoundDesc$");
  return(1);

InitializePlayer:
	// Set message position
	SetTutorialMessagePos(MSG_HCenter | MSG_Top, 0, 30);
  return(1);

Script1:
	wait(10);
	return(1);

Script2:
	TutorialMessage("$TxtYourGoal$");
	SetArrowToObj(FindObject(CRYS));
	wait(20);
	return(1);

Script3:
	TutorialMessage("");
	RemoveArrow();
	wait(5);
	return(1);

Script4:
	TutorialMessage("$TxtBuildBalloon$");
	SetArrowToObj(FindObject(WRKS));
	wait(20);
	return(1);

Script5:
	TutorialMessage("");
	RemoveArrow();
	wait(5);
	return(1);

Script8:
	TutorialMessage("$TxtMineGold$");
	SetArrow(72, 315);
	wait(30);
	return(1);
	
Script9:
	TutorialMessage("");
	RemoveArrow();
	wait(5);
	return(1);

Script10:
	TutorialMessage("$TxtFlintsBase$");
	SetArrowToObj(FindObject(HUT3));
	wait(20);
	return(1);

Script11:
	TutorialMessage("");
	RemoveArrow();
	wait(5);
	return(1);

Script12:
	TutorialMessage("$TxtGoodLuck$");
	wait(10);
	return(1);

Script13:
	TutorialMessage("");
	return(1);

Script14:
	if (!FindObject(CRYS)->Contained()) return(goto(14));
	return(1);
	
Script15:	
	TutorialMessage("$TxtDigThrough$");
	SetArrowToObj(FindObject(SLBS));
	wait(20);
	return(1);

Script16:
  if (!FindObject2(Find_ID(CLNK), Find_Action("Push"), Find_ActionTarget(FindObject(SLBS)))) return(goto(16));
  return(1);
  
Script17:  
	TutorialMessage("$TxtBackHome$");
	RemoveArrow();	  	
	return(1);	

Script18:
	// Crystal not yet in hut
	var pContainer = Contained(FindObject(CRYS));
	if (pContainer) if (pContainer->Contained()) pContainer = pContainer->Contained();
  if (!pContainer) return(goto(18));
  if (pContainer->GetID() != HUT3) return(goto(18));
  TutorialMessage("$TxtSellCrystal$");
  return(1);
  
Script19:
	// Crystal sold?
	if (FindObject(CRYS)) return(goto(19));
  FindObject(SCRG)->Fulfill();
  SetNextMission("Tutorial.c4f\\Tutorial08.c4s", "$BtnNextTutorial$", "$BtnNextTutorialDesc$");
	return(1);		

global func PXSRemove()
{
	return(1);
}

global func FxFixPositionTimer(pTarget, iEffectNumber)
{
  if (!FindObject2(Find_ID(CLNK), Find_Action("Push"), Find_ActionTarget(pTarget)))
  {
		pTarget->SetPosition(sailboatX, sailboatY);
		pTarget->SetXDir(0);
		pTarget->SetYDir(0);
	}
	else
	{
		sailboatX = pTarget->GetX();
		sailboatY = pTarget->GetY();
	}
}
