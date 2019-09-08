/*-- Flüssig-Granit --*/

#strict

protected func Hit()
{
  CastPXS("Granite", 150, 30);
  RemoveObject();
  return(1);
}