/*-- Deckenlicht --*/

#strict
#include BLGH

func CreateLight() {
  pLight = AddLightAmbience(55);
  pLight->ChangeGraphics("Half");
  pLight->SetR(180);
  return(1);
}
