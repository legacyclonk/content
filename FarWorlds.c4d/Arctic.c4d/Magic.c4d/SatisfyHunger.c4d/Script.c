/* Hunger stillen */

#strict

func Activate(pCaster,none,pAmulet) {
  if(!(pCaster->~MagicHunger(250)))
    return(0,RemoveObject()); 
  Sound("Magic1");
  CreateParticle("PxSpark",RandomX(-5,5),RandomX(-5,5),0,0,RandomX(80,200),RGB(255,255,255));
  RemoveObject();
  return(1);
}

/* Zaubercombo */

public func GetSpellClass(object pMage) { return(WATR); }
public func GetSpellCombo(pMage) { return ("511"); } // (1: Backward; 2: Down; 3: Forward; 4: Throw; 5: Jump; 6: Dig)
