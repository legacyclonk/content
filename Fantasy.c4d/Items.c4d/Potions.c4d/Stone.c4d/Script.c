/* Steintrank */

#strict

protected func Hit() { Sound("Crystal*"); }

public func IsPotion() { return(1); }
public func BrewTime() { return(6*35); }

public func EffectDuration() { return(30*35); } // ~30s

public func Glow(iTime)   { return(RGB(
  70+Cos(iTime,20),
  70+Cos(iTime,20),
  70+Cos(iTime,20)
  ));
}

public func Spark() { return(RGBa(196,196,196,120)); }

public func Light(iTime) { return(RGBa(127,95,20,100)); }
public func LightSpeed() { return(2); }
public func LightSize()  { return(100); }

func Initialize() { AddEffect("Potion",this(),100,1,0,PFIR); }

protected func Activate(object pClonk) {
  // Sound
  Sound("Drink");
  // Aktion
  pClonk->~Drink(this());
  // Effekt starten (zeitverzögert)
  Schedule("AddEffect(\"BlowResistancePotion\",this(),160,1,0,PSTO,PSTO->EffectDuration())",80,0,pClonk);
  // fertig..
  RemoveObject();
  return(1);
}

private func FxBlowResistancePotionStart (object pTarget, int iNumber, int iTemp, int iTime) { 
  if(iTemp) return();

  AddEffect("Obj3D",pTarget,300,1,0,PFIR,PSTO);

  // Zeit speichern
  EffectVar(0,pTarget,iNumber)=iTime;
}

private func FxBlowResistancePotionDamage(object pTarget, int iEffectNumber, int iDmgEngy, int iCause) {

  if(iCause != 34 && iCause != 40 && iCause != 33) return(iDmgEngy);

  CreateParticle("MSpark", GetX(pTarget)+RandomX(-8,8),GetY(pTarget)+RandomX(-10,10), 0, 0, 40, RGB(128,128,128));

  // Objekttreffer: drittel
  if(iCause == 34) return(iDmgEngy/3);
  // Clonk-zu-Clonk-Kampf: halb
  if(iCause == 40) return(iDmgEngy/2);
  // Explosion: 2/3
  if(iCause == 33) return(iDmgEngy*2/3);
}

private func FxBlowResistancePotionEffect(string szNewEffectName) { if(szNewEffectName eq "BlowResistancePotion") return(-2); }

private func FxBlowResistancePotionAdd(object pTarget, int iNumber, string szNewEffectName, int iNewEffectTimer, int iTime) {
  // Effekt übernehmen
  EffectVar(0,pTarget,iNumber) += iTime;
}

private func FxBlowResistancePotionTimer(object pTarget, int iNumber, int iTime) {
  // Zeit abgelaufen!
  if(iTime >= EffectVar(0,pTarget,iNumber)) return(-1);
  
}

private func FxBlowResistancePotionStop(object pTarget, int iEffectNumber, int iReason, bool fTemp) {
  if(fTemp) return();

  for(var num,i=0; num=GetEffect("Obj3D",pTarget,i); ++i)
    if(EffectVar(0,pTarget,num) == PSTO)
      { RemoveEffect(0,pTarget,num); break; }
}