/*-- Gastank --*/

#strict

protected func Initialize()
{
  // Standardfärbung
  SetColorDw(RGB(100,255,100));
}

protected func Incineration()
{
  /* Mächtiger Badabumm! */

  // TIM1 nicht geladen
  if(!GetCategory(0, TIM1)) return(InstaExplode());
  // TIM1 ist geladen. Also kleine Bomben verteilen!
  for(var i=0, tim ; i < 7 ; i++)
    {
    tim = CreateObject(TIM1, RandomX(-80, 80), RandomX(-20,20), -1);
    Schedule(Format("Explode()", 20+Random(20)), 25 + Random(25), 0, tim);
    }
  // Und explodieren!
  return(InstaExplode());
}

private func InstaExplode()
{
  /* Viel Effekt */
  CastParticles("Wave", 7+Random(5), RandomX(35,55), 0,0, 30,60, RGBa(0,0,200,50), RGBa(20,20,255,100));
  CastParticles("Glas", 15+Random(5), 100, 0,0, 60,100, RGBa(200,200,200), RGBa(200,200,200));
  AddLightFlash(120, 0,0, RGBa(255,255,200,100));
  Sparks(7+Random(5), RGBa(255,255,150,100));
  /* Rauch und Explosionen! */
  return(Explode(50));
}

public func IsCraneGrabable() { return(1); }
