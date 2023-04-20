/*-- Hintergrundlicht --*/

#strict
#include BLGH

func CreateLight()
{
  pLight = AddLightCone(1000,RGBa(255,255,255,20),this());
  pLight->ChangeSize(750);
  pLight->ChangeR(180);
}

protected func Glowing() {
  CreateParticle("PSpark", RandomX(-2,2), 4, RandomX(-1,1), RandomX(1,3), 10+Random(15), RGB(50,50,25), this(), true);
}
