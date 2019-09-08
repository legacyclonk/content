#strict

#include _STY

/*
protected func Initialize()
{
  iFallSpeed = GetFallSpeed() + GetSpeedBoost();
  SetAlive(1);
  var pBallon = CreateObject(BALO);
  SetAction("Ballon", pBallon);
}*/

public func GetColor() { return(RGB(100,200,100)); }

private func Hit()
{
  Split2Components();
}

public func ShotHit(iPlr)
{
  if(iPlr<0) return();
  DoEnergy(100, GetCursor(iPlr));
  Sound("Glass");
//  Hit();
  RemoveObject();
}
