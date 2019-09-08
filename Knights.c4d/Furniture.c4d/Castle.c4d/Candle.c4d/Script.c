/*-- Candle --*/

#strict

protected func Initialize()
{
  SetCategory(C4D_Vehicle());
  SetAction("Off");
}

protected func ControlDigDouble()
{
  if(GetAction() S= "On") return(SetOff());
  SetAction("On");
}

public func SetOff()
{
  SetAction("Off");
  Sound("Pshshsh");
  return(1);
}
