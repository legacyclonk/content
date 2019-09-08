/*-- Iglu --*/

#strict

private func MaxOccupant() { return(2); }

/* Eingangssteuerung */

protected func ActivateEntrance(pByObj) {
  // Raus geht's immer
  if (Contained(pByObj)==this())
    return(Exit(pByObj,-20,14));
  // Weitere Clonks können nur rein, bis die Maximalzahl erreicht ist
  if (GetOCF(pByObj)&OCF_CrewMember())
    if (IsOccupied())
      return(0,Message("$Full$",this()));
  // Sonst Objekt aufnehmen
  return(Enter(this(),pByObj));
}

/* Status */

public func IsOccupied() {
  if ((ObjectCount(0,0,0,0,0,OCF_CrewMember(),0,0,this())+1)>MaxOccupant())
    return(1);
}

/* TimerCall */

protected func Activity() { 
  // Es ist wohl zu warm
  if (GetTemperature()>10) Melt();
  // Zeit als minimale Baustelle berechnen
  if (!GetComponent(SWBL)) Local(9)++;
  // Baustelle errichtet, aber nicht weitergebaut: abbrechen
  if (Local(9)>35) RemoveObject();  
  return(1);
}

private func Melt() {
  DoCon(-2);
  CastPXS("Water",8,0);
}

/* Wärme */

public func GetWarmth() {
  // Normales Iglu
  return(50);
}