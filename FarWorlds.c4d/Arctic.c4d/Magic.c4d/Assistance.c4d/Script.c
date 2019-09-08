/* Beistand */

#strict

public func Activate(pCaster,none,pAmulet) {
  if(Global()<3) {
    Message("$TxtMyst$",pCaster);
    Sound("Error");
    RemoveObject();
    return(0);
  }
  Sound("Magic1");
  SetAction("Cast");
  return(1);
}


protected func Casting() {
    SetXDir(5,CreateObject(SPRK,10+Random(90),Random(71)-100,-1));
    SetXDir(-5,CreateObject(SPRK,-(10+Random(90)),Random(71)-100,-1));
    SetXDir(5,CreateObject(SPRK,10+Random(90),Random(71)-100,-1));
    SetXDir(-5,CreateObject(SPRK,-(10+Random(90)),Random(71)-100,-1));

    if(GetActTime()>80) RemoveObject();  
    if(Random(2)) {
      SetXDir(+10,CreateObject(MSPK,10+Random(90),Random(71)-100,-1));
      SetXDir(-10,CreateObject(MSPK,-(10+Random(90)),Random(71)-100,-1));
    }
}

/* Zaubercombo */

public func GetSpellClass(object pMage) { return(AIR1); }
public func GetSpellCombo(pMage) { return ("622"); } // (1: Backward; 2: Down; 3: Forward; 4: Throw; 5: Jump; 6: Dig)
