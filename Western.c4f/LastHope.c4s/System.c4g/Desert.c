/*-- Hitze --*/

#strict

#appendto INDI

// Die Wüste ist durstiger

func FxLifeTimer()
{
  // Durst steigt
  if(FindObject(NDWA))
    if(GetMaterial(0, 0) != Material("Water"))
      if(GetX()>1960 && GetX()<4140)
        if(!Contained())
          iThirst=BoundBy(iThirst+19*ObjectCount(NDWA), 0, MaximumThirst()); // Extremer Durst
  return(_inherited());
}
