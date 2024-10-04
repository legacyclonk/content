/*-- Bürotisch --*/

#strict

public func IsBulletTarget() {
  if(Random(3)) return(-1);
}

protected func Damage() {
  // Splittern
  Sound("WoodHit*");
  // Kaputtgehen
  if(Inside(GetDamage(), 25, 34))
    {
    Sound("CrateCrack");
    RemoveObject();
    }
}

public func IsCraneGrabable() { return(1); }
