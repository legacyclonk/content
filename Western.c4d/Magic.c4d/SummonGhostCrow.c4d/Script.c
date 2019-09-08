/* Geisterkrähe beschwören */

#strict

func Activate(pCaster,pRealcaster) {
  var pClonk, pComboObj, fBlast;
  if(pRealcaster) pClonk=pRealcaster;
  else pClonk=pCaster;

 // Effekt prüfen
  var iResult;
  if (iResult = CheckEffect("IceCrowPSpell", 0, 125)) return(iResult!=-1 && RemoveObject());  
 
  CreateObject(GHCR, 0, 0, GetOwner(pClonk))->SetMaster(pClonk);
  
  Sound("Raven*");
  RemoveObject();
  return(1);
}

/* Zaubercombo */

public func GetSpellClass(object pMage) { if (GetName(0, ICE2)) return(ICE2); else return(WATR); }
public func GetSpellCombo(pMage) { if (GetName(0, ICE2)) return ("655"); else return("155"); } // (1: Backward; 2: Down; 3: Forward; 4: Throw; 5: Jump; 6: Dig)
