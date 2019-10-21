#strict 2

func Initialize() {
	// Always have some music
	Music("Frontend", 1);
	// Gamma
  SetGamma(RGB(10,2,1), RGB(161,128,129), RGB(255,234,221));
  // Crystal
  CreateObject(CRYS,765,550,-1);
  // Game goal
	CreateObject(SCRG);
	// Evaluation dialog options
  SetNextMission("Tutorial.c4f\\Tutorial10.c4s", "$BtnRepeatRound$", "$BtnRepeatRoundDesc$");
	// Start script
  wait(10);
}

func InitializePlayer() {
  RemoveAll(COAL);
  SetTutorialMessagePos(MSG_HCenter | MSG_Top, 0, 30);
  // Less energy for the clonk to prevent crystal snatching
  GetCrew()->SetPhysical("Energy", 5000, PHYS_Temporary);
  GetCrew()->SetCommand(0, "MoveTo", 0, 500, 439);
}

func EmergencyLinekit(int n) {
  // Spieler werfen manchmal ihre Leitungsbausaetze aus der Landschaft.
  if (ObjectCount2(Find_ID(LNKT)) < n) {
    // Manchmal verkaufen sie sie auch
  	if(GetWealth(0) < 10) {
      CreateContents(LNKT,FindObject2(Find_ID(HUT3)));
    }
  }
}

func EmergencyConkit() {
  if(!FindObject2(Find_Or(Find_ID(DRCK),Find_ID(CNKT)))) {
    if(GetWealth(0) < 15) {
      CreateContents(CNKT,FindObject2(Find_ID(HUT3)));
    }
  }
}

func EmergencyOilbarrel() {
  if(!FindObject2(Find_ID(OBRL), Find_Container(FindObject2(Find_ID(POWR))))) {
    CreateContents(OBRL, FindObject2(Find_ID(POWR)));
  }
}

func NoEnergyCommand() {
	var i = 0;
	while (GetCrew()->GetCommand(0,0,i))
	{
		if (GetCrew()->GetCommand(0,0,i) == "Energy")
			{ GetCrew()->SetCommand(0, "None"); GetCrew()->SetComDir(COMD_Stop); return; }
		i++;
	}
}

func ForceEnergyCommand(object pTarget) {
	var i = 0;
	// Has energy command?
	while (GetCrew()->GetCommand(0,0,i))
	{
		if (GetCrew()->GetCommand(0,0,i) == "Energy")
			return;
		i++;
	}
	// No? Set one.
	GetCrew()->SetCommand(0, "Energy", pTarget);
}

static last_message;

func Message(msg)
{
	// Prevent multiplication
	if (msg == last_message) return;
	last_message = msg;
	return TutorialMessage(msg, ...);
}

global func CheckActionTarget2(obj) {
  return GetActionTarget(1) == obj;
}

func Script1() {
  Message("$Txt01$");
  SetArrowToObj(FindObject(CRYS));
  wait(25);
}

