/*-- Hangartor --*/

#strict
#include GBDR

public func Initialize() 
{
	inherited(...);
   SetAction("Closed");
   SetMaxDamage(500);
   //by default locked
   Lock();
}

public func OnDmg(int iDmg, int iType)
{
  // diese Tür ist besonders dick
  if(iType == DMG_Projectile)	return(70);
  if(iType == DMG_Melee)	return(80);
  if(iType == DMG_Energy)	return(50);
  if(iType == DMG_Explosion)	return(0);

  return(100);
}

public func OnDestroyed(iType)
{
  Explode(30);
}

public func OnOpen() // Aufmachen
{
  var phase = GetPhase();
	if(GetAction() eq "GateClose") phase = 11-GetPhase();
  SetAction("GateOpen");
  SetPhase(phase);
  CheckSolidMask();
}

protected func Opened()
{
  SetAction("GateStill");
  SetPhase(11);
  closed = false;
  StopSound();
  CheckSolidMask();
}

public func OnClose() // Zumachen
{
  var phase = 11-GetPhase();
  SetAction("GateClose");
  SetPhase(phase);
  closed = false;
  CheckSolidMask();
}

public func Stop() // Sofort stoppen
{
  var phase = GetPhase();
  SetAction("GateStill");
  SetPhase(phase);
  closed = true;
  StopSound();
  CheckSolidMask();
}

public func ConsoleControl(int i)
{
  if(i == 1) return("$Open$");
  if(i == 2) return("$Close$");
  if(i == 3) return("$Stop$");
}

public func ConsoleControlled(int i)
{
  if(i == 1 && GetAction() ne "GateOpen") Open();
  if(i == 2 && GetAction() ne "GateClose") Close();
  if(i == 3 && GetAction() ne "GateStill") Stop();
}

private func StopSound() { Sound("Discharge"); }

private func CheckSolidMask()
{
  SetSolidMask(10*GetPhase(), 0, 10, 60, 0, 0);
}

/* Serialisierung */

public func Serialize(array& extra)
{
	if (GetPhase()) {
		extra[GetLength(extra)] = Format("SetPhase(%d)", GetPhase());
		extra[GetLength(extra)] = "CheckSolidMask()";
	}
}
