/*-- Schockwellenaufsatz(angeschraubt) --*/

local iEnergy;

#strict
#include CTW2

protected func RotationSpeed() { return(4); }
protected func CannonMobileID() { return(CT10); }
protected func MaxEnergy() { return(300); }
private func EnergyPerShot() { return(100); }       // Energiebedarf für einen Schuss

public func ComStopDouble()
{
  // Turm braucht keine Energie mehr fordern
  GetActionTarget()->RequireEnergy(false);
  // Sonstiges Abbauen erledigen
  return(inherited());
}

/* TimerCall */

private func EnergyTransfer()
{
  // Energie vom Turm abzwacken
  if(GetEnergy(GetActionTarget()) > 9) DrainEnergy(GetActionTarget(), 10);
}

private func DrainEnergy(object pTower, int iLevel) // Energie vom Turm abziehen
{
  // Energiebegrenzung
  iLevel = Min(iLevel, MaxEnergy() - iEnergy);

  DoEnergy(-iLevel, pTower);
  iEnergy += iLevel;
  
  CheckEnergyUsage();
}

public func ComEnter(object pObj)                   // kommt in den Turm
{
  // Batterie
  if(GetID(pObj) == ADM3 && iEnergy < MaxEnergy() ) 
  {
    iEnergy+=300;
    Sound("LightningLoad");
    RemoveObject(pObj);
    CheckEnergyUsage();
  }
}

public func ComLeft()
{
  if(GetAction() S= "Reload") return(1);
  return(inherited() );
}

public func ComRight()
{
  if(GetAction() S= "Reload") return(1);
  return(inherited() );
}

public func ComStop()
{
  if(GetAction() S= "Reload") return(1);
  return(inherited() );
}

public func ComFire(object pClonk)                   // schießen
{
  if(GetAction() S= "Reload") return(Sound("Click") );
  if(iEnergy < EnergyPerShot()) return(Sound("Click") );

  // babumm!
  Sound("LightningCannon");
  CreateObject(SKWV,Sin(GetR(),30),-Cos(GetR(),30),GetOwner())->Start(GetR());
  
  // Energie abziehen
  iEnergy -= EnergyPerShot();
  
  // Nachladen
  SetRDir();
  SetAction("Reload");

  CheckEnergyUsage();
 
  return(1);
}

private func CheckEnergyUsage()
{
  // Nicht voll aufgeladen?
  if(iEnergy < MaxEnergy() )
    // Turm fordert noch keine Energie?
    if(!GetActionTarget()->IsRequiringEnergy() )
      // Energie her, aber zackig!
      GetActionTarget()->RequireEnergy(true);
  
  // Voll aufgeladen?
  if(iEnergy >= MaxEnergy() )
    // Turm fordert noch Energie?
    if(GetActionTarget()->IsRequiringEnergy() )
      // Er kann damit aufhoeren, wir brauchen keine mehr
      GetActionTarget()->RequireEnergy(false);
}

