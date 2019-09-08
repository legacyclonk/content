/* Einfrier Funktionen */

#strict

global func Freeze(object pObject, object pFrom)
{
  // Erstmal dafür sorgen, dass das Objekt passt
  if(!pObject) pObject = this();
  if(!pObject) return(0);
  if(!GetAlive(pObject)) return(0);
  
  // Wer will denn den Schaden verursachen?
  if(!pFrom) pFrom = this();
  
  // Löschen
  Extinguish(pObject); 
  
  // Kraftfelder sollen nicht eingefroren werden
  if(GetAction(pObject)S="Field") return(0);
  
  // Und jetzt endlich einfrieren
  AddEffect("Freeze", pObject, 111, 5, 0, FREZ, GetController(pFrom)); 
}

global func Unfreeze(object pObject)
{   
  // Passendes Objekt ermitteln
  if(!pObject) pObject = this();
  if(!pObject) return(0);
  
  // Eis- und Frosteffekte entfernen
  while(RemoveEffect("Freeze", pObject));
  while(RemoveEffect("*Ice*", pObject));
}

global func Frozen(object pObject)
{   
  // Passendes Objekt ermitteln
  if(!pObject) pObject = this();
  if(!pObject) return(0);
  
  // Hat das Objekte einen passenden Effekt?
  if(GetEffect("Freeze", pObject)) return(1);
  if(GetEffect("*Ice*", pObject)) return(1);
}

private func DecPhysical(string szPhysical, int iDec, int iMode, object pObject)
{
  // Physical heruntersetzen
  SetPhysical(szPhysical,Min(GetPhysical(szPhysical,0,pObject)-iDec,100000),iMode,pObject);
}

global func FxFreezeStart(pTarget, iEffectNumber, iTemp, iFromController)
{
  // Physicals herunter setzen
  SetPhysical("Dig"   , GetPhysical("Dig"   , 0, pTarget)*3/4, PHYS_StackTemporary, pTarget);
  SetPhysical("Walk"  , GetPhysical("Walk"  , 0, pTarget)*3/4, PHYS_StackTemporary, pTarget);
  SetPhysical("Jump"  , GetPhysical("Jump"  , 0, pTarget)*3/4, PHYS_StackTemporary, pTarget);
  SetPhysical("Swim"  , GetPhysical("Swim"  , 0, pTarget)*3/4, PHYS_StackTemporary, pTarget);
  SetPhysical("Scale" , GetPhysical("Scale" , 0, pTarget)*3/4, PHYS_StackTemporary, pTarget);
  SetPhysical("Hangle", GetPhysical("Hangle", 0, pTarget)*3/4, PHYS_StackTemporary, pTarget);
  SetPhysical("Float" , GetPhysical("Float" , 0, pTarget)*3/4, PHYS_StackTemporary, pTarget);
  // Bei einer temporären Änderung aufhöhren
  if(iTemp) return();
  // Toller Sound
  Sound("Crystal2", 0, pTarget);
  // Zeit einstellen
  EffectVar(0, pTarget, iEffectNumber)+=35;
  // ID speichern
  var id = GetID(pTarget);
  if(GetDefHeight(id)>GetDefWidth(id)) EffectVar(1, pTarget, iEffectNumber) = GetDefHeight(id);
  else EffectVar(1, pTarget, iEffectNumber) = GetDefWidth(id);
  // Verursacher speichern
  EffectVar(2, pTarget, iEffectNumber) = iFromController;
  return(1);    
}

global func FxFreezeTimer(pTarget, iEffectNumber, iEffectTime)
{
  // In Lava schwimmend? Dann schnell auftauen (nur für Monster und andere nicht brennbare Lebewesen wichtig)
  if(GetMaterialVal("Incindiary", "Material", pTarget->GetMaterial()))
    return -1;
  // Partikeleffekte für den betroffenen
  var id = GetID(pTarget);
  CreateParticle("NoGravSpark",GetX(pTarget)+RandomX(-GetDefWidth(id)/2, GetDefWidth(id)/2),GetY(pTarget)+RandomX(-GetDefHeight(id)/2, GetDefHeight(id)/2),0,RandomX(-2, -4),RandomX(18,28),RGB(10,120,210), pTarget);
  CreateParticle("Freeze",GetX(pTarget),GetY(pTarget),0,0,EffectVar(1, pTarget, iEffectNumber)*5+20,RGB(0,40,80),pTarget);
  // Runterzählen
  EffectVar(0, pTarget, iEffectNumber)-=1;
  if(EffectVar(0, pTarget, iEffectNumber)<=0) return FX_Execute_Kill;
  // Ein bischen tut Einfrieren auch weh
  if(!Random(10)) {
   // Damit der richtige Spieler den Schaden verursacht
   var pFrom = GetCursor(EffectVar(2, pTarget, iEffectNumber));
   if(pFrom)
     pFrom->DoEnergy(-1,pTarget);
   else
     DoEnergy(-1, pTarget);
  }
  return FX_OK;
}

global func FxFreezeEffect(string szNewEffectName, object pTarget, int iEffectNumber, int iNewEffectNumber)
{
  // nur einmal einfrieren
  if(szNewEffectName eq "Freeze") { EffectVar(0, pTarget, iEffectNumber)+=35; return(-1); }
  // Feuer löst die Erfrierung
  if(szNewEffectName eq "Fire") return(RemoveEffect(0,pTarget,iEffectNumber));
}

global func FxFreezeStop(pTarget, iEffectNumber, iReason, fTemp)
{
  // Alte Physicals wiederherstellen
  ResetPhysical(pTarget, "Dig");
  ResetPhysical(pTarget, "Walk");
  ResetPhysical(pTarget, "Jump");
  ResetPhysical(pTarget, "Swim");
  ResetPhysical(pTarget, "Scale");
  ResetPhysical(pTarget, "Hangle");
  ResetPhysical(pTarget, "Float");
  // Wenn der Effekt endgültig entfernt wird: Sound!
  if(fTemp) return();
  Sound("DePressurize", 0, pTarget);
  return(0);
}
