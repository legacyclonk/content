/*-- Sandsackbarriere --*/

#strict

protected func Incineration()
{
  // Kaputt!
  CastParticles("Sandbag", 10, 100, 0,0, 35, 45, RGBa(228,228,228,0), RGBa(250,250,250,50));
  RemoveObject();
}

/* Serialisierung */

public func Serialize(array& extra)
{
	extra[GetLength(extra)] = Format("SetDir(%d)", GetDir());
}
