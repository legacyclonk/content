#strict

func Initialize()
{
	// No music
	MusicLevel(0);
	// Create drains
	CreateDrain(140);
	CreateDrain(500);
	// Create goal
	CreateObject(SCRG);
	// Start script
  ScriptGo(1);
	// Evaluation dialog options
  SetNextMission("Tutorial.c4f\\Tutorial09.c4s", "$BtnRepeatRound$", "$BtnRepeatRoundDesc$");
}

func InitializePlayer(int iPlr)
{
  // Verbesserte Physicals (wie Aquaclonk)
  GetCrew(iPlr, 0)->SetPhysical("Swim", 90000, 2);
  GetCrew(iPlr, 0)->SetPhysical("Breath", 250000, 2);
  SetTutorialMessagePos(MSG_HCenter | MSG_Top, 0, 30);
}

func Script1()
{
	wait(10);
}

func Script2() 
{
  TutorialMessage("$TxtBuildIgloo$");
}

func Script3() 
{
  if (!FindBase(0)) return(repeat());
  wait(5);
}

func Script4()
{  
  TutorialMessage("$TxtCatchFish$");
  wait(12);
}

func Script5()
{
	if (GetCrew()->GetAction() ne "Swim") return(repeat());
	wait(5);
}

func Script6()
{	
  TutorialMessage("$TxtBreath$");
  wait(20);
}

func Script7()
{
	if (!GetCrew()->FindContents(FISH)) return(repeat());	
  TutorialMessage("$TxtSellFish$");
  wait(12);
}

func Script8()
{
  if (GetPlrValueGain() < 15) return(repeat());
  wait(5);
}

func Script9()
{
  TutorialMessage("$Txt100Points$");
  wait(20);
}

func Script10()
{
  TutorialMessage("");
}

func Script11()
{	
  if (GetWealth(0) < 100) return(repeat());
  FindObject(SCRG)->Fulfill();
  SetNextMission("Tutorial.c4f\\Tutorial10.c4s", "$BtnNextTutorial$", "$BtnNextTutorialDesc$");
}

func GetHorizonHeight(x)
{
  var y;
  while (y < LandscapeHeight() && !GBackSemiSolid(x, y))
    y += 10;
  while (y > LandscapeHeight() || GBackSemiSolid(x, y))
    --y;
  return(y);
}
    
func CreateDrain(x)
{
	var y = GetHorizonHeight(x);
  if (GBackLiquid(x, y + 1))
    return (CreateObject(DRAI, x, y, -1));
  return(0);
}
