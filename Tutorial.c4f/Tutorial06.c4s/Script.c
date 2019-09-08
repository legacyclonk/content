#strict

Initialize:
	// No music
	Music(0);
	return(1);

InitializePlayer:
  RemoveObject(Contents(0,CreateConstruction(POWR,390,150,0,100,1)));
  CreateObject(CRYS,20,140,-1);
  SetGamma(RGB(0,0,0),RGB(100,100,100),RGB(200,200,200));
  ScriptGo(1);
	// Set message position
	SetTutorialMessagePos(MSG_Bottom | MSG_Left | MSG_XRel | MSG_WidthRel, 10, -30, 35);
	// Evaluation dialog options
  SetNextMission("Tutorial.c4f\\Tutorial06.c4s", "$BtnRepeatRound$", "$BtnRepeatRoundDesc$");
  return(1);

Script1:
	wait(10);
	return(1);
Script2:
  SetPlrShowControlPos(0,1);
  SetPlrShowControl(0,"___345678_   345678 __________");
  TutorialMessage("$TxtWithyourfirstclonkco$");
  SetArrowToObj(FindObject(CRYS));
  return(1);
Script3:
  if (!Contained(FindObject(CRYS))) return(goto(3));
  TutorialMessage("");
  RemoveArrow();
  LaunchEarthquake(60,140);
  ShakeFree(60,160,50);
  return(1);
Script10:
	TutorialMessage("$TxtNotSafe$");
	return(1);
Script23:
	TutorialMessage("");
	return(1);	  
Script25:
  FindObject(HUT3)->CreateContents(LNKT);
  FindObject(HUT3)->CreateContents(METL, 0, 2);
  FindObject(HUT3)->CreateContents(WOOD, 0, 4);
  FindObject(HUT3)->CreateContents(CNKT);
  SetPlrShowControl(0,"012345678_012345678 __________");
  TutorialMessage("$TxtWiththeotherclonkact$");
  SetArrow(333, 127);
  return(1);
Script26:
	// Elevator is being built
  if (!FindObject(ELEV)) return(goto(26));
  TutorialMessage("");
  RemoveArrow();
	return(1);
Script31:
	// Elevator complete?
  if (!FindObject(ELEC)) return(goto(31));
  TutorialMessage("$TxtElevPowerSupply$");
  wait(12);
  return(1);
Script32:
	// Powerline complete
  if (FindObject(PWRL)) return(goto(33));
  // Find working clonk
	var pClnk = FindObject(0, 0,0,LandscapeWidth(),160, OCF_CrewMember);
	if (!pClnk) return(ScriptGo(0)); // Failure: second clonk fell down as well...
  // No powerline, but clonk has lost his commands
  if (!pClnk->GetCommand())
  	// Force energy supply command
  	pClnk->SetCommand(0, "Energy", FindObject(ELEV));
  return(goto(32));
Script33:
	wait(5);
  return(1);
Script34:
	// Trigger elevator
	FindObject(ELEC)->ControlDown();
  TutorialMessage("$TxtNowdigoutafewchunkso$");
  SetArrow(466, 130);
  wait(5);
  return(1);
Script35:
	// First coal
	if (ObjectCount(COAL) < 1) return(goto(35));
	RemoveArrow();
	return(1);
Script36:
	// Not enough coal yet
	if (ObjectCount(COAL) < 3) return(goto(36));
	return(1);
Script37:
	TutorialMessage("$TxtThrowTheCoalInto$");
	SetArrow(406, 111);
	return(1);
Script38:
	// Powerplant working?
	if (!(FindObject(POWR)->GetAction() eq "Burning")) return(goto(38));
	RemoveArrow();
	wait(10);
	return(1);
Script39:
  TutorialMessage("$TxtOncetheelevatorhaspo$");
  return(1);
Script40:
	// Clonk at bottom?
	if (!FindObject(0, 280,300,200,100, OCF_CrewMember)) return(goto(40));
	TutorialMessage("$TxtThinkOfAWay$");
	wait(30);
	return(1);
Script41:
	TutorialMessage("");
	return(1);	
Script50:
	// Crystal not yet in hut
	var pContainer = Contained(FindObject(CRYS));
	if (pContainer) if (pContainer->Contained()) pContainer = pContainer->Contained();
  if (!pContainer) return(goto(50));
  if (pContainer->GetID() != HUT3) return(goto(50));
  TutorialMessage("$TxtActivatethesellmenuu$");
  return(1);
Script51:
	// Crystal sold?
	if (FindObject(CRYS)) return(goto(51));
  FindObject(SCRG)->Fulfill();
  SetNextMission("Tutorial.c4f\\Tutorial07.c4s", "$BtnNextTutorial$", "$BtnNextTutorialDesc$");
	return(1);		
	