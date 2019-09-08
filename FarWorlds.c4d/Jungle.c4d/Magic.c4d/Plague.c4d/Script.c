/* Verseuchen */
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
  iRand=RandomX(GetActTime()*70,GetActTime()*90);
  CreateParticle("PxSpark",0,0,0,0,iRand,RGB(40,255,0));
}

protected func Slay() {
  var pClonk;
  while(pClonk=FindObject(0,-iRand/6,-iRand/6,iRand/3,iRand/3,OCF_CrewMember(),0,0,0, pClonk))
    if(GetActionTarget() != pClonk)
      AddEffect("PoisonCurse",pClonk,182,37*30,0,PARW);
  RemoveObject();
}

public func GetSpellClass(object pMage) { return(EART); }
public func GetSpellCombo(pMage) { return ("466"); } // (1: Backward; 2: Down; 3: Forward; 4: Throw; 5: Jump; 6: Dig)
