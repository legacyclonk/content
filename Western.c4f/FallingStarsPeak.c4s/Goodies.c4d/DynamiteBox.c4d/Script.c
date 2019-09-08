/*-- Dynamitkiste --*/

#strict

#include _STY

protected func Initialize()
{
  iFallSpeed = GetFallSpeed() + GetSpeedBoost();
  SetAlive(1);
  var pBallon = CreateObject(BALO);
  SetAction("Ballon", pBallon);
}

public func GetColor() { return(RGB(200,100,0)); }

public func GetPoints() { return(0); }

public func GetTime() { return(0); }

private func Hit()
{
  Sound("CrateCrack");
  Split2Components();
//  RemoveObject();
}

public func ShotHit(iPlr)
{
  if(iPlr<0) return();
  Sound("Blast1");
  CreateParticle("Blast", 0, 0, 0, 0, 200, RGB(255,255,255));
  DoLifes(1, iPlr);
  if(GetActionTarget()) GetActionTarget()->~Hit();
  RemoveObject();
}
