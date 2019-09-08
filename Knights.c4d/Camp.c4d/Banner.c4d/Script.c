/*--- Banner ---*/

#strict

protected func Initialize()
{
  SetAction("Fly");
  Wind2Fly();
}

/* TimerCall */

private func Wind2Fly()
{
  SetDir(BoundBy(7 + GetWind() / 10, 0, 13));
}

/* Einpacken */

public func ControlDigDouble()
{
  [$MnuPack$]
  CreateObject(BANP, 0, +20, GetOwner());
  RemoveObject();
  Sound("TentPack");
  return(1);
}

/* Farbe setzen */

protected func OnOwnerChanged() 
{ 
  return(UpdateColor()); 
}

public func Unpacked()
{
  return(UpdateColor()); 
}  

public func UpdateColor()
{
  // Farbmodulation nach Besitzer setzen
  SetClrModulation(GetPlrColorDw(GetOwner()));
  // Fertig
  return(1);
}
