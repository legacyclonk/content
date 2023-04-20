/*-- Rauch --*/

#strict


public func Reek() {

  var size = GetCon();
  
  var dirrand = RandomX(-size/15,size/15);
  var xdir = +Sin(GetR()+dirrand,size/4);
  var ydir = -Cos(GetR()+dirrand,size/4);

  CreateParticle("Thrust",0,0,xdir,ydir,RandomX(size*4/5,size),RGBa(255,200,200,60),0,0);

  //CreateParticle("Smoke2",0,0,xdir,ydir,RandomX(size,size*5/4),RGBa(220,200,180,0),0,0);
  Smoke(0,0,size/5);

}