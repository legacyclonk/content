/* Zielen für KI-Zauberer */

#strict 2

#appendto MCLK

public func DoSpellAim(object pSpell)
  {
  // Winkelziel-Callback
  // Normaler Spieler
  if (!ai_iRange) return(_inherited(pSpell, ...));
  // KI: Zielt immer aufs gegenwärtige Ziel
  var pTarget = ai_pSpellTarget;
  if (!pTarget) pTarget = ai_pTarget;
  if (!pTarget) return(0);
  SetDir(GetX(pTarget) > GetX());
  var iAngle = Angle(GetX(), GetY(), GetX(pTarget), GetY(pTarget));
  if (iAngle > 180) iAngle -= 360;
  //Log("%v ActivateAngle %d", pSpell, iAngle);
  if (GetEffect("*Recall*Aiming*", this))
    {
    // Zauber abbrechen!
    Call("OnAimerAbort");
    if (pAimer) RemoveObject(pAimer);
    return true;
    }
  pSpell->~AimingAngle(this, iAngle);
  pSpell->ActivateAngle(this, iAngle);
  return true;
  }

public func DoSpellSelect(object pSpell, int iRange)
  {
  //Log("DoSpellSelect(%s, %d, %v,%v,%v) TRG=%v", GetName(pSpell), iRange, x,y,z, ai_pTarget);
  // Zielauswahl-Callback
  // Normaler Spieler
  if (!ai_iRange) return(_inherited(pSpell, iRange, ...));
  // KI: Zielt bevorzugt auf die Befehlsziele; ansonsten auf sich, wenn möglich
  var pTarget;
  if (!TrySelectorTarget(pSpell, pTarget=ai_pSpellTarget, iRange))
    if (!TrySelectorTarget(pSpell, pTarget=ai_pTarget, iRange))
      if (!TrySelectorTarget(pSpell, pTarget=this, iRange))
        return;
  //Log("Activate spell %v @ %v)", pSpell, pTarget);
  pSpell->ActivateTarget(this, pTarget);
  return true;
  }
  
private func TrySelectorTarget(object pSpell, object pTarget, int iRange)
  {
  if (!pTarget) return;
  if (ObjectDistance(pTarget) > iRange) return;
  return (pSpell->~SelectorTarget(pTarget));
  }

global func Fling(object o, int vx, int vy)
  {
  // Feuerklumpen-Fling nicht auf KI
  if (GetID()==MFBL) if (LocalN("ai_iRange", o)) return;
  return(_inherited(o, vx, vy, ...));
  }
