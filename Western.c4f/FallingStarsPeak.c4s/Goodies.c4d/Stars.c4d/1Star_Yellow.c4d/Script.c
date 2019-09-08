/*-- Gelber Stern --*/

#strict

local iFallSpeed;

public func GetColor() { return(RGB(255,255)); }

public func GetPoints() { return(1); }

public func GetTime() { return(3); }

public func GetSpeedBoost() { return(0); }

protected func Initialize()
{
  iFallSpeed = GetFallSpeed() + GetSpeedBoost();
  SetAlive(1);	
}

protected func Hit() 
{
  CastParticles("FSpark", 10, 15, 0, 0, 40, 50, GetColor(), GetColor());	
  RemoveObject();	
}

public func ShotHit(iPlr)
{
  if(iPlr<0) return();
  DoTime(GetTime(), iPlr);
  DoPoints(GetPoints(), iPlr);
  if(Random(100)<=5) CreateObject(MBAG);
  Sound("Crystal*");
  if(GetEffect("Sheriff", GetCursor(iPlr))) { CastObjects(SPRK,10,20,0,0); DoPoints(GetPoints(), iPlr);}
  Hit();
}
local iAngle;
protected func Timer()
{
  CreateParticle("MSpark", RandomX(-5,5), 0, 0, 0, 50, GetColor());
  SetYDir(iFallSpeed);
  iAngle+=45;
  SetXDir(Sin(iAngle, 10));
  SetRDir(5);
}
