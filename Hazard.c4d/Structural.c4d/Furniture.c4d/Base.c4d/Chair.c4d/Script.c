/*-- Stuhl --*/

#strict

public func IsBulletTarget() {
  if(!Random(3)) return(1);
}

public func OnDmg(int iDmg, int iType) {
  if(iType == DMG_Fire) {
    return(-100);
  }
}

public func OnHit(int iDmg, int iType, object pFrom) {
  if(iType == DMG_Fire) {
    Incinerate();
  }
}

protected func Damage() {
  // Splittern
  if(Random(2)) CastObjects(SLNT, 1, 50, RandomX(-18,18), RandomX(-5,1));
  Sound("WoodHit*");
  // Kaputtgehen
  if(Inside(GetDamage(), 14, 19))
    {
    CastObjects(SLNT, 1, 45, RandomX(-18,18), RandomX(-5,1));
    CastObjects(SLNT, 1, 45, RandomX(-18,18), RandomX(-5,1));
    Sound("CrateCrack");
    RemoveObject();
    }
}

// Vertex-Fix
protected func Initialize() { SetAction("Exist"); }

private func CheckVertex() {
  var dir = GetDir()*2-1;
  SetVertex(0, 0, 4*dir);
}

/* Serialisierung */

public func Serialize(array& extra)
{
	if (GetDir() != 0)
		extra[GetLength(extra)] = "SetDir(1)";
}