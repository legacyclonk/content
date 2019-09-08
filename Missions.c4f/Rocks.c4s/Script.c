/*-- Donnerberge --*/

#strict 2

func Initialize()
{
  ScriptGo(1);
  // Parallax sky
  SetSkyParallax(0,20,0);
  // Spielziel einrichten
  FindObject2(Find_ID(OREM))->SetMissionAccess("ComeDown");
  return(1);
}

static g_flag_created;

func InitializePlayer(int plr)
{
  // Clonks in die Huette
  var start_cst_x = 260,
      start_cst_y = 800;
  var cst = FindObject2(Find_ID(CST2), Find_AtPoint(start_cst_x,start_cst_y));
  var i;
  for (i=0; i<GetCrewCount(plr); ++i)
  {
    var plrcrew = GetCrew(plr, i);
    if (cst) Enter(cst, plrcrew); else SetPosition(start_cst_x, start_cst_y, plrcrew);
  }
  // Erster, beitretender Spieler kriegt die Basis und Flagge
  if (!g_flag_created)
  {
    for (var baseequip in FindObjects(Find_InRect(0,700, 500,130), Find_Category(C4D_Structure | C4D_Vehicle))) SetOwner(plr, baseequip);
    var flag_carrier = cst;
    if (!flag_carrier) flag_carrier = GetCursor(plr);
    if (!flag_carrier) flag_carrier = GetCrew(plr);
    if (flag_carrier)
    {
      SetOwner(plr, CreateContents(FLAG, flag_carrier));
    }
    g_flag_created = true;
  }
  // Fertig
  return true;
}


func Script150()
{
  // Steinschlag nur im rechten Teil der Landschaft erzeugen
  var bigrock = CreateObject(RCK2, 1800+Random(300), -40-Random(100), -1);
  if (bigrock) bigrock->SetXDir(-20+Random(25));
  // Zufaellige Menge; ansonsten variierende Verzoegerung bis zum naechsten Steinschlag
  if (Random(4)) goto(150); else goto(Random(150));
  return true;
}


func CleanupScen()
{
  for (var sysobj in FindObjects(Find_Category(C4D_Goal | C4D_Rule | C4D_Environment))) RemoveObject(sysobj);
}