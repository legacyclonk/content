/* Geisterkrähe beschwören */

#strict

func Activate(pCaster,pRealcaster) {
  var pClonk, pComboObj, fBlast;
  if(pRealcaster) pClonk=pRealcaster;
  else pClonk=pCaster;

 // Effekt prüfen
  var iResult;
  if (iResult = CheckEffect("IceCrowPSpell", 0, 125)) return(iResult!=-1 && RemoveObject());  
 
  CreateObject(ICCR, 0, 0, -1)->SetMaser(pClonk);
  
  Sound("Raven*");
  RemoveObject();
  return(1);
}

/* Zaubercombo */

public func GetSpellClass(object pMage) { return(AIR1); }
public func GetSpellCombo(pMage) { return ("654"); } // (1: Backward; 2: Down; 3: Forward; 4: Throw; 5: Jump; 6: Dig)
