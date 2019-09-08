/*-- Toter Drache --*/

#strict

/* Initialisierung */

protected func Initialize()
{
  // Rote Farbe
  SetClrModulation(RGB(100,40,0));
  SetAction("Dead");
}

/* Verwesung */

protected func Decaying()
{
  DoCon(-2);
}