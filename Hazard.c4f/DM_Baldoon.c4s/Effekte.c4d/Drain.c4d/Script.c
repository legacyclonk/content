/*-- Abfluss --*/

#strict

func Initialize() {

  return(1);
}

func Drain() {
  if(!ExtractMaterialAmount(0,0,Material("Acid"),40)) {return(RemoveObject());}
}
