/*-- Spind --*/

#strict
#include CHST

func Damage(int iChange) {
  if(GetAction() ne "Open")
    SetAction("Open");
    
  AddEffect("Opening",this(),50,1,this(),GetID(),Abs(iChange));
}

func FxOpeningStart(object pTarget, int iEffectNumber, int iTemp, int speed) {
  EffectVar(0,pTarget,iEffectNumber) = speed;
  EffectVar(1,pTarget,iEffectNumber) = 1;
}

func FxOpeningTimer(object pTarget, int iEffectNumber, int iEffectTime) {
  if(EffectVar(0,pTarget,iEffectNumber) < 15 && iEffectTime%(15-EffectVar(0,pTarget,iEffectNumber)))
    return();
  var phase = pTarget->GetPhase();
  phase += Max(1,EffectVar(0,pTarget,iEffectNumber)/15)*EffectVar(1,pTarget,iEffectNumber);
  if(phase > 5) {
    EffectVar(1,pTarget,iEffectNumber) = -1;
    phase = 5-phase%5; 
  }
  if(phase < 0) {
    EffectVar(1,pTarget,iEffectNumber) = 1;
    phase = phase%5; 
  }
  pTarget->SetPhase(phase);
  if(--EffectVar(0,pTarget,iEffectNumber) <= 0)
    return(-1);
}

func Hit()
{
  // Bumpf
  Sound("ClonkHit*");
}

public func IsCraneGrabable() { return(1); }
