/*-- Bürostuhl --*/

#strict

public func IsBulletTarget() {
  if(!Random(5)) return(-1);
}

protected func Damage() {
  Sparks(5, RGBa(255,255,220,20));
}

// Vertex-Fix
protected func Initialize() { SetAction("Exist"); SetPhase(Random(6)); }

private func CheckVertex() {
  var dir = GetDir()*2-1;
  SetVertex(0, 0, 4*dir);
}

/* Serialisierung */

public func Serialize(array& extra)
{
	extra[GetLength(extra)] = Format("SetPhase(%d)", GetPhase());
}
