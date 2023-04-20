/*-- Glasröhre --*/

#strict

local smashed;

protected func Initialize()
{
  SetAction("Full");
  SetDir(Random(2));
}

public func Damage()
{
  if(GetDamage() > 25 && !smashed)
  {
    //Sound machen
    Sound("Glass");
    SetAction("Break");
    for(var i = -15; i < 20; i += 2)
    {
      if(Random(2)) CastParticles("SplinterGlass", 2, 35, RandomX(-10,10), i, 20, 20, RGBa(255,255,255,0), RGBa(255,255,255,0));
      if(Random(2)) CastParticles("SplinterGlass", 2, 35, RandomX(-10,10), i, 20, 20, RGBa(255,255,255,0), RGBa(255,255,255,0));
      if(!Random(5))  CastPXS("DuroLava", 5+ Random(5), 20, 0, 0);
    }
    
    smashed = true;
  }
}

public func IsBulletTarget(id def) {
  if(smashed) return();
  if(def) if(def->~NoDecoDamage()) return();
  return(!Random(20));
}
