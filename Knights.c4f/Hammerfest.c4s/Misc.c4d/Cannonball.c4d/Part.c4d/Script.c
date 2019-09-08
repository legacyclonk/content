/*-- Bruchteil einer Kanonenkugel --*/

#strict

local alpha;

func Initialize() {
  SetAction("Exist");
  SetPhase(Random(4));
}

/* Langsam verschwinden */
func FadeMeOut() {
  SetClrModulation(RGBa(255,255,255,alpha++));
	if(alpha==255) return(RemoveObject());
	return(1);
}

func Hit() { return(Sound("RockHit*")); }
