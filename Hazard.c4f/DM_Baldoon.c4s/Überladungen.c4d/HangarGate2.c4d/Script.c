/*-- Hangartor --*/

#strict

protected func Initialize()
{
  SetAction("GateStill");
}

public func Open() // Aufmachen
{
  var phase = GetPhase();
  SetAction("GateOpen");
  SetPhase(phase);
  CheckSolidMask();
}

protected func Opened()
{
  SetAction("GateStill");
  SetPhase(11);
  StopSound();
  CheckSolidMask();
}

public func Close() // Zumachen
{
  var phase = 11-GetPhase();
  SetAction("GateClose");
  SetPhase(phase);
  CheckSolidMask();
}

public func Stop() // Sofort stoppen
{
  var phase = GetPhase();
  SetAction("GateStill");
  SetPhase(phase);
  StopSound();
  CheckSolidMask();
}

public func ConsoleControl(int i)
{
  //if(i == 1) return("$Open$");
  if(i == 1) return("$Close$");
  //if(i == 3) return("$Stop$");
}

public func ConsoleControlled(int i)
{
  //if(i == 1 && GetAction() ne "GateOpen") Open();
  if(i == 1 && GetAction() ne "GateClose") Close();
  //if(i == 3 && GetAction() ne "GateStill") Stop();
}

private func StopSound() { Sound("Discharge"); }

private func CheckSolidMask()
{
  SetSolidMask(10*GetPhase(), 0, 10, 60, 0, 0);
}
