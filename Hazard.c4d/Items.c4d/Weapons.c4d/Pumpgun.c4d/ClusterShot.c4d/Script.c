#strict
#include SHT1

public func BulletStrike(object pObj)
{
  // nope,... stattdessen...
	// |
	// v
}

private func HitObject(object pObject) {
  Explode(iDamage,0,0,0,1);
}

public func Remove() {
  if(pTrail) {
    pTrail->SetPosition(GetX(),GetY());
    pTrail->Remove();
  }

  Explode(iDamage,0,0,0,1);
}