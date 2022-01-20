/*-- Schleimgranate --*/

#strict

// Ist eine "Granate"
#include GREN

func Initialize() {
  _inherited();
  AddAlienFlareEffect(this(),300);
}

func Hit() { inherited(); HitObject(); }

func HitObject()
{
  for(var i = 7+Random(5), glob ; i ; i--)
    {
    glob = CreateObject(SLST, 0, 0, GetOwner());
    glob ->~ Launch(RandomX(-40,40), RandomX(-40,40), iDamage);
    }
  CastParticles("SlimeGrav", 10, 25, 0, 0, 20, 40, RGBa(0, 240, 0, 10), RGBa(20, 255, 20, 75));
  CastParticles("FrSprk", 30, 5, 0, 0, 70, 130, RGBa(0, 240, 0, 10), RGBa(20, 255, 20, 75));  
  Sound("SlimeHit");
  Sound("Poff");
  RemoveObject();
}

func ExplodeDelay() {   return(180); }
