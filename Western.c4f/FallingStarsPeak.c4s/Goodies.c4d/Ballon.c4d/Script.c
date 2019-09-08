/*-- Ballon --*/

#strict

#include _STY

public func GetColor() { return(RGB(200,100,0)); }

public func GetPoints() { return(0); }

public func GetTime() { return(0); }

private func Hit()
{
  var obj = FindObject(0, 0, 0, 0, 0, 0, "Ballon", this());	
  if(obj) obj->~Hit();	
  DoHit();
}

private func DoHit()
{
  Sound("Poff");
  CastParticles("FSpark", 30, 28, 0, 0, 70, 80, RGB(255,255,255),RGB(255,255,255));
  RemoveObject();
}

public func ShotHit(iPlr)
{
  if(iPlr<0) return();
  return(DoHit());
}

protected func Timer()
{
  if(Contained()) return(RemoveObject());
  CreateParticle("MSpark", RandomX(-5,5), 0, 0, 0, 50, GetColor());
  SetYDir(iFallSpeed);
  SetRDir(5);
}
