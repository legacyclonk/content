/*-- Zugbrücke (links) --*/

#strict

/* Konfiguration */

public func GetTowerType() { return(CPT4); }

private func FindTower() { return(FindObject(GetTowerType(), 50,0)); }

/* Initialisierung */

protected func Initialize()
{
  SetAction("Raised");
}

public func IsAttachedTo(object pTarget)
{	
	return (pTarget == FindTower());
}		

/* Status */

public func IsOpen() { return(GetAction() S= "Lowered"); }
public func IsOpening() { return(GetAction() S= "Lower"); }
public func IsClosed() { return(GetAction() S= "Raised"); }
public func IsClosing() { return(GetAction() S= "Raise"); }

public func IsDown() { return(IsOpen() || IsOpening()); }
public func IsUp() { return(IsClosed() || IsClosing()); }

/* Burgteilfunktionen */

public func CastleChange()
{
	return(DestructCheck());
}	

private func DestructCheck()
{
  if (!FindTower()) 
  	RemoveObject();  
  return(1);
}

/* Steuerung */

public func ControlUp()
{
  if (GetAction() S= "Lowered")
    return(SetAction("Raise"));
  if (GetAction() S= "Lower")
  {
  	var iPhase = GetPhase();
  	SetAction("Raise");
  	SetPhase(20 - iPhase);
  	return(1);
	}    
  return(0);
}
  
public func ControlDown()
{
  if (GetAction() S= "Raised")
    return(SetAction("Lower"));
  if (GetAction() S= "Raise")
  {
  	var iPhase = GetPhase();
  	SetAction("Lower");
  	SetPhase(20 - iPhase);
  	return(1);
	}    
  return(0);  
}
  
/* Anschlag */

protected func Hit()
{
	// Geräusch
  Sound("DrawbridgeHit");
  // Transferzone des Turms ändert sich
  var pTower = FindTower();
  if (pTower) pTower->UpdateTransferZone();
}
  
private func UpdateSolidMask()
{
  if (GetAction() S= "Lower")
    SetSolidMask(GetPhase() * 60, 60, 60, 60, 0, 0);
  if (GetAction() S= "Raise")
    SetSolidMask(1200 - GetPhase() * 60, 60, 60, 60, 0, 0);
  if (GetAction() S= "Lowered")
    SetSolidMask(1200, 60, 60, 60, 0, 0);
  if (GetAction() S= "Raised")
    SetSolidMask(0, 60, 60, 60, 0, 0);
}  