/*-- AFK --*/

#strict

/* Steuerung */

protected func ControlLeft() { return(Remove()); }
protected func ControlRight() { return(Remove()); }
protected func ControlDown() { return(Remove()); }
protected func ControlUp() { return(Remove()); }
protected func ControlThrow() { return(Remove()); }
protected func ControlDig() { return(Remove()); }

protected func ControlCommand() { return(Remove()); }
protected func CrewSelection(bool fDeselect) { if(fDeselect) return(Remove()); }

private func Remove()
{
  var rule = FindObject(NOCA);
  if(!rule)
  {
    SetCursor(GetOwner(), GetCrew(GetOwner()));
    RemoveObject();
    return(1);
  }
  else
  {
    rule->AFK(GetOwner(), false);
    return(1);
  }
}
