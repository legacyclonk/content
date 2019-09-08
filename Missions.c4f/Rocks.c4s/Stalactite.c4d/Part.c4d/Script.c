/*-- Bruchteil eines Stalaktiten --*/

#strict 2

local alpha;

/* Langsam verschwinden */
func FadeMeOut() {
  SetClrModulation(RGBa(255,255,255,alpha++));
	if(alpha==255) return(RemoveObject());
	return true;
}

func Hit() { return(Sound("RockHit*")); }