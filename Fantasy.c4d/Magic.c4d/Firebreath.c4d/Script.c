/*-- Feueratem --*/

#strict

protected func Activate(pCaster, pRealcaster)
  {
  var pObj,pClonk;
  
  // Effekt prüfen
  var iResult;
  if (iResult = CheckEffect("FireBreathNSpell", 0, 125)) return(iResult!=-1 && RemoveObject());

  // Den Clonk ermitteln
  if(pRealcaster) pClonk=pRealcaster;
  else pClonk=pCaster;

  // Zielen wenn möglich
  if (pClonk->~DoSpellAim(this(), pCaster)) return(1);

  // wenn nicht möglich Feueratem erstellen: Als Aktionsparameter pCaster
  CreateFireBreath(pCaster,40,GetDir(pCaster)*12-6,-4,0);
  Sound("DragonFire");
  RemoveObject();
  return(1);
  }

// Aktivierung nach Zielen (iAngle: -90=links; 0=oben; +90=rechts)
public func ActivateAngle(object pCaller, int iAngle)
  {
  if(GetAction(pCaller) eq "HoverCast") return(0);

  // Feueratem ausführen
  if(!GetDir(pCaller) && iAngle>0) { CreateFireBreath(pCaller,40,Sin(iAngle,6),Cos(iAngle,8)-4,-Abs(iAngle)-90); Message("!", pCaller);} // Hack für Artefakte
  else CreateFireBreath(pCaller,40,Sin(iAngle,6),Cos(iAngle,8)-4,Abs(iAngle)-90);
  Sound("DragonFire");
  // Objekt löschen
  RemoveObject();
  return(1);
  }

public func IsAimerSpell() { return(1); }

public func GetSpellClass(object pMage) { return(FIRE); }
public func GetSpellCombo(pMage) { return ("344"); } // (1: Backward; 2: Down; 3: Forward; 4: Throw; 5: Jump; 6: Dig)
