/*-- Fenster --*/

#strict

#strict

local smashed;

public func IsBulletTarget(id id) {
  if(id == HMIS)
    return(0);
  if(!smashed) return(!Random(3));
  else
   return(0);
}

protected func Damage() {
  // Ist schon kaputt
  if(smashed) return();
  // Zersplittern
  Sound("Glass");
  for(var i = 0 ; i < 10 ; i++)
    CastParticles("SplinterGlass", 1, 35, RandomX(-20,20), -5, 20, 20, RGBa(255,255,255,0), RGBa(255,255,255,0));
  // Glas kaputt
  smashed = true;
}
