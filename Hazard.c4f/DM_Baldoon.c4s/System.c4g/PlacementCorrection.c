/*-- Placement Correction --*/
/*-- Vernichtet die in der Erde platzierten Objekte wenn sie zu nahe an der Erdoberfläche liegen --*/

#strict
#appendto ROCK
#appendto LOAM
#appendto BONE

func Initialize() {
    if(!GBackSolid(-6,-6) || !GBackSolid(6,-6) || !GBackSolid(6,6) || !GBackSolid(-6,6)) RemoveObject();
    if(GBackLiquid(-6,-6) || GBackLiquid(6,-6) || GBackLiquid(6,6) || GBackLiquid(-6,6)) RemoveObject();
  return(1);
}
