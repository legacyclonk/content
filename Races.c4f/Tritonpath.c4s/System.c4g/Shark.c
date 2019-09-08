/*-- Hai --*/

#strict
#appendto SHRK

private func FindPrey()
{
  var prey;
  SetActionTargets();      
  // Zuerst schwimmende Beute suchen...
  prey = FindObject(0, -250, -100, 500, 200, OCF_Prey(), "Swim", 0, NoContainer());
  if(ObjectCount2(Find_ID(GetID()), Find_ActionTarget(prey))>=2)
    prey = 0;
  // ...dann auch andere Beute im Wasser suchen (z.B. stehende Aquaclonks)
  if (!prey)
  {
    while (prey = FindObject(0, -250, -100, 500, 200, OCF_Prey(), 0, 0, NoContainer(), prey))
    // Höchstens 2 Verfolger
     if(ObjectCount2(Find_ID(GetID()), Find_ActionTarget(prey))<2)
      if (GetAlive(prey))
        if (InLiquid(prey))
          break;
  }
  // Gefundene Beute zurückliefern
  SetActionTargets(prey);
  return(prey);  
}
