/*-- Glühendes Metall --*/

#strict

protected func Initialize()
{
  SetAction("Glowing");
}

private func GlowingProcess()
{
  // Effekte
  if (!Contained()) 
  {
    Smoke(0, 0, 1);
    CastParticles("PxSpark", 3, 10, 0, 0, 15, 30, RGB(255,223,127), RGB(255,223,127));
  }
  // Abkühlung
  if (GetActTime() > 1800) 
    CoolConvert();
}

private func CoolConvert()
{
  ChangeDef(METL);
}
  
protected func Hit()
{
  Sound("MetalHit*");
}

private func FastCoolOff()
{
  Sound("Pshshsh");
  CoolConvert();
}
 
protected func SellTo() { return(METL); }
