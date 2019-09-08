/*--- Eis ---*/

#strict

protected func CheckTemp()
{
  if (GetTemperature() > 0) Melt();
  return(1);
}  
  
protected func Hit()
{
  Sound("RockHit*");
  return(1);
}
  
private func Melt()
{
  DoCon(-2);
  CastPXS("Water", 4, 0);
  return(1);
}

func IsAlchemContainer() { return(true); }
func AlchemProcessTime() { return(140); }
