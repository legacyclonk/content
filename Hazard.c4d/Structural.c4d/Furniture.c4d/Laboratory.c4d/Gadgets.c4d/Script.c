/*-- Apparatur --*/

#strict

protected func Initialize() {
	// Ein zufällig Aussehen
	Set(Random(GetActions())+1);
	return(1);
}

public func Set(int i)
{
	i = BoundBy(i,1,GetActions());
	Call(Format("Gadget%d", i));
}

// Anzahl verschiedener Daseinsformen
private func GetActions() { return(3); }

/* Für jedes Aussehen setzen wir andere Werte */

protected func Gadget1() {
  SetAction("1");
  SetVertexXY(0, -4, 7);
  SetVertexXY(1,  4, 7);
}

protected func Gadget2() {
  SetAction("2");
  SetVertexXY(0, -9, 9);
  SetVertexXY(1,  9, 9);
}

protected func Gadget3() {
  SetAction("3");
  SetVertexXY(0, -9, 8);
  SetVertexXY(1,  9, 8);
}

/* Treffer! */

public func IsBulletTarget(id def) {
  if(def->~NoDecoDamage()) return();
  return(1);
}

protected func Damage() {
  // Viel Glas!
  CastParticles("SplinterGlass", 5+Random(5), 50, 0,0, 20,25, RGBa(255,255,255), RGBa(255,255,255));
  // Flüssigkeiten
  if(GetAction() ne "3")
  {
    if(Random(2)) CastPXS("Acid", 5+Random(10), 50);
    if(Random(2)) CastPXS("DuroLava", 5+Random(10), 50);
  }
  // Rauch
  CreateSmokeTrail(RandomX(15,20), Random(360), 0,0, this());
  CreateSmokeTrail(RandomX(15,20), Random(360), 0,0, this());
  // Sound
  Sound("Glass");
  // Explosion oder Verschwinden
  if(Random(2)) Explode(5+Random(5));
  else RemoveObject();
}

/* Serialisierung */

public func Serialize(array& extra)
{
	extra[GetLength(extra)] = Format("Set(%s)", GetAction());
}
