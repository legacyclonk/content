/*-- Fell --*/

#strict

local szGraphic;

protected func Hit() { Sound("WoodHit*"); }

public func SetSkinGraphics(szName)
{
  szGraphic = szName;
  SetGraphics(szGraphic);
}

func IsAlchemContainer() { return(true); }
func AlchemProcessTime() { return(100); }
