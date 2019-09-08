/*-- Schneeblock --*/

#strict

protected func Initialize()
{
  // Position erhöhen, damit der grabende Clonk den Block gleich bekommt
  SetPosition(GetX(), GetY()-1);
}

protected func Hit() {
  if(InLiquid()) return();
  CastPXS("Snow",GetCon()*RandomX(100,160)/100,RandomX(15,25));
  RemoveObject();
}
  
protected func CheckTemp() {
  if (GetTemperature()>0 || InLiquid()) Melt();
}

private func Melt() {
  DoCon(-2);
  CastPXS("Water",4,0);
  SetClrModulation(RGB(196,196,255));
}
