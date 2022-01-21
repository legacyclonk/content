#strict

local shooter;
local explo;
local nospeed;

/* Bullet */

func Construction(object byObj) {
  // nichts? :C
  if(!byObj)
    return();
  // Waffe?
  shooter = GetShooter(byObj);
	
  SetExplo(30);
}

public func Launch(int xdir, int ydir)
{
  AddEffect("HitCheck", this(), 1, 1, 0, SHT1,shooter,true);
  SetSpeed(xdir, ydir);
  SetAction("Travel");
}

public func SetExplo(int ex) {
  explo = ex;
}

public func Hit() {
  Explode(explo);
}

public func HitObject(obj) { Hit(); }

func Traveling () {
  SetR(Angle(0,0,GetXDir(),GetYDir()));
  
  // wie, liegt das rum?! kaputtmachen
  if(!GetXDir() && !GetYDir()) {
	if(!nospeed) {
		nospeed = true;
	}
	else {
		Hit();
	}
  } else {
	nospeed = false;
  }

  CreateParticle("Smoke2", 0, 0, RandomX(-15, 15), -5,
                           RandomX(80, 160), RGBa(100,100,100,60)); 
}