/*-- Heal --*/

#strict

public func IsBonus() { return(true); }

public func Color() { return(RGB(255,0,0)); }

public func Activate(object obj) {
  Sound("Heal", 1, 0,0, GetOwner(obj)+1);
  obj->DoEnergy(100);
  //ein Partikelkreuz! :D
  var j,size=35, p = "NoGravSpark";
  var x=obj->GetX(),y=obj->GetY();
  for(j=1; j <= 4; j++) {
    CreateParticle(p,x+5*j,y+5,0,0,size,RGB(255,0,0),obj,0);
    CreateParticle(p,x+5*j,y-5,0,0,size,RGB(255,0,0),obj,0);
  }
  CreateParticle(p,x+20,y,0,0,size,RGB(255,0,0),obj,0);
  
  for(j=1; j <= 4; j++) {
    CreateParticle(p,x-5*j,y+5,0,0,size,RGB(255,0,0),obj,0);
    CreateParticle(p,x-5*j,y-5,0,0,size,RGB(255,0,0),obj,0);
  }
  CreateParticle(p,x-20,y,0,0,size,RGB(255,0,0),obj,0);
  
  for(j=1; j <= 4; j++) {
    CreateParticle(p,x+5,y+5*j,0,0,size,RGB(255,0,0),obj,0);
    CreateParticle(p,x-5,y+5*j,0,0,size,RGB(255,0,0),obj,0);
  }
  CreateParticle(p,x,y+20,0,0,size,RGB(255,0,0),obj,0);
  
  for(j=1; j <= 4; j++) {
    CreateParticle(p,x+5,y-5*j,0,0,size,RGB(255,0,0),obj,0);
    CreateParticle(p,x-5,y-5*j,0,0,size,RGB(255,0,0),obj,0);
  }
  CreateParticle(p,x,y-20,0,0,size,RGB(255,0,0),obj,0);
  
  return(true);
}
