/* Immunitätstrank */

#strict

protected func Hit() { Sound("Crystal*"); }

public func IsPotion() { return(1); }
public func BrewTime() { return(6*35); }

private func EffectDuration() { return(4*60*35); } // ~4min

public func Glow(iTime)   { return(RGBa(
  64+Sin(iTime*3/2,32),
  64+Cos(iTime*2/2,32),
  64-Cos(iTime*2/3,32),
  32-Sin(iTime*3/3,32)
  ));
}

public func Spark() { return(RGBa(255,255,255,130)); }

public func Light(iTime)   { return(RGBa(
  64+Sin(iTime*3/2,32),
  64+Cos(iTime*2/2,32),
  64-Cos(iTime*2/3,32),
  160
  ));
}
public func LightSpeed() { return(4); }
public func LightSize()  { return(40); }

func Initialize() { AddEffect("Potion",this(),100,1,0,PFIR); }

protected func Activate(object pClonk) {
  // Sound
  Sound("Drink");
  // Aktion
  pClonk->~Drink(this());
  // Effekt starten (zeitverzögert)
  Schedule("AddEffect(\"BanNegPotion\",this(),210,35,0,PIMM,PIMM->EffectDuration())",80,0,pClonk);
  // fertig..
  RemoveObject();
  return(1);
}

private func FxBanNegPotionStart (object pTarget, int iNumber, int iTemp, int iTime) {
  if(iTemp) return();

  AddEffect("Obj3D",pTarget,300,1,0,PFIR,PIMM);
  // Zeit speichern
  EffectVar(0,pTarget,iNumber)=iTime;
  // negative Effekte beseitigen
  var iEffect;
  for(var i = GetEffectCount(); i > 0; --i) {
    iEffect = GetEffect("*NSpell",pTarget,i);
    if(!iEffect) iEffect = GetEffect("*Curse*",pTarget,i);
    if(iEffect) RemoveEffect(0,pTarget,iEffect);
  }
  if(iEffect) CastParticles("PSpark", 20, 20, GetX(pTarget), GetY(pTarget), 55, 65);
}

private func FxBanNegPotionEffect(string szNewEffectName, object pTarget) {
  // keine negativen Effekte
  if(WildcardMatch(szNewEffectName,"*Curse*") || WildcardMatch(szNewEffectName,"*NSpell"))
  {
    Sound("Magic1", 0, pTarget, 50);
    CastParticles("PSpark", 20, 20, GetX(pTarget), GetY(pTarget), 55, 65);
    return(-1);
  }  
  // gleichen Effekt zusammenfassen (Trank zweimal hintereinander getrunken)
  if(szNewEffectName eq "BanNegPotion") return(-2);
}

private func FxBanNegPotionAdd(object pTarget, int iNumber, string szNewEffectName, int iNewEffectTimer, int iTime) {
  // Effekt übernehmen
  EffectVar(0,pTarget,iNumber) += iTime;
}

private func FxBanNegPotionTimer(object pTarget, int iNumber, int iTime) { 


  // kein offensichtlicher Effekt damit Gegner nicht merken dass er Immun
  // Zeit abgelaufen!
  if(iTime >= EffectVar(0,pTarget,iNumber)) return(-1);
}

private func FxBanNegPotionStop(object pTarget, int iNumber, int iReason, bool fTemp) {
  if(fTemp) return();
  for(var num,i=0; num=GetEffect("Obj3D",pTarget,i); ++i)
    if(EffectVar(0,pTarget,num) == PIMM)
      { RemoveEffect(0,pTarget,num); break; }
}
