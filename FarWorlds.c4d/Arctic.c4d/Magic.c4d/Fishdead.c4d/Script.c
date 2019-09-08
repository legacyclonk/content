/* Fischsterben */

#strict

local iRand;

public func Activate(pCaster,none,pAmulet) {
  Sound("Wave");
  SetAction("Active",pCaster);
  return(1);
}

protected func Effect() {
  iRand=RandomX(GetActTime()*120,GetActTime()*160);
  CreateParticle("PxSpark",0,0,0,0,iRand,RGB(255,40,0));
}

protected func Slay() {
  var pFish;
  while(pFish=FindObject(FISH,-iRand/6,-iRand/6,iRand/3,2000,0,0,0,0, pFish)) Kill(pFish);
  while(pFish=FindObject(QFSH,-iRand/2,-iRand/6,iRand,2000,0,0,0,0,pFish))    Kill(pFish);
  RemoveObject();
}

/* Zaubercombo */

public func GetSpellClass(object pMage) { return(FIRE); }
public func GetSpellCombo(pMage) { return ("233"); } // (1: Backward; 2: Down; 3: Forward; 4: Throw; 5: Jump; 6: Dig)
