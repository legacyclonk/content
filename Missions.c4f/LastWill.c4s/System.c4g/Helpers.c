/* Allgemeine Hilfsfunktionen */

#strict 2

/* Unverwundbarkeit */

global func SetInvincible(object obj)
  {
  if (!obj) if (!(obj = this)) return 0;
  return AddEffect("IntInvincibility", obj, 200, 0, obj);
  }
  
global func SetVincible(object obj)
  {
  if (!obj) if (!(obj = this)) return 0;
  return RemoveEffect("IntInvincibility", obj);
  }

global func FxIntInvincibilityDamage(object obj, int iEff, int iDmg, int iCause, int iCausePlr) { }

global func FxIntInvincibilityEffect(string szNewEffectName)
  {
  // nicht anbrennen lassen
  if(WildcardMatch(szNewEffectName,"*Fire*")) return FX_Effect_Deny;
  // gleichen Effekt ablehnen
  if(szNewEffectName == "IntInvincibility") return FX_Effect_Deny;
  }
  