/*-- Schießpulver --*/

#strict 2

#appendto GUNP

protected func Damage()
{
  // Explodiert nicht, wenn er von einem Clonk getragen wird (Sonderregel gegen T-Flint-Superjumps)
  if(Contained())
    if(Contained()->~IsClonk()) {
      Extinguish();
      return(0);
    }
  // Explosion - verzögert, um Endlosrekursion zu verhindern
  // Hängt von der Größe ab
  if(GetCon() < 4) return(RemoveObject(this()));
  Schedule(Format("Explode(%d)",GetCon()*3/10),1);
  return(1);
}