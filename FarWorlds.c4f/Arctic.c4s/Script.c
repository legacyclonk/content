/* Arktis */

#strict

/* Initialisierung */

protected func Initialize()
{ // Das erzeugt zusammen mit SkyFade=0,0,0,255,255,255 zwar
  // nicht ganz den Originalen Farbverlauf, aber es ist nahe dran
  SetSkyAdjust(RGBa(220,220,255,99), RGB(153,141,255));
}
