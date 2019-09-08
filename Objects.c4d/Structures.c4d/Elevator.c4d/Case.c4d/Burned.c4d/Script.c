/*-- Verbrannter Fahrstuhlkorb --*/

#strict

/* Status */

private func Elevator()
{
  return (FindObject(0, 0, 0, 0, 0, 0, "LiftCase", this()));
}

/* Zerstörung: Reduzieren des Fahrstuhls für Wiederaufbau/Reparatur */

protected func Incineration(iCausedBy) { return(ObjectCall(Elevator(), "RemoveCase")); }
protected func IncinerationEx(iCausedBy) { return(ObjectCall(Elevator(), "RemoveCase")); }
