/*-- Unterwasser --*/

#strict 2

func Initialize()
{
  // Spielziel einrichten
  FindObject(_CRM)->SetMissionAccess("Aquagold");
  return true;
}

static g_flag_created;

func InitializePlayer(int plr)
{
  // Clonks in die Huette
  var start_hut_x = 35,
      start_hut_y = 180;
  var hut3 = FindObject2(Find_ID(HUT3), Find_AtPoint(start_hut_x,start_hut_y));
  var i;
  for (i=0; i<GetCrewCount(plr); ++i)
  {
    var plrcrew = GetCrew(plr, i);
    if (hut3) Enter(hut3, plrcrew); else SetPosition(start_hut_x, start_hut_y, plrcrew);
  }
  // Erster, beitretender Spieler kriegt die Huette und Flagge, sowie ein U-Boot
  if (!g_flag_created)
  {
    for (var baseequip in FindObjects(Find_InRect(0,140, 250,80), Find_Category(C4D_Structure | C4D_Vehicle))) SetOwner(plr, baseequip);
    var flag_carrier = hut3;
    if (!flag_carrier) flag_carrier = GetCursor(plr);
    if (!flag_carrier) flag_carrier = GetCrew(plr);
    if (flag_carrier)
    {
      SetOwner(plr, CreateContents(FLAG, flag_carrier));
    }
    var subm = CreateObject(SUB1, 30*14, 18*14, plr);
    if (subm)
    {
      for (i=0; i<5; ++i) CreateContents(TRP1, subm);
      for (i=0; i<3; ++i) CreateContents(FLNT, subm);
    }
    CreateContents(SFLN, subm);
    g_flag_created = true;
  }
  // Fertig
  return true;
}
