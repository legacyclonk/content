/* Seehundsterben */

#strict
#include DTFH  // siehe Fischsterben

protected func Slay() {
  var pSeal;
  while(pSeal=FindObject(SEAL,-iRand/6,-iRand/6,iRand/3,2000,0,0,0,0, pSeal)) Kill(pSeal);
  RemoveObject();
}

/* Zaubercombo */

public func GetSpellClass(object pMage) { return(FIRE); }
public func GetSpellCombo(pMage) { return ("211"); } // (1: Backward; 2: Down; 3: Forward; 4: Throw; 5: Jump; 6: Dig)
