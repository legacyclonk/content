/*-- Tiefsee-Geschützturm --*/

#strict
#include DOOR
#include CTW0

local require_energy;

/* Lokal angepasste Sounds */

protected func SoundOpenDoor()
{
  Sound("Airlock1");
}
  
protected func SoundCloseDoor()
{
  Sound("Airlock2");
  SetIdleAction();
}

/* ~#exclude des fundaments */

protected func Construction() {}
protected func Destruction() {}

/* Callbacks */

public func IsDeepSeaResearch() { return(1); }

public func GetResearchBase() { return(WRK2); }

/* Für DOOR */

private func ActIdle()
{
  return(inherited() || GetAction() eq "RequireEnergy");
}

/* Kein Energieverbrauch */

protected func Initialize()
{
  RequireEnergy(false);
}

/* Geschuetze koennen Energieverbrauch aktivieren */

public func RequireEnergy(bool require)
{
  require_energy = require;
  
  // Idle?
  if(ActIdle() ) SetIdleAction();
}

private func SetIdleAction()
{
  // Aktion anpassen
  if(require_energy)
    SetAction("RequireEnergy");
  else
    SetAction("Idle");
}

/* Gibt zurueck, ob der Turm gerade Energie fordert */

public func IsRequiringEnergy()
{
  return(require_energy);
}

/* Abschrauben des Geschützes */

public func ReleaseCannon()
{
  // Energiebedarf zurücksetzen  
  RequireEnergy(false);
  EnergyCheck(0);
  return(_inherited());
}
