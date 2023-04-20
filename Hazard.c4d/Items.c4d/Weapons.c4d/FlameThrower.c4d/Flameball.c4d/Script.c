/*-- Flammenfeuerball der Verdammnis!! --*/

#strict

#include FFLM

public func MaxLifeTime() { return 75; }

protected func Flying() {
  SetYDir(GetYDir(0,1000)+GetGravity()*2,0,1000);

  CreateParticle("Smoke2", GetXDir()/6, GetYDir()/6, RandomX(-15, 15), RandomX(-5,-20),
                       RandomX(120, 160), RGBa(150,150,150,80)); 
	
	_inherited();
}

