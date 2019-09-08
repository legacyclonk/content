/*-- Muschel --*/

#strict

protected func Hit()
{
  Sound("MetalHit*");
  return(1);
}

func IsAlchemContainer() { return(true); }
func AlchemProcessTime() { return(50); }
