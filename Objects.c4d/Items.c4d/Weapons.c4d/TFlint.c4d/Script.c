/*-- T-Flint --*/

#strict 2

local iLastController; // Fuer Killverfolgung: Letzter Controller vor dem Einsammeln

public func Initialize()
{
  iLastController = GetController();
  return true;
}

public func ExplodeSize() {
  return 20;
}

protected func Hit() {
  Sound("RockHit*");
  if(!ActIdle()) return true;
  Sound("Fuse");
  SetAction("Activated");
  // Picture anpassen
  SetPicture(0, 12 + 64, 64, 64);
  return true;
}

public func Activate()
{
 [$TxtFuse$]
 if(!ActIdle()) return true;
 Sound("Fuse");
 SetAction("Activated");
 iLastController = GetController();
 // Picture anpassen
 SetPicture(0, 12 + 64, 64, 64);
 return true;
}

public func RejectEntrance()
{
  // RejectEntrance: Aufruf passiert bevor der neue Container den Controller setzt
  if(GetAction() == "Activated") iLastController = GetController();
  return false;
}

private func BlowUp() {
  // Bei Explosion in einem Container gilt derjenige als Verursacher, der den Flint vor dem Einsammeln kontrolliert hat
  // Fuer richtige Killverfolgung, wenn ein Spieler den Flint wirft und ein anderer ihn aufsammelt und er dann im Inventar explodiert
  if (Contained() && GetAction() == "TimeOver") SetController(iLastController);
  // Wumm!
  Explode(ExplodeSize());
  return true;
}                   
                    
public func MagicExtinguish() {
  // kann durch Zauber "Löschen" gelöscht werden
  if(GetAction() != "Activated") return false;
  DoExtinguish();
  return true;
}

private func DoExtinguish() {
  Sound("Pshshsh");
  SetAction("Idle");  
  // Picture zurücksetzen
  SetPicture(0, 12, 64, 64);
}

/* Produkteigenschaften */

public func IsChemicalProduct() { return true; }

/* Forschung */

public func GetResearchBase() { return FLNT; }
