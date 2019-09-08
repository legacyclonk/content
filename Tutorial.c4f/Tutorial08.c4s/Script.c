#strict

func Initialize()
{
	// Always have some music
	Music("Frontend", 1);
	MusicLevel(30);
	// Create lens flare
	CreateObject(LENS, 550, 200, NO_OWNER);
	// Bird song
	CreateObject(BRDS); CreateObject(BRDS); CreateObject(BRDS);
	// Grass
	CreateObject(EGRS);
	// Create goal
	CreateObject(SCRG);
	// Start script
  ScriptGo(1);
	// Evaluation dialog options
  SetNextMission("Tutorial.c4f\\Tutorial08.c4s", "$BtnRepeatRound$", "$BtnRepeatRoundDesc$");
}

func InitializePlayer(int plr)
{
	// Find manual start position - never underground!
	var x = 80 + Random(LandscapeWidth() - 160);
	var y = GetHorizonHeight(x);
	var pHut = CreateObject(HUT3, x, y, plr);
	pHut->CreateContents(FLAG);
	pHut->CreateContents(CNKT);
	GetCrew()->SetPosition(x, y - 8);
	CreateObject(LORY, x + 20, y - 10);
	// Set message position
	SetTutorialMessagePos(MSG_HCenter | MSG_Top, 0, 30);
}


Script1:
	wait(10);
	return(1);

Script2:
	TutorialMessage("$TxtWipfies$");
	wait(20);
	return(1);

Script3:
	TutorialMessage("");
	wait(5);
	return(1);

Script4:
	TutorialMessage("$TxtCatchEm$");
	wait(20);
	return(1);

Script5:
	TutorialMessage("");
	wait(5);
	return(1);

Script6:
	TutorialMessage("$TxtSellEm$");
	wait(20);
	return(1);
	
Script7:
	TutorialMessage("");
	RemoveArrow();
	wait(5);
	return(1);


func Script50()
{
  if (FindObject(WIPF)) return(goto(50));
  FindObject(SCRG)->Fulfill();
  SetNextMission("Tutorial.c4f\\Tutorial09.c4s", "$BtnNextTutorial$", "$BtnNextTutorialDesc$");
}

private func GetHorizonHeight(x)
{
  var y;
  while (y < LandscapeHeight() && !GBackSemiSolid(x, y))
    y += 10;
  while (y > LandscapeHeight() && GBackSemiSolid(x, y))
    --y;
  return(y);
}
