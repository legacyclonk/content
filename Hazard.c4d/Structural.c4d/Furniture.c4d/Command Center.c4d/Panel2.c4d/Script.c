/*-- Panel --*/

#strict

local broken;

protected func Damage()
{
  if(GetDamage() > 20 && !broken)
  {
    SetAction("Broken");
    Sparkle(20);
    Sound("Electrify");
    Schedule("CastObjects(SPRK, 1, 30);", 100, 10, this());
    broken = true;
  }
}
