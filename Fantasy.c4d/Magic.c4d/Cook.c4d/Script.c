/* Kochen */

#strict

public func Activate(pCaller, pClonk)
{                            
  if(!pClonk) pClonk = pCaller;
  // Effekt prüfen
  var iChkEff;
  if (iChkEff = CheckEffect("CookPSpell", pCaller, 180)) return(iChkEff!=-1 && RemoveObject());
  // Inhalt untersuchen
  var obj = Contents(0, pClonk), obj2;
  var res;
  if(!obj) res = 0;
  else if(obj->~Cook()) res = 1; // Kochbar?
  else if(obj->~Bake()) res = 1; // Backbar?
  else if(GetAlive(obj)) { Kill(obj); res=1; } // Tötbar?
  
  if(!res) return(0, Message("$NoCook$", pCaller), RemoveObject());
  // Einmaliger Effekt pro Zauber
  Sound("Cook");
  CastParticles("Fire", 20, 30, 0, 0, 50, 70, RGBa(255, 100, 0, 50), RGBa(200, 100, 0, 75),pClonk);
  CastParticles("Fire2", 100, 50, 0, 0, 50, 70, RGBa(255, 100, 0, 50), RGBa(200, 100, 0, 75),pClonk);
  return(true);
}

public func GetSpellClass(object pMage) { return(FIRE); }
public func GetSpellCombo(pMage) { return ("265"); } // (1: Backward; 2: Down; 3: Forward; 4: Throw; 5: Jump; 6: Dig)

