/*-- Schwarzer Stern --*/

#strict

#include _STY

protected func Hit() 
{
  for( var i=5; i>0; --i)
    {
    CreateParticle("Fire", RandomX(-2,2), RandomX(-2,2), Sin(Random(360), RandomX(50,65))+RandomX(-2,2), Cos(Random(360), RandomX(50,65))+RandomX(-2,2), Random(55)+20, RGBa(Random(255),0,0,90));
    CreateParticle("Fire2", RandomX(-2,2), RandomX(-2,2), Sin(Random(360), RandomX(50,65))+RandomX(-2,2), Cos(Random(360), RandomX(50,65))+RandomX(-2,2), Random(55)+20);
    }
  CastObjects(DFLM, RandomX(1, 2), RandomX(20,30));	
  RemoveObject();	
}

protected func Timer()
{
  for( var i=5; i>0; --i)
  {
    CreateParticle("Fire2", RandomX(-5,5), RandomX(-2,2), RandomX(-2,2), 0, 50);
    CreateParticle("Fire", RandomX(-5,5), RandomX(-2,2), RandomX(-2,2), 0, 50);
  }  
  SetYDir(iFallSpeed);
  iAngle+=45;
  SetXDir(Sin(iAngle, 10));
  SetRDir(5);
}

public func GetPoints() { return(0); }

public func GetTime() { return(0); }

public func GetSpeedBoost() { return(15); }
