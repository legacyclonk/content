/*-- Baumsteuerung --*/

#strict

local MotionThreshold;
local MotionOffset;

/* Überladbare Konstanten */

private func ReproductionAreaSize() { return(400); } // Größe des Bereichs, in dem neue Bäume dieser Art entstehen können
private func ReproductionRate()     { return(500); } // Die Chance, dass in einem Timerintervall eine Vermehrung stattfindet
private func MaxTreeCount()         { return(40); } // Maximale Baumanzahl im Umkreis

private func TreeStrength() { return(150); } // Dicke des Stammes


/* Initialisierung */

protected func Construction()
{
  // Bewegungsschwelle
  MotionThreshold = Random(10);
  // Aktionszeitoffset
  MotionOffset = Random(GetActMapVal("Length", "Breeze"));
  // Verzögerte Initialisierung: Schlägt SetAction fehl, ist es ein alter Baum ohne Animation. Motion-Variablen werden ignoriert.
  if (SetAction("Initialize"))
    {
    // Ausrichtung
    SetDir(Random(2));
    }
  return(0);
}
  
private func Initializing()
{
  // Bis zum Aktionszeitoffset gewartet: los geht's
  if (GetPhase() == MotionOffset) 
    SetAction("Still");
  // Noch warten
  return(1);  
}

/* Bewegung (Wind) */  
  
private func Still()
{
  if (Abs(GetWind()) > 49 + MotionThreshold) SetAction("Breeze");
}
    
private func Breeze()
{
  if (Abs(GetWind()) < 50 + MotionThreshold) SetAction("Still");
}

   
/* Kontext */

public func ContextChop(pClonk)
{
  [$TxtChop$|Image=CXCP|Condition=IsStanding]
  AddCommand(pClonk, "Chop", this());
  return(1);
}

/* Fortpflanzung */

private func Seed()
{
  // Je nach Fortpflanzungsrate mehr oder weniger oft...
  if (!Random(ReproductionRate()))
    // ...reproduzieren
    Reproduction();
}

public func Reproduction()
{
  // Ist noch Platz für einen Baum?
  var iSize = ReproductionAreaSize();
  var iOffset = iSize / -2;
  if (ObjectCount(0, iOffset, iOffset, iSize, iSize, OCF_Chop())<MaxTreeCount())
    // OK, hin damit
    return(PlaceVegetation(GetID(this()), iOffset, iOffset, iSize, iSize, 10) );
  // Kein Platz ;'(
  return(0);
}
    
/* Schaden */    
    
protected func Damage()
{
  // Wenn der Baum schon zu viel Schaden genommen hat...
  if (GetDamage() > TreeStrength() * Max(GetCon(), 30)/100)
    {
    // ...und er noch steht...
    if (GetCategory() & C4D_StaticBack)
      // fällen
      ChopDown(); 
    // Beschädigte, tote Bäume sollten außerdem verfallen
    if (IsDeadTree()) if (!GetEffect("IntTreeDecay", this()))
      if (GetCon() < 90) if (GetCategory() & C4D_Vehicle)
        AddEffect("IntTreeDecay", this(), 1, 51, this());
    }
  return(1);
}

protected func FxIntTreeDecayTimer(object target, int num, int time)
{
  // Zur Sicherheit
  if (!target->~IsDeadTree() || GetCon(target) >= 90 || (GetCategory(target) & C4D_StaticBack))
    return FX_Execute_Kill;
  // Verfall
  DoCon(-1, target);
  return FX_OK;
}

public func ChopDown()
{
  // Bereits gefällt
  if (!IsStanding()) return(0);
  // Kategorie ändern
  SetAction("Idle");
  SetCategory(C4D_Vehicle);
  // Aus der Erde lösen
  while (Stuck() && (++Var(0) < 6))
    SetPosition(GetX(), GetY() - 1, this());
  // Umfallen
  SetRDir(+10); if (Random(2)) SetRDir(-10);
  // Geräusch
  if (GetCon() > 50) Sound("TreeDown*");
  // Fertig
  return(1);
}

/* Status */

public func IsTree() { return(1); }

public func IsStanding() { return(~GetCategory() & C4D_Vehicle); }

public func IsDeadTree() { return false; } // Überladen von toten Bäumen
