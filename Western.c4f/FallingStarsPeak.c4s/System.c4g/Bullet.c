/* Hilfsfunktionen */

#strict

#appendto CSHO

/* Fliegt */
private func Travel ()
{
  CreateParticle("ShotSpark",                  0,                  0, 0, 0, 30, RGB(255,255,0));
  CreateParticle("ShotSpark", -GetXDir()*50/1000, -GetYDir()*50/1000, 0, 0, 30, RGB(255,255,0));
  CreateParticle("ShotSpark", -GetXDir()*25/1000, -GetYDir()*25/1000, 0, 0, 30, RGB(255,255,0));
  CreateParticle("ShotSpark", -GetXDir()*75/1000, -GetYDir()*75/1000, 0, 0, 30, RGB(255,255,0));
  return(_inherited());
}

private func HitLiving (living)
{
  if(living->~ShotHit(GetOwner())) return(RemoveObject());	
  Sound("Punch*");
  // Schaden machen
  if(Random(5))
    DoEnergy(-dam, living);
  else
    // Manchmal Punch benutzen
    Punch(living, dam);
  RemoveObject();
}
