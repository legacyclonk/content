/*-- Schneeball --*/

#strict
#include ICE1

protected func Construction() {
  // Entfernen, wenn keine Schneeballregel und freigegrabener Schneeball
  if (GetID() == SNWB && !Contained() && !FindObject(RSNB, 0, 0, -1, -1))
    RemoveObject();
}

protected func Hit() {
  CastPXS("Snow", 100, 20);
  RemoveObject();
  return(1);
}

protected func Departure(thrower) {
  var proc = GetProcedure(thrower);
  var comd = GetComDir(thrower);
  // Ablegen -> Abbruch
  if (GetPlrDownDouble(GetOwner(thrower)))
    // Nur Ablegen in der Luft mit Befehlsrichtung? Luftwurf erlauben!
    if (proc ne "FLIGHT" || GetComDir(thrower) == COMD_None)
      return(0);
  
  // Ablegen im stehenden Hangeln -> Abbruch
  if (proc eq "HANGLE" && comd == COMD_None)
    return(0);
  
  // Ablegen im Schwimmen / Klettern -> Abbruch
  if (proc eq "SWIM" || proc eq "SCALE")
    return(0);
  
  // Wurfrichtung rausfinden
  var dir;
  // Nach Befehlsrichtung, wenn sinnvolle Werte vorhanden
  if (comd == COMD_Left() || comd == COMD_Right()) {
    if (comd == COMD_Left())  dir = -1;
    if (comd == COMD_Right()) dir = +1;
  }
  // sonst nach Blickrichtung
  else {
    if (GetDir(thrower) == DIR_Left()) dir = -1;
    else dir = +1;
  }

  // Wurfgeschwindigkeiten berechnen
  var x_dir = dir * GetPhysical("Throw", 0, thrower)/1000 + GetXDir(thrower) / 3;
  var y_dir = -30;

  if (!x_dir) return(0);
  
  // Position anpassen
  SetPosition(GetX() + x_dir / 5, GetY() - 2 + GetYDir(thrower) / 2);

  // Geschwindigkeit setzen
  SetXDir(x_dir);
  SetYDir(y_dir);
}

func IsAlchemContainer() { return(true); }
func AlchemProcessTime() { return(100); }
