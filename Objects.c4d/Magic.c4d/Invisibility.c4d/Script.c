/* Unsichtbarkeitszauber mit Effektsystem */

#strict

// EffectVars:
//   0 - Vorheriger Sichtbarkeitsstatus
//   1 - Vorherige Farbmodulation

public func Activate(object pCaster, object pCaster2)
  {
  // Zauberer ermitteln
  if (pCaster2) pCaster = pCaster2;
  // Magie kann man hören, ganz klar ;)
  Sound("Magic*");
  // Wenn Objektkomponente vorhanden, mehrere Objekte unsichtbar machen
  var pCom=FindContents(LNKT,pCaster),pObj,iCnt;
  // Objektkomponente vorhanden
  if(pCom) {
    // alle Objekte in der Nähe zur Unsichtbarmachung in Erwägung ziehen
    while(pObj=FindObject(0,0,0,-1,-1,0,0,0,NoContainer(),pObj)) {
      // nur Freunde (außer wenn eine bestimmte Regel aktiviert ist)
      if(Hostile(GetOwner(pObj),GetOwner(pCaster)) && !ObjectCount(NTMG)) continue;
      // nur Objekte die einen Besitzer haben
      if(GetOwner(pObj)==-1) continue;
      // Objekt zu weit weg oder schon genug Objekte unsichtbar gemacht 
      if(ObjectDistance(pObj,pCaster)>100 || (iCnt++)>RandomX(3,5)) break;
      AddEffect("InvisPSpell",pObj,1400,1111,0,GetID());
    }
  }
  if(iCnt>1) RemoveObject(pCom);
  // Zauberer unsichtbar machen (40sec)
  else AddEffect("InvisPSpell", pCaster, 200, 1400, 0, GetID());
  // Fertig - das Zauberobjekt wird nun nicht mehr gebraucht
  return(RemoveObject());
  }

protected func FxInvisPSpellStart(object pTarget, int iEffectNumber, int iTemp)
  {
  // Vorherige Sichtbarkeit des Zauberers speichern
  EffectVar(0, pTarget, iEffectNumber) = GetVisibility(pTarget);
  EffectVar(1, pTarget, iEffectNumber) = GetClrModulation(pTarget);
  // Zauberer unsichtbar machen
  SetVisibility(VIS_Owner() | VIS_Allies() | VIS_God(), pTarget);
  // Halbdurchsichtig bläulich für den Besitzer und Verbündete
  SetClrModulation(ModulateColor(GetClrModulation(pTarget),RGBa(127,127,255,127)), pTarget);
  // Fertig
  return(1);
  }

protected func FxInvisPSpellStop(object pTarget, int iEffectNumber, int iCause, bool fTemp)
  {
  // Status wiederherstellen
  SetVisibility(   EffectVar(0, pTarget, iEffectNumber), pTarget);
  SetClrModulation(EffectVar(1, pTarget, iEffectNumber), pTarget);
  // Fertig
  return(1);
  }

protected func FxInvisPSpellEffect(szNewEffect, pTarget, iEffectNumber)
  {
  // Andere Unsichtbarkeitszauber aufnehmen
  if (SEqual(szNewEffect, "InvisPSpell")) return(-2);
  // Sonst keine Behandlung
  return();
  }

protected func FxInvisPSpellAdd(object pTarget, int iEffectNumber, string szNewEffect, int iNewTimer)
  {
  // Effekt aufrechnen: Rest vom alten Timer ermitteln
  var iOldTimer = GetEffect(0, pTarget, iEffectNumber, 3) - GetEffect(0, pTarget, iEffectNumber, 6);
  // Diesen Effekt mit neuem Timer weiterführen
  ChangeEffect(0, pTarget, iEffectNumber, szNewEffect, iOldTimer + iNewTimer);
  // Fertig
  return(1);
  }

public func GetSpellClass(object pMage) { return(AIR1); }
public func GetSpellCombo(pMage) { return ("122"); } // (1: Backward; 2: Down; 3: Forward; 4: Throw; 5: Jump; 6: Dig)
