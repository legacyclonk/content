/* Vernichtung */

#strict

public func Activate(pCaster,none,pAmulet) {
  if(Global(0)<3) {
    Message("$TxtMyst$",pCaster);
    Sound("Error");
    RemoveObject();
    return(0);
  }
  Explode(50,pCaster);
  Explode(RandomX(15,30),CreateObject(ROCK,RandomX(-40,-20),RandomX(-30,+30),-1));
  Explode(RandomX(15,30),CreateObject(ROCK,RandomX(+20,+40),RandomX(-30,+30),-1));
  Explode(RandomX(15,30),CreateObject(ROCK,RandomX(-30,+30),RandomX(-40,-20),-1));
  Explode(RandomX(15,30),CreateObject(ROCK,RandomX(-30,+30),RandomX(+20,+40),-1));
  Sound("Magic1");
  Sound("Blast3");
  Sound("Blast*");
  Sound("Earthquake");
  SetAction("Armageddon");
  return(1);
}

protected func Chaos() {
  if(Random(3))
    CreateParticle("PxSpark",0,0,0,0,RandomX(4000,9000),RGB(255,40,0));
  Sound("Blast*");
  if(!Random(3)) Sound("Earthquake");
  if(GetActTime()==30) Armageddon();
}

private func Armageddon() {
  var pObj;
  Sound("Blast3");
  while(pObj=FindObject(0,-700,-700,1400,1400,0,0,0,NoContainer(),pObj))
    DoEnergy(-116+ObjectDistance(pObj)/6,pObj);
  while(pObj=FindObject(0,-500,-500,1000,1000,0,0,0,NoContainer(),pObj))
    if(GetOCF(pObj)&OCF_Inflammable())
      Incinerate(pObj);
  while(pObj=FindObject(0,-250,-250,500,500,0,0,0,NoContainer(),pObj)) {
    if(!(GetOCF(pObj)&OCF_Exclusive()))
      Explode(40-ObjectDistance(pObj)/10,pObj);
  }
  RemoveObject();
}

/* Zaubercombo */

public func GetSpellClass(object pMage) { return(EART); }
public func GetSpellCombo(pMage) { return ("242"); } // (1: Backward; 2: Down; 3: Forward; 4: Throw; 5: Jump; 6: Dig)
