/*-- Werkstatt --*/

/* Initialisierung */

#strict
#include DOOR
#include WRKS

/* Produkteigenschaften (überladbar) */
private func ProductType() { return(C4D_Vehicle() | C4D_Object()); }
private func ProductCondition() { return(); }

/* Türsteuerung */

private func SoundOpenDoor()  { Sound("SteelGate1"); }
private func SoundCloseDoor() { Sound("SteelGate1"); }

/* ~#exclude des fundaments */

protected func Construction() {}
protected func Destruction() {}

/* Aktivität */

private func Building() { if (!Random(10)) Sound("Build*"); }

private func Bubbling() { if(!Random(2)) return(); Bubble(RandomX(-11,49),13); Bubble(RandomX(-11,49),20); }

/* Callbacks */

public func IsDeepSeaResearch() { return(1); }

public func GetResearchBase() { return(RSU1); }
