/* Fischhaut */

#strict

public func Activate(target1, target2)
  {
  var clonk = target1;
  if (target2) clonk = target2; 

  // Zauberndes Objekt (evtl. zaubernder Clonk draußen) aufwerten
  return(Revaluation(clonk, target1));
  }

public func Revaluation(target, target1)
  {
  // Aufwertungsziel: Aquaclonk
  var idRevaluationTarget = ACLK, id2 = GetID(target);
  // Zauberbegabte Clonks werden zum Fischclonk
  if(GetPhysical("Magic", 0, target)) idRevaluationTarget = FCLK;

  // Keine Aufwertung zum gleichen Clonktyp
  if (idRevaluationTarget == GetID(target))
    return(NoRevaluation(target, target1, Format("$MsgIsRev$", GetName(target), )));
  // Aufwertung möglich? (z.B. nicht vom König zum Ritter)
  if (idRevaluationTarget->~IsRevaluationBlocked(target))
    return(NoRevaluation(target, target1, Format("$MsgBlocked$", GetName(target))));

  // Effekt
  Sound("Magic1");
  // Umwandlung
  if (!target->~Redefine(idRevaluationTarget))
    if (GetOCF(target) & OCF_Alive())
      {
      ChangeDef(idRevaluationTarget, target);
      ObjectSetAction(target, "Walk");
      }
    else return(0, RemoveObject());
  // Clonkspezifische Aktionen nach Umwandlung (z.B. Zauberenergie für Magier)
  target->~OnRevaluation();
  // Fix: Klettern & Hangeln wieder aktivieren
  SetPhysical("CanScale", 1, 2, target);
  SetPhysical("CanHangle", 1, 2, target);
  // Rückverwandlung
  AddEffect("FishSkinPSpell", target, 101, 800, 0, GetID(), id2, GetPortrait(target), GetPortrait(target, 1));
  // Grafik verändern
  SetGraphics("FishClonk", target, FHSK);
  // Portrait
  SetPortrait("FishClonk1", target, FHSK);

  RemoveObject(); 
  return(1);
  }

public func NoRevaluation(target, target1, szErrorMessage)
{
  // Zauber fehlgeschlagen
  Sound("Error", false, 0, 100, GetOwner()+1);
  PlayerMessage(GetOwner(), szErrorMessage, target);
  RemoveObject(); 
  return(0);
}

public func GetSpellClass(object pMage) { return(WATR); }
public func GetSpellCombo(pMage) { return ("625"); } // (1: Backward; 2: Down; 3: Forward; 4: Throw; 5: Jump; 6: Dig)

/* Effekt */

public func FxFishSkinPSpellStart(target, no, temp, id2, portrait, portraitId)
{
  if(temp) return();
  // Clonk kann (muss) unter Wasser atmen
  SetPhysical("BreatheWater", 1, 2, target);
  // Alte ID des Clonks speichern
  EffectVar(0, target, no) = id2;
  // Portrait speichern
  EffectVar(1, target, no) = portrait;
  EffectVar(2, target, no) = portraitId;
}

public func FxFishSkinPSpellTimer(target, no, time)
{
  // Zauber vorbei
  if(time >= 800) return(-1);
}

func FxFishSkinPSpellInfo() { return ("{{FHSK}} $SpellInfo$"); }

public func FxFishSkinPSpellDamage(target, no, dmg, cause)
{
  // Erstickungsschaden -> Zauber endet
  if(cause & 37)
    {
    RemoveEffect("FishSkinPSpell", target);
    // Kein Schaden
    return();
    }
  // Feuerschanden -> mehr Schaden
  if(cause & 35)
    return(dmg + dmg/2);
}

public func FxFishSkinPSpellStop(target, no, reason, temp)
{
  if(temp) return();
  // Clonk ist nun kein Fisch mehr
  SetPhysical("BreatheWater", 0, 2, target);
  // Magie -> hören
  Sound("DeEnergize", 0, target);
  // Clonk wieder zurückverwandeln, wenn er noch ein Aquaclonk ist (als er zB nicht Aufwertung benutzt hat)
  var id2 = EffectVar(0, target, no);
  if(GetID(target) == FCLK || GetID(target) == ACLK)
    {
    // Umwandlung
    if (!target->~Redefine(id2))
      if (GetOCF(target) & OCF_Alive())
        {
        ChangeDef(id2, target);
        ObjectSetAction(target, "Walk");
        }
    // Fix: Klettern & Hangeln wieder aktivieren
    SetPhysical("CanScale", 1, 2, target);
    SetPhysical("CanHangle", 1, 2, target);
    // Grafik zurücksetzen
    SetGraphics(0, target, GetID(target));
    // Portrait zurücksetzen
    SetPortrait(EffectVar(1, target, no), target, EffectVar(2, target, no));
    }
}
