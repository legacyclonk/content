/*-- Torpedowerfer(angeschraubt) --*/

#strict
#include CTW2

protected func RotationSpeed() { return(4); }
protected func CannonMobileID() { return(CTW8); }

public func ComFire(object pByObj)
{
  // Noch beim Nachladen
  if ((GetAction() S= "Reload1") || (GetAction() S= "Reload2"))
    return(Sound("Click"));  
  // Ggf. Rotation beenden
  SetAction("Attaching",GetActionTarget());
  SetRDir(0);
  // Torpedo suchen
  var obj = FindContents(TRP1, GetActionTarget()); // Sollte später noch mit IsTorpedo gelöst werden...
  if (!obj)
    return(Sound("Click"));  
  // Auswerfen und abfeuern
  Exit(obj, 0, 3);
  obj->Launch(0, GetR());
  SetPlrView(GetOwner(pByObj), obj);
  Sound("LaunchTorpedo");
  // Torpedo soll hinter dem Geschütz erscheinen
  SetCategory(2, obj); SetObjectOrder(obj);
  SetCategory(2, this()); GetActionTarget()->SetObjectOrder(this());
  AddEffect("TorpedoLaunch", obj, 1, 15, this());
  // Nachladen
  SetAction("Reload1");
}

public func FxTorpedoLaunchStop(pTorpedo)
{
  // Dem abgefeuerten Torpedo wieder seine ursprüngliche Kategorie verpassen
  SetCategory(3600, pTorpedo);
}

public func ComLeft(object pClonk)
{
  // Nachladen soll nicht durch Drehung abgebrochen werden können
  if ((GetAction() S= "Reload1") || (GetAction() S= "Reload2"))
    return(Sound("Click"));  
  // Steuerung
  return(_inherited(pClonk));
}

public func ComRight(object pClonk)
{
  // Nachladen soll nicht durch Drehung abgebrochen werden können
  if ((GetAction() S= "Reload1") || (GetAction() S= "Reload2"))
    return(Sound("Click"));  
  // Steuerung
  return(_inherited(pClonk));
}

public func ComStop(object pClonk)
{
  // Nachladen soll nicht durch Drehung abgebrochen werden können
  if ((GetAction() S= "Reload1") || (GetAction() S= "Reload2"))
    return(Sound("Click"));  
  // Steuerung
  return(_inherited(pClonk));
}

private func StartReloading()
{
  Sound("Airlock1");  
}

private func EndReloading()
{
  Sound("Airlock2");  
}