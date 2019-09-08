/*-- In der Höhe --*/

#strict

static hut_placed;

static homebase_material;
static knowledge;

protected func Initialize()
{
  // Spielziel eirichten
  FindObject(SCRG)->SetMessages("$MsgGoalFulfilled$", "$MsgGoalUnfulfilled$");
  FindObject(SCRG)->SetMissionAccess("TheClouds");
  CreateObject(_STA, 230, 2922);
  // Statuenteile erzeugen
  SetR(Random(360),CreateObject(_PA1,75,2460,-1));
  SetR(Random(360),CreateObject(_PA2,30,2065,-1));
  SetR(Random(360),CreateObject(_PA3,45,1815,-1));
  SetR(Random(360),CreateObject(_PB4,285,1605,-1));
  SetR(Random(360),CreateObject(_PA5,175,555,-1));
  SetR(Random(360),CreateObject(_PA6,95,110,-1));
  homebase_material = [];
  knowledge = [];
  return(1);
}

protected func InitializePlayer(int player)
{
  if(!hut_placed)
  {
    var crew = GetCrew(player);
    var hut = CreateConstruction(HUT2, 280, 2930, player, 100, 1);
    hut->CreateContents(FLAG);
    hut->CreateContents(LOAM, 0, 8);
    hut_placed = true;
    DoWealth(player, 30);
  }

  for(var entry in homebase_material)
    DoHomebaseMaterial(player, entry[0], entry[1]);
  for(var entry in knowledge)
    SetPlrKnowledge(player, entry);
}

global func AddKnowledge(id def)
{
  knowledge[GetLength(knowledge)] = def;
  for(var i = 0; i < GetPlayerCount(); ++ i)
    SetPlrKnowledge(GetPlayerByIndex(i), def);
}

global func AddHomebaseMaterial(id material, int amount)
{
  homebase_material[GetLength(homebase_material)] = [material, amount];
  for(var i = 0; i < GetPlayerCount(); ++ i)
    DoHomebaseMaterial(GetPlayerByIndex(i), material, amount);
}

func TACC_BlockFirstAllianceAdd() { return 1; }
