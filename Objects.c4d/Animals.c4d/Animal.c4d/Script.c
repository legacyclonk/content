/*-- Tiervermehrung --*/

#strict

// Status
public func IsAnimal() { return true; }

// Bevölkerungskontrolle
private func ReproductionAreaSize() { return(800); }   // Größe des Bereichs, in dem neue Tiere dieser Art entstehen können
private func ReproductionRate()     { return(4000); }  // Die Chance, dass in einem Timerintervall eine Vermehrung stattfindet
private func MaxAnimalCount()       { return(10); }    // Maximale Tieranzahl im Umkreis

// Spezielles Vermehren (z.b. Monster mit Ei)
private func SpecialRepr()
{
}

// Spezielle Anforderung (z.b. Fisch mit Schwimm-Action)
private func SpecialReprodCond()
{
  return(1);
}

// Tiere Zählen
private func CountMe()
{
  var ReprodSize = ReproductionAreaSize();
  var ReprodSizeHalb = ReprodSize  / -2;
  return(ObjectCount(GetID(this()), ReprodSizeHalb, ReprodSizeHalb, ReprodSize , ReprodSize , OCF_Alive()));
}

/* Vermehrung */

public func Reproduction(bool fRepr)
{
  // Nicht mehr am Leben
  if (!GetAlive()) return(0);
  // Noch nicht ausgewachsen
  if (GetCon() < 100) return(0);
//  if(FindObject(REPR) || !fRepr) return();
  // Spezielle Anforderung nicht erfüllt
  if (!SpecialReprodCond()) return(0);
  // Schon zu viele Tiere dieser Art
//  Log("%d/%d",  ObjectCount(GetID())+1, GetComponent(GetID(), 0, FindObject(REPR)));
  if(!FindObject(REPR)) { if (CountMe() >= MaxAnimalCount()) return(0); }
  else if(ObjectCount(GetID())+1 >= GetComponent(GetID(), 0, FindObject(REPR))) return(0);
  // Reproduktion
  if (!SpecialRepr())
  {
    // Normale Reproduktion
    var pChild = CreateConstruction(GetID(this()), 0, 0, -1, 40);
    pChild->~Birth();
  }
  // Erfolg
  return(1);
}

/* Geburt */

public func Birth()
{
  SetAction("Walk");
  if (Random(2)) SetComDir(COMD_Left());
  else SetComDir(COMD_Right());
  return(1);
}

/* Einsammeln von Tieren */

local fForceEnter;

// Einsammeln erzwingen
public func ForceEnter(object pContainer)
{
  fForceEnter = 1;
  Enter(pContainer);
  fForceEnter = 0;
  return(1);
}

protected func RejectEntrance(object pContainer)
  {
  // Übergabe (z.B. Clonk->Lore) immer OK
  if (Contained()) return();
  // Tot auch OK
  if (!GetAlive()) return();
  // Einsammeln erzwingen OK
  if (fForceEnter) return();
  // Ansonsten abhängig von globaler Einstellung - definiert in Spielregel
  return !ANIM_IsCollectible(pContainer);
  }
