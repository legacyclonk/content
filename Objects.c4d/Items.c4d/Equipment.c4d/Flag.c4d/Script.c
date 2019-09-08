/*-- Flagge --*/

#strict

private func Flying()
{
  SetDir(BoundBy(GetWind()/10+4,0,8));
  if (!GetActTime())
  {
    SetCategory(31 & GetCategory(GetActionTarget()));
    SetObjectOrder (GetActionTarget());
  }
  return(1);
}

func ResetCategory()
{
  SetCategory(2576);
  return 1;
}

func OnOwnerChanged(int new_owner, int old_owner)
{
  // Flagge hat den Besitzer verloren? Dann runterfallen!
  if (new_owner == NO_OWNER) SetAction("Idle");
}