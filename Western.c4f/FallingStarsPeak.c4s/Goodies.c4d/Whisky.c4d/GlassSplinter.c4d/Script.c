#strict

local alpha;

/* Langsam verschwinden */
protected func FadeMeOut()
{
  SetClrModulation(RGBa(255,255,255,alpha++));
	if(alpha==255) return(RemoveObject());
	return(1);
}

protected func Hit()
{
return(Sound("Glass"));
}