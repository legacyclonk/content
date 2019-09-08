/*-- Steintor --*/

#strict

local iSwitchDir;

protected func Initialize()
  {
  SetAction("Gate");
  SetComDir(iSwitchDir=COMD_Down);
  }

public func ControlUp(object pController)
  {
  [$DescUp$]
  // 2do: sound
  if (GetComDir() != COMD_Up) Sound("Elevator");
  // Schalter schalten dauerhaft; der Benutzer nur für max. 30 Frames
  if (pController && pController->~IsSwitch()) 
    iSwitchDir = COMD_Up;
  else
    Schedule("SetComDir(iSwitchDir)", 30);
  return(SetComDir(COMD_Up));
  }

public func ControlDown(object pController)
  {
  [$DescDown$]
  // 2do: sound
  if (GetComDir() != COMD_Down) Sound("Elevator");
  // Schalter schalten dauerhaft; der Benutzer nur für max. 30 Frames
  if (pController && pController->~IsSwitch()) 
    iSwitchDir = COMD_Down;
  else
    Schedule("SetComDir(iSwitchDir)", 30);
  return(SetComDir(COMD_Down));
  }

protected func Hit()
  {
  Sound("Discharge");
  // 2do: sound
  }
