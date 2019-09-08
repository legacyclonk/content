/* Beistand */

#strict
#include SPRK

protected func Process() {
  if (GetActTime()>60) RandomChange();
}

protected func Hit() { CastObjects(SPRK,3,25); }

private func RandomChange() {
  var iRand=Random(16);
  if(!iRand--) return(ChangeDef(BONE));
  if(!iRand--) return(ChangeDef(LTHR));
  if(!iRand--) return(ChangeDef(SKIN));
  if(!iRand--) return(ChangeDef(MEAT));
  if(!iRand--) return(ChangeDef(FAT1));
  if(!iRand--) return(ChangeDef(SWBL));
  if(!iRand--) return(ChangeDef(WOOD));
  RemoveObject();
}