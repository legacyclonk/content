/*-- Money --*/

#strict

public func IsBonus() { return(true); }

public func Color() { return(RGB(255,255,255)); }

public func Activate(object obj) {
  Sound("Money", 1, 0,0, GetOwner(obj)+1);
  var plr = GetController(obj);
  //ka-ching!
  SetWealth(plr,GetWealth(plr)+100);
  
  //Effekt! :O
  var x = GetX(obj), y = GetY(obj);
  var p = "NoGravSpark", s = 35, c = RGB(255,255,0);
  CreateParticle(p,x+10,y-13,0,0,s,c,obj);
  CreateParticle(p,x+5,y-15,0,0,s,c,obj);
  CreateParticle(p,0,  y-15,0,0,s,c,obj);
  CreateParticle(p,x-5,y-13,0,0,s,c,obj);
  CreateParticle(p,x-10,y-9,0,0,s,c,obj);
  CreateParticle(p,x-9,y-4,0,0,s,c,obj);
  CreateParticle(p,x-3,y-1,0,0,s,c,obj);
  
  CreateParticle(p,x+3,y+1,0,0,s,c,obj);
  CreateParticle(p,x+9,y+4,0,0,s,c,obj);
  CreateParticle(p,x+10,y+9,0,0,s,c,obj);
  CreateParticle(p,x+5,y+13,0,0,s,c,obj);
  CreateParticle(p,x,  y+15,0,0,s,c,obj);
  CreateParticle(p,x-5,y+15,0,0,s,c,obj);
  CreateParticle(p,x-10,y+13,0,0,s,c,obj);
  
  for(var j; j <= 8; j++)
    CreateParticle(p,x,y-20+5*j,0,0,s,c,obj);
  return(true);
}
