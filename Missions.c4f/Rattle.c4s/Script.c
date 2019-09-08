/*-- Rattlesnake Canyon --*/

#strict 

func Initialize()
{
  // Flatten village area for train operation
  DigFreeRect(1600,300,300,10);
  // Create initial bridge segement
  CreateObject(BRDG,1595,320,-1);
  // Create train
  ObjectSetAction( CreateObject(STMG,1800,309,-1),
                   "Push",
                   CreateObject(WAGN,1830,309,0) );
  // Parallax sky
  SetSkyParallax(0,20,10);
  // Mission password
  FindObject(GLDM)->SetMissionAccess("RattledOut");
  // Done
  return(1);
}

static g_fInitializedPlayers;

func InitializePlayer(int iPlr)
{
  // Der erste Spieler bekommt das Startvermögen und ein Brückensegment in die Werkstatt. :)
  if (GetPlayerCount() == 1 && !g_fInitializedPlayers)
  {
    SetWealth(iPlr, 100);
    var pHut = CreateConstruction(HUT2, 2036, 310, iPlr, 100, 1);
    pHut->CreateContents(FLAG);
    for(var i = 0; i < 8; i++)
      pHut->CreateContents(WOOD);
    var pWindMill = CreateConstruction(WMIL, 1992, 310, iPlr, 100, 1);
    var pWorkshop = CreateConstruction(WRKS, 2102, 310, iPlr, 100, 1);
    pWindMill->CreateObject(PWRL)->SetAction("Connect", pWindMill, pWorkshop);
    CreateConstruction(FNDR, 1944, 310, iPlr, 100, 1);
    Enter(pWorkshop, CreateObject(BRDG));
    g_fInitializedPlayers = 1;
  }
  return(1); 
}

func TACC_BlockFirstAllianceAdd() { return 1; }
