/* Tiersterben */
/* Script größtenteils aus Arktis übernommen */

#strict

local iRand;

public func Activate(pCaster,pRealcaster) {
  Sound("Wave");
  if(pRealcaster) pCaster=pRealcaster;
  SetAction("Active",pCaster);
  return(1);
}

protected func Effect() {
  iRand=RandomX(GetActTime()*120,GetActTime()*160);
  CreateParticle("PxSpark",0,0,0,0,iRand,RGB(255,40,0));
}

protected func Slay() {
  var pAnimal,i=-1;
  while(Animals(++i))
    while(pAnimal=FindObject(Animals(i),-iRand/6,-iRand/6,iRand/3,iRand/3,0,0,0,0, pAnimal)) Kill(pAnimal);
  RemoveObject();
}

private func Animals(int i) {
  if(i==0) return(PIRA);
  if(i==1) return(BIRD);
  if(i==2) return(SNKE);
  if(i==3) return(WIPF);
  if(i==4) return(ZAP1);
  return(0);
}

public func GetSpellClass(object pMage) { return(FIRE); }
public func GetSpellCombo(pMage) { return ("222"); } // (1: Backward; 2: Down; 3: Forward; 4: Throw; 5: Jump; 6: Dig)
