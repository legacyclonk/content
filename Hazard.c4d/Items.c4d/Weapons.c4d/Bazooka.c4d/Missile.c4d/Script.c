/* Rakete */

#strict

local iSpeed, iDamage, exploding;
local shooter;

public func Acceleration() { return(5); }
public func MaxTime() { return(60); }
public func MaxSpeed() { return(180); }

func Construction(object byObj) {
  // nichts? :C
  if(!byObj)
    return();
  // Waffe?
  shooter = GetShooter(byObj);
}

public func Launch(int iAngle, int iDmg, int speed)
{
  if(!speed) speed = 20;
  iSpeed = speed;
  iDamage = iDmg;
  if(!iDamage) iDamage = 35;

  SetR(+iAngle);
  SetXDir(+Sin(iAngle,iSpeed));
  SetYDir(-Cos(iAngle,iSpeed));
  SetAction("Travel");

  AddLight(100,RGB(255,127,0),this(),GLOW);

  AddEffect("HitCheck", this(), 1,1, 0, SHT1,shooter,true);
}

/* Timer */

private func Accelerate() { 
  if(iSpeed < MaxSpeed())
    iSpeed += Acceleration();

  SetXDir(+Sin(GetR(),iSpeed));
  SetYDir(-Cos(GetR(),iSpeed));
}

private func Smoking() {

  var dist = Distance(0,0,GetXDir(),GetYDir());
  var maxx = +Sin(GetR(),dist/10);
  var maxy = -Cos(GetR(),dist/10);
  var ptrdist = 50;

  for(var i=0; i<dist; i+=ptrdist) {

    var x = -maxx*i/dist;
    var y = -maxy*i/dist;


    var rand = RandomX(-30,30);
    var xdir = +Sin(GetR()+rand,20);
    var ydir = -Cos(GetR()+rand,20);

    CreateParticle("Thrust",x,y,GetXDir()/2,GetYDir()/2,RandomX(80,100),RGBa(255,200,200,60),0,0);
    CreateParticle("Smoke2",x,y,xdir,ydir,RandomX(100,120),RGBa(220,200,180,0),0,0);
  }
}

private func Traveling()
{
  // kommt nich weiter
  if(GetActTime() >= MaxTime()) return(Hit());

  // beschleunigen
  Accelerate();

  // Effekte
  Smoking();
}
  
/* Treffer */

private func Hit() {
  HitObject();
}

private func HitObject(object pObject) {
  exploding = true;
  BlastObjects(GetX(), GetY(), iDamage/2);
  BlastObjects(GetX(), GetY(), iDamage/2);
  Explode(iDamage,0,0,0,1);
}

public func Damage() {
  if(GetDamage() > 20 && !exploding)
    Hit();
}

public func EMPShock() {
  SetAction("Idle");
}

public func IsBulletTarget() { return(true); }
public func NoDecoDamage() { return(true); }
public func IsMachine() { return(true); }

public func OnDmg(int iDmg, int iType) {
	if(iType == DMG_Bio)		return(100);
	if(iType == DMG_Fire)		return(100);
}