/*-- Tisch --*/

#strict

public func IsBulletTarget() {
  return(Inside(Abs(GetR()),80,100)||Inside(Abs(GetR()),260,280));
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

public func ControlDigDouble(object pByObj) {
  SetR(85*(pByObj->GetR()*2 - 1));
  SetPosition(GetX(),GetY()-8);
}

protected func Damage() {
  Sound("WoodHit*");
  // Kaputtgehen
  if(Inside(GetDamage(), 25, 34))
    {
    Sound("CrateCrack");
    RemoveObject();
    }
}

public func IsCraneGrabable() { return(1); }
