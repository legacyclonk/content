/*-- Banner (eingerollt) --*/

#strict

protected func Initialize()
{
  UpdateColor();
}  

/* Auspacken */

public func Activate(pClonk)
{
  [$MnuUnpack$]
  Sound("TentPack");
  CreateConstruction(BANR, 0, +12, GetOwner(pClonk), 100)->Unpacked();
  RemoveObject();
  return(1);
}

/* Aufschlag */

protected func Hit()
{
  Sound("TentPackHit");
  return(1);
}

/* Kann auf dem Amboss hergestellt werden */

public func IsAnvilProduct() { return(1); }

/* Farbe setzen */

protected func UpdateTransferZone()
{
  // Aktualisierung für alte Szenarien
  SetClrModulation(GetColorDw());
  return(1);
}

protected func OnOwnerChanged() 
{ 
  return(UpdateColor()); 
}

private func UpdateColor()
{
  // Farbe nach Besitzer setzen
  var dwClr = GetPlrColorDw(GetOwner());
  SetClrModulation(dwClr);
  SetColorDw(dwClr);
  // Fertig
  return(1);
}
