/*-- Himmelsdorf --*/

#strict 2

protected func Initialize()
{
  // Spielziele setzen
  RemoveAll(CROB);
  var goal = CreateObject(CROB, 50,50,-1);
  if (goal)
  {
    goal->AddType(CST2, 1);
    goal->AddType(BLMP, 1);
    goal->SetMissionAccess("Speckles");
  }
  return true;
}

protected func InitializePlayer(int plr)
{
  // In die Basis, wenn noch vorhanden
  var base = FindObject(HUT3);
  var crew = GetCrew(plr);
  if (base)
  {
    Enter(base, crew);
    SetOwner(plr, base);
  }
  else
  {
    SetPosition(656, 500, crew);
  }
  // Nur eine Flagge pro Spiel; aber wenn man sie verloren hat, gibts eine neue.
  if (!ObjectCount(FLAG))
  {
    if (!base) CreateContents(FLAG, crew); else CreateContents(FLAG, base);
  }
  return true;
}

func TACC_BlockFirstAllianceAdd() { return 1; }
