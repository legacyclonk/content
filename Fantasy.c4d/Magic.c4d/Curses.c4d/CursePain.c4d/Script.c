/* Schmerzensfluch */

#strict
#include CAHE // Fluch

public func GetCurseName() { return "Pain"; }

protected func FxCursePainStart(pClonk, iEffectNumber, iTemp, iX, iY, pCaller)
{
  return FxCurseStart(pClonk, iEffectNumber, iTemp, iX, iY, pCaller);
}

protected func FxCursePainTimer(pClonk, iEffectNumber, iEffectTime)
{
  return FxCurseTimer(pClonk, iEffectNumber, iEffectTime);
}

protected func FxCursePainInfo(pClonk, iEffectNumber)
{
  if (FxCurseIsActive(pClonk, iEffectNumber)) return ("{{CPAN}} $SpellInfo$");
}

func FxCursePainDamage(pClonk, iEffectNumber, iDmgEngy, iCause)
{
  // Schaden verstärken	
  if (!FxCurseIsActive(pClonk, iEffectNumber)) return iDmgEngy;
  if(iDmgEngy>0) return iDmgEngy;
  CastParticles("FSpark", -iDmgEngy/250, 50, GetX(pClonk), GetY(pClonk), 50, 70, RGBa(255, 0, 0, 50), RGBa(200, 0, 0, 75));
  return(iDmgEngy*5/4);
}

/* Zaubercombo */

public func GetSpellClass() { return(WATR); }
public func GetSpellCombo() { return ("424"); } // (1: Backward; 2: Down; 3: Forward; 4: Throw; 5: Jump; 6: Dig)
