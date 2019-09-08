#strict

local iTimer;

protected func CheckLost()
  {
  if (++iTimer > 50) return(Disappear());
  if (ActIdle() || !GetAlive(GetActionTarget())) 
    RemoveObject();
  return(1);
  }

public func ShieldFactor() { return (50); }
  
public func Unbuckle()
  {
  // Abnehmen nicht möglich
  return();
  }

public func Disappear()
  {
  Sparkle(5);
  return(RemoveObject());
  }

public func Activate(pForClonk)
  {
  // Alten Schild verlängern?
  var pPrevShield;
  if (pPrevShield = FindObject(_SHM, 0,0,0,0, 0, "Shield", pForClonk))
    return(pPrevShield->IncreaseDuration(), RemoveObject());
  // Neu setzen
  return(SetAction("Shield", pForClonk));
  }

public func IncreaseDuration()
  {
  // Schildzeit verlängern
  iTimer -= 60;
  }
