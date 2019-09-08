/* Krone */

#strict 2

protected func Initialize()
{
  // Blinkaktivität setzen
  SetAction("Glitter0");
  return(1);
}

// Auftreffsound
protected func Hit() { Sound("Crystal*"); }

// Verlust
protected func Destruction() { GameCallEx("OnCrownLost", this); }

// Aktivität
protected func Glittering()
{
	// Steckt fest?
	if (Stuck()) SetPosition(GetX(), GetY()-1);
  SetAction(Format("Glitter%d", Random(5)));
}

// Einsammelbar?
protected func RejectEntrance(object new_container)
{
  // Aus anderen Containern heraus (feindliche Clonks, ...) kein Problem
  if (Contained()) return false;
  // Keine Tricks mit Loren, etc.
  if (!(GetOCF(new_container) & OCF_CrewMember)) return true;
  // Nur von gegnerischen Clonks
  return !Hostile(GetOwner(), GetOwner(new_container));
}
