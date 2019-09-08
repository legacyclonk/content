/*-- Fischturm --*/

#strict
#appendto FSTW

protected func Bubbling()
{
  // Seepferdchen nachproduzieren
  if(ObjectCount(HICP) < GetPlayerCount() + 2)
    SetCommand(CreateContents(HICP), "Exit");
  return(_inherited());
}
