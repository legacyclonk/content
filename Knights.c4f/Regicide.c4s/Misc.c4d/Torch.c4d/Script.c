/*-- Fackel --*/

#strict

public func Activate()
{
  if(GetAction() S= "Burning") return(Extinguishsh());
  SetAction("Burning");
  return(this() );
}
  
private func Extinguishsh()
{
  SetAction("Idle");
  Sound("Pshshsh");
  return(1);
}

