/*-- Perlentauchen --*/

#strict

static base_left_set;
static base_right_set;

protected func Initialize()
{
  // Spielziel setzen
  FindObject(RMOB)->AddType(SHEL, 0);
  FindObject(RMOB)->SetMissionAccess("Glint");
  // Objekte platzieren
  CreateObject(_WRK,420,700,-1);
  // Muscheln
  CreateShell(290, 350);
  CreateShell(270, 540);
  CreateShell(300, 630);
  CreateShell(380, 670);
  CreateShell(430, 670);
  CreateShell(510, 660);
  CreateShell(580, 640);
  CreateShell(650, 580);
  CreateShell(715, 500);
  CreateShell(670, 310);
  return(1);
}

protected func InitializePlayer(int player)
{
  var x;
  var wmil, sawm, base;
  if(player % 2 == 0)
  {
    if(!base_left_set)
    {
      wmil = CreateConstruction(WMIL, 30, 210, player, 100, 1);
      sawm = CreateConstruction(SAWM, 120, 220, player, 100, 1);
      base = CreateConstruction(HUT1, 200, 230, player, 100, 1);
      base_left_set = true;
    }
    else
    {
      x = RandomX(180, 220);
    }
  }
  else
  {
  	if(!base_right_set)
  	{
      wmil = CreateConstruction(WMIL, 960, 210, player, 100, 1);
      sawm = CreateConstruction(SAWM, 830, 220, player, 100, 1);
      base = CreateConstruction(HUT1, 760, 220, player, 100, 1);
      base_right_set = true;
    }
    else
    {
      x = RandomX(740, 780);
    }
	}
  
  if(base)
  {
    base->CreateContents(FLAG);
    base->CreateContents(CNKT);
    base->CreateContents(LNKT);
    CreateObject(PWRL)->SetAction("Connect", wmil, sawm);
    x = base->GetX();
    SetWealth(player, 50);
  }
    
  var y = 100;
  while(!GBackSolid(x, y) && y < 300) ++ y;
  GetCrew(player)->SetPosition(x, y - 10);
}

protected func CreateShell(int x, int y)
{  
  while(GBackSolid(x, y)) -- y;
  while(!GBackSolid(x, y)) ++ y;

  CreateObject(SHEL, x, y+3, NO_OWNER);
}
