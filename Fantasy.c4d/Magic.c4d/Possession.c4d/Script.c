/* Besitznahme */

#strict

public func PossessionTime() { return(35 * 60 * 2); } // Dauer des Zaubers in Frames (2 Minuten)

/* Tierauswahl */

public func Activate(object pCaster, object pRealCaster)
{
 var pClonk = pCaster;
 if (pRealCaster) pClonk = pRealCaster;

 // Dieser Zauber kann nur im Freiem ausgeführt werden
 if (Contained(pClonk))
 {
  Message("$NotInFree$", pClonk, GetName(pClonk));
  Sound("Error");
  return(RemoveObject());
 }

 // Per Selektor auswählen, wenn möglich
 if (pClonk->~DoSpellSelect(this(), 400, pCaster)) return(1);

 // Wenn nicht, dann automatisch ein Tier suchen
 var pAnimal;
 while (pAnimal = FindObject(0, -400,-400,800,800, OCF_Alive(), 0,0, 0, pAnimal))
  if (ObjectDistance(pAnimal) <= 400)
   if (SelectorTarget(pAnimal))
    return(ActivateTarget(pClonk, pAnimal));

 // Falls kein Tier gefunden worden ist, eine Fehlermeldung ausgeben
 PlayerMessage(GetController(pClonk), "$CanNotControl$", pClonk, GetName(pClonk));
 Sound("Error", true, 0, 100, GetController(pClonk)+1);
 RemoveObject();
 return(0);
}

public func SelectorTarget(object pTarget)
{
 // Das Zielobjekt muss am Leben sein
 if (!(pTarget->GetOCF() & OCF_Alive())) return();

 // Das Zielobjekt muss übernehmbar sein
 if (!pTarget->~IsPossessible()) return();

 // Das Zielobjekt muss sich im Freiem befinden
 if (Contained(pTarget)) return();

 // Das Zielobjekt darf nicht bereits von einem Clonk gesteuert werden
 if (GetEffect("PossessionSpell", pTarget)) return();

 // Das Zielobjekt muss für den Besitzer sichtbar sein
 if (!Visible(GetOwner(), pTarget)) return();

 return(1);
}

public func ActivateTarget(object pCaller, object pTarget)
{
 // Die Seele des Clonks in das Tier hineinversetzen
 AddEffect("PossessionSpell", pTarget, 200, 35, 0, GetID(), pCaller);

 // Diesen Zauber löschen
 return(RemoveObject());
}

/* Tierübernahme */

protected func FxPossessionSpellStart(pTarget, iEffectNumber, fTemp, pCaster)
{
 // Keine temporären Aufrufe
 if (fTemp) return();

 // Der Zauberer lässt alle Gegenstände fallen
 while (pCaster->Contents()) pCaster->Contents()->Exit(0,0,pCaster->GetDefBottom()-pCaster->GetY());

 // Den Zauberer, seine Position und sein Portrait speichern
 EffectVar(0, pTarget, iEffectNumber) = pCaster;
 EffectVar(1, pTarget, iEffectNumber) = GetX(pCaster);
 EffectVar(2, pTarget, iEffectNumber) = GetY(pCaster);
 EffectVar(3, pTarget, iEffectNumber) = GetPortrait(pCaster);
 EffectVar(4, pTarget, iEffectNumber) = GetPortrait(pCaster, 1);

 // Die Aktion des Zauberers auf Walk zurücksetzen, und ihn in das Tier hineinversetzen.
 pCaster->SetAction("Walk");
 Enter(pTarget, pCaster);
 
 // Den Controller anpassen
 SetController(GetController(pCaster),pTarget);
 
 // Das Portrait anpassen
 SetPortrait("random", pCaster, GetID(pTarget));

 // Effekte
 DoParticleEffect(pCaster->GetX(), pCaster->GetY());
 DoParticleEffect(pTarget->GetX(), pTarget->GetY());
 DrawParticleLine("PxSpark", pCaster->GetX(), pCaster->GetY(), pTarget->GetX(), pTarget->GetY(), 15, 150, RGBa(255,50,50,50), RGBa(50,255,50,100));
 Sound("Magic1");
}

protected func FxPossessionSpellTimer(pTarget, iEffectNumber, iEffectTime)
{
 // Nach einer festgelegten Zeit abbrechen
 if (iEffectTime >= PossessionTime()) return(-1);

 // Countdown für die letzten 10 Sekunden
 if (iEffectTime >= (PossessionTime()-10*35))
 {
  var pSorcerer = EffectVar(0, pTarget, iEffectNumber);
  PlayerMessage(GetOwner(pSorcerer), "@%d", pTarget, PossessionTime() / 35 - iEffectTime / 35);
 }
}

protected func FxPossessionSpellStop(pTarget, iEffectNumber, iReason, fTemp)
{
 // Keine temporären Aufrufe
 if (fTemp) return();

 var pSorcerer = EffectVar(0, pTarget, iEffectNumber);

 // Der Zauberer lässt alle Gegenstände fallen
 while (pSorcerer->Contents()) pSorcerer->Contents()->Exit(0,0,6);

 // Das Tier verliert die Countdown Message
 PlayerMessage(GetOwner(pSorcerer), "", pTarget);

 // Den Zauberer zurück an seine Ursprungsposition stellen
 Exit(pSorcerer);
 SetPosition(EffectVar(1, pTarget, iEffectNumber), EffectVar(2, pTarget, iEffectNumber), pSorcerer);

 // Das Portrait des Zauberers anpassen
 SetPortrait(EffectVar(3, pTarget, iEffectNumber), pSorcerer, EffectVar(4, pTarget, iEffectNumber));

 // Wenn der Clonk noch einen Zauber zielt wird dieser abgebrochen
 if(LocalN("pAimer", pSorcerer))
  pSorcerer->~AbortCasting();

 // Effekte
 DoParticleEffect(pSorcerer->GetX(), pSorcerer->GetY());
 DoParticleEffect(pTarget->GetX(), pTarget->GetY());
 DrawParticleLine("PxSpark", pSorcerer->GetX(), pSorcerer->GetY(), pTarget->GetX(), pTarget->GetY(), 15, 150, RGBa(255,50,50,50), RGBa(50,255,50,100));
 Sound("DeMagic");
}

protected func DoParticleEffect(x, y, color1, color2) 
{
  if (!color1) color1 = RGB(196, 196, 196);
  if (!color2) color2 = RGB(255, 255, 255);
  CastParticles("PxSpark", 5, 35, AbsX(x), AbsY(y), 5*8, 5*50, color1, color2);
  return(1);
}

/* Zaubercombo */

public func GetSpellClass(object pMage) { return(EART); }
public func GetSpellCombo(pMage) { return ("555"); } // (1: Backward; 2: Down; 3: Forward; 4: Throw; 5: Jump; 6: Dig)
