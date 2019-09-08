/*-- Neuland --*/

#strict

protected func Initialize()
{
  // Spielziele setzen
  FindObject(CROB)->AddType(CST3, 1);
  FindObject(CROB)->AddType(TWR2, 2);
  FindObject(CROB)->AddType(HUT3, 6);
  FindObject(CROB)->SetMissionAccess("NewLand");
  // Einzelne Hütte erzeugen
  CreateConstruction(HUT2, 1070, 330, -1, 100, 1);
  return 1;
}

static g_fInitializedPlayers;

func InitializePlayer(iPlr)
{
	if(g_fInitializedPlayers) return;
	GetCrew(iPlr)->CreateContents(FLAG);
	g_fInitializedPlayers = 1;
}

func TACC_BlockFirstAllianceAdd() { return 1; }
