#strict 2

/*-- Schatzsuche --*/

func Initialize()
{
  // Spielziel setzen
  FindObject(RMOB)->AddType(_SKL, 0);
  FindObject(RMOB)->SetMissionAccess("SkullGone");
  // Schädel erzeugen
  CreateObject(_SKL,  62, 519, -1);
  CreateObject(_SKL, 444, 792, -1);
  CreateObject(_SKL, 512, 999, -1);
  return 1;
}

static g_fInitializedPlayers;

func InitializePlayer(iPlr)
{
	if(g_fInitializedPlayers)
	{
	  // Aber Rejoiner brauchen eine Flagge!
	  var flag = FindObject2(Find_ID(FLAG));
	  if (!flag) GetCrew(iPlr)->CreateContents(FLAG);
	  else if (GetOwner(flag) == NO_OWNER) flag->SetOwner(iPlr);
	  return;
	}
	CreateConstruction(WMIL, 529, 238, iPlr, 100, 1);
	CreateConstruction(WRKS, 479, 238, iPlr, 100, 1);
	CreateConstruction(HUT1, 567, 238, iPlr, 100, 1)->CreateContents(FLAG);
	SetWealth(iPlr, 120);
	g_fInitializedPlayers = 1;
}

func TACC_BlockFirstAllianceAdd() { return 1; }
