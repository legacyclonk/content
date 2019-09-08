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
  var idRevaluationTarget = ACLK;

  // Keine Aufwertung zum gleichen Clonktyp
  if (idRevaluationTarget == GetID(target))
    return(NoRevaluation(target, target1, Format("$MsgIsRev$", GetName(target), )));
  // Aufwertung möglich? (z.B. nicht vom König zum Ritter)
  if (idRevaluationTarget->~IsRevaluationBlocked(target))
    return(NoRevaluation(target, target1, Format("$MsgBlocked$", GetName(target))));

  // Umwandlung
  // Jetzt mal auf andere Art, die vorherige kann zu Physical-Bugs führen.
  if (GetOCF(target) & OCF_Alive())
    MorphTo(idRevaluationTarget, target);
  else return(0, RemoveObject());
  // Effekt
  Sound("Magic1",0,target);

  // Clonkspezifische Aktionen nach Umwandlung (z.B. Zauberenergie für Magier)
  target->~OnRevaluation();
  // Rückverwandlung
  AddEffect("FishSkinPSpell", target, 101, 800, 0, GetID());

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
public func GetSpellCombo(pMage) { return ("444"); } // (1: Backward; 2: Down; 3: Forward; 4: Throw; 5: Jump; 6: Dig)

/* Effekt */

public func FxFishSkinPSpellStart(target, no, temp)
{
  if(temp) return();
  // Clonk kann (muss) unter Wasser atmen
  SetPhysical("BreatheWater", 1, 3, target);
  // Grafik verändern
  SetGraphics("FishClonk",target,FHSK);
  // Portrait anpassen
  SetPortrait("FishClonk1",target,FHSK,false);
  // Fertig
  return(1);
}

public func FxFishSkinPSpellTimer(target, no, time)
{
  // Zauber vorbei
  if(time >= 800) return(-1);
}

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
  ResetPhysical(target,"BreatheWater");
  // Magie -> hören
  Sound("DeEnergize", 0, target);
  // Der Clonk bekommt wieder atem
  // Damit es genau abläuft wird erst nach einem Frame die Atemluft wieder aufgefüllt
  Schedule(Format("DoBreath(100,Object(%d))",ObjectNumber(target)),1,1,target);
  // Clonk wieder zurückverwandeln, wenn er noch ein Aquaclonk ist (als er zB nicht Aufwertung benutzt hat)
  if(GetID(target) == ACLK)
    // Umwandlung
    MorphBack(target);
  // Der Rest wird dank meiner neuen Funktion nicht gebraucht ^^
  return(1);
}
