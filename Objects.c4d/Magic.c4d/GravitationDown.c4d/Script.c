/*-- Gravitation runter --*/

#strict

public func Activate(pCaster, pCaster2)
  {
  // Gültigkeit prüfen
  if(GetGravity()<=30) { Message("$MsgGravityMinimum$",pCaster); return(0, RemoveObject()); }
  // Magie kann man hören, ganz klar ;)
  Sound("Magic*");
  // Effekt global starten
  if (!AddEffect("GravChangeUSpell", 0, 150, 80, 0, GetID(), -20))
    return(0, RemoveObject());
  // Gravitation anzeigen
  Message("$MsgGravityDec$",pCaster,GetGravity());
  // Fertig - das Zauberobjekt wird nun nicht mehr gebraucht
  return(RemoveObject());
  }


/*-- Gravitationseffekt --*/
// Wird auch vom anderen Gravitationszauber verwendet

// EffectVars: 0 - Vorherige Gravitation
//             1 - Änderung durch den Zauber

protected func FxGravChangeUSpellStart(pTarget, iEffectNumber, iTemp, iChange)
  {
  // Effektstart
  if (!iTemp) EffectCall(pTarget, iEffectNumber, "DoSparks", iChange);
  // Vorherige Gravitation sichern
  var iOldGrav = EffectVar(0, pTarget, iEffectNumber) = GetGravity();
  // Für nichttemporäre Aufrufe wird iChange übergeben, und auf den Änderungswert aufgerechnet
  if (iChange) EffectVar(1, pTarget, iEffectNumber) += iChange;
  // Gravitationsänderung setzen
  // Die Änderung kann in temporären Aufrufen auch ungleich iChange sein
  SetGravity(iOldGrav + EffectVar(1, pTarget, iEffectNumber));
  // Fertig
  return(1);
  }

protected func FxGravChangeUSpellTimer(pTarget, iEffectNumber)
  {
  // Gravitation in Richtung Normalwert schrauben
  var iGravChange = EffectVar(1, pTarget, iEffectNumber);
  // Fertig?
  if (Inside(iGravChange, -1, 1)) return(-1);
  // Anpassen
  var iDir = -iGravChange/Abs(iGravChange);
  if (Abs(iGravChange) > 40) iDir*=2;
  EffectVar(1, pTarget, iEffectNumber) += iDir;
  SetGravity(GetGravity() + iDir);
  return(1);
  }

protected func FxGravChangeUSpellStop(pTarget, iEffectNumber)
  {
  // Gravitation Wiederherstellen
  SetGravity(EffectVar(0, pTarget, iEffectNumber));
  // Effekt entfernen
  return(1);
  }

protected func FxGravChangeUSpellEffect(szNewEffect, pTarget, iEffectNumber)
  {
  // Falls der neu hinzugefügte Effekt auch eine Gravitationsänderung ist, Interesse am Übernehmen anmelden
  if (SEqual(szNewEffect, "GravChangeUSpell")) return (-3);
  // Ansonsten ignorieren
  return();
  }

protected func FxGravChangeUSpellAdd(pTarget, iEffectNumber, szNewEffect, pTarget, iNewTimer, iChange)
  {
  // Aufruf erfolgt, wenn der Effekt übernommen werden konnte
  // Gravitationsänderung auf diesen Effekt aufrechnen
  EffectVar(1, pTarget, iEffectNumber) += iChange;
  SetGravity(GetGravity() + iChange);  
  // Grafikeffekt
  EffectCall(pTarget, iEffectNumber, "DoSparks", iChange);
  // Fertig
  return(1);
  }

protected func FxGravChangeUSpellDoSparks(pTarget, iEffectNumber, iMovement)
  {
  // Graphischer Effekt
  var lwdt=LandscapeWidth(),lhgt=LandscapeHeight(),x=GetX(),y=GetY();
  var i=LandscapeWidth()*LandscapeHeight()/10000;
  while (i--) CreateParticle("MSpark", Random(lwdt)-x,Random(lhgt)-y, 0,iMovement*2, 30+Random(50), RGBa(255-Random(50),255-Random(50),0,128));
  }

public func GetSpellClass(object pMage) { return(EART); }
public func GetSpellCombo(pMage) { return ("222"); } // (1: Backward; 2: Down; 3: Forward; 4: Throw; 5: Jump; 6: Dig)