func Script2() {
  goto(2);

  // Build derrick
  if (!FindObject2(Find_ID(DRCK))) {
    EmergencyConkit();
    SetArrow(270, 475);
    Message("$Txt02$");
    return;
  }

	// Wait until derrick complete
  if (!FindObject2(Find_ID(DRCK),Find_OCF(OCF_Fullcon))) {
	  RemoveArrow();
	  Message("");
	  return;
	}

	// Start energy line from power plant
  if (!FindObject2(Find_ID(PWRL),Find_ActionTarget(FindObject2(Find_ID(POWR))))) {
    EmergencyLinekit(1);
    NoEnergyCommand();
    if (!FindObject2(Find_ID(LNKT),Find_Container(GetHiRank())))
      { RemoveArrow(); Message("$Txt03$"); wait(12); return; }
    else
      { SetArrowToObj(FindObject(POWR)); Message("$Txt04$"); wait(25); return; }
  }

  // Connect energy line to derrick
  if (!FindObject2(Find_ID(PWRL),Find_Func("CheckActionTarget2",FindObject2(Find_ID(DRCK))))) {
    SetArrowToObj(FindObject(DRCK));
    Message("$Txt05$");
    wait(16);
    return;
  }

  EmergencyOilbarrel();

	// Start drain pipe from derrick
  if (!FindObject2(Find_ID(DPIP),Find_ActionTarget(FindObject2(Find_ID(DRCK))))) {
    EmergencyLinekit(1);
    if (!FindObject2(Find_ID(LNKT),Find_Container(GetHiRank())))
      { RemoveArrow(); Message("$Txt06$"); wait(12); return; }
    else
      { SetArrowToObj(FindObject(DRCK)); Message("$Txt07$"); wait(25); return; }
  }

  // Connect drain pipe to power plant
  if (GetActionTarget(1,FindObject2(Find_ID(DPIP),Find_ActionTarget(FindObject2(Find_ID(DRCK)))))!=FindObject2(Find_ID(POWR))) {
    SetArrowToObj(FindObject(POWR));
    Message("$Txt08$");
    wait(16);
    return;
  }

	// Grab derrick
  if (!FindObject2(Find_ID(CLNK),Find_Action("Push"),Find_ActionTarget(FindObject2(Find_ID(DRCK))))) {
    SetArrowToObj(FindObject(DRCK));
    Message("$Txt09$");
    wait(10);
    return;
  }

	// Drill well to oil
  if (!FindObject2(Find_ID(PIPH),Find_InRect(200,585,500,750-585),Find_NoContainer())) {
  	RemoveArrow();
    Message("$Txt10$");
    wait(15);
    return;
}

	Message("$Txt11$");
	FindObject(POWR)->CreateContents(BARL, 0, 5);

  SetPlrKnowledge(0, PUMP);
  goto(3);
  wait(25);
}

func Script3() {
  goto(3);

  // Build pump
  if (!FindObject2(Find_ID(PUMP),Find_OCF(OCF_Fullcon))) {
    EmergencyConkit();
    SetArrow(550, 425);
    Message("$Txt12$");
    return;
  }

  // Force energy supply for pump
  if (!FindObject2(Find_ID(PWRL),Find_Func("CheckActionTarget2",FindObject2(Find_ID(PUMP))))) {
	  RemoveArrow();
    EmergencyLinekit(1);
    ForceEnergyCommand(FindObject(PUMP));
    Message("");
    return;
  }

	// Start source pipe from pump
  if (!FindObject2(Find_ID(SPIP),Find_ActionTarget(FindObject2(Find_ID(PUMP))))) {
    EmergencyLinekit(1);
    if (!FindObject2(Find_ID(LNKT),Find_Container(GetHiRank())))
      { Message("$Txt06$"); wait(12); return; }
    else
      { Message("$Txt13$"); wait(25); return; }
  }

  // Throw source pipe into lava lake
  if (!FindObject2(Find_ID(LNKT),Find_InRect(720,530,120,35),Find_NoContainer())) {
    SetArrow(755, 515);
    Message("$Txt14$");
    wait(20);
    return;
  }

	// Create drain pipe from pump
  if (!FindObject2(Find_ID(DPIP),Find_ActionTarget(FindObject2(Find_ID(PUMP))))) {
	  RemoveArrow();
    EmergencyLinekit(2);
    if (!FindObject2(Find_ID(LNKT),Find_Container(GetHiRank())))
      { Message("$Txt06$"); wait(12); return; }
    else
      { Message("$Txt15$"); wait(25); return; }
  }

  // Drop drain pipe on the left
  if (!FindObject2(Find_ID(LNKT),Find_InRect(0,0,400,LandscapeHeight()),Find_NoContainer())) {
    SetArrow(223, 490);
    Message("$Txt16$");
    wait(35);
    return;
  }
  RemoveArrow();

  goto(4);
}

func Script4() {
  Message("$Txt17$");
}

func Script5() {
  EmergencyOilbarrel();
  if (GetMaterial(770, 542) == Material("DuroLava")) return(repeat());
  Message("$Txt18$");
}

func Script6() {
	// Crystal sold?
	if (FindObject(CRYS)) return(repeat());
  FindObject(SCRG)->Fulfill();
}
