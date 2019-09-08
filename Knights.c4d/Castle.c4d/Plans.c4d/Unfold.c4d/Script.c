/*-- Burgbaupläne (Ansicht) --*/

#strict

protected func Initialize()
{
  SetOnlyVisibleToOwner(1);
  SetClrModulation(RGBa(255, 255, 255, 127));
  return(1);
}

public func Activate()
{
  DoCon(-90);
  SetAction("Zoom");
  return(1);        
}
  
private func Expand()
{
  DoCon(+10);
  SetPosition(GetX(), GetY() - 1);
  if (GetCon() >= 100) SetAction("Show");
  return(1);  
}
  
private func Hide()
{
  RemoveObject();
  return(1);
}