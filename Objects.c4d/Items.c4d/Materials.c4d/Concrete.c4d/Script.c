/*-- Beton --*/

#strict

protected func Hit()
{
  CastPXS("Rock",100,30);
  RemoveObject();
  return(1);
}