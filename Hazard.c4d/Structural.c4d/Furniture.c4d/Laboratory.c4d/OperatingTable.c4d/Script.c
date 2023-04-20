/*-- OP-Tisch --*/

#strict

#include BLGH

protected func Initialize()
{
  SetAction("Exist");
  SetDir(Random(2));
  
  pLight = AddLightCone(100, RGBa(255,255,220,70), this());
  pLight->ChangeSizeXY(700,400);
  pLight->ChangeOffset(-8,-29,true);
  pLight->ChangeR(180);
  TurnOff();
}
