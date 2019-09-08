/*-- Windrad --*/

#strict

/* TimerCall */

private func Wind2Turn()
{
  // Zielobjekt verloren
  if (!GetActionTarget())
    return(RemoveObject());
  // Drehgeschwindigkeit
  var turn = BoundBy(GetWind()/20 +5, 1, 9);
  if(!(GetAction() eq Format("Turn%d", turn)))
    SetAction(Format("Turn%d", turn));
  // Energieerzeugung
  DoEnergy(GetActMapVal("Delay", Format("Turn%d", turn))*30, GetActionTarget());
}
