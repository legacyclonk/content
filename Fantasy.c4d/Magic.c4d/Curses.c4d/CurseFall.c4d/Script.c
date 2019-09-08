/* Stolperfulch */

#strict

#include CAHE // Fluch

public func GetCurseName() { return "Falling"; }

protected func FxCurseFallingStart(pClonk, iEffectNumber, iTemp, iX, iY, pCaller)
{
  return FxCurseStart(pClonk, iEffectNumber, iTemp, iX, iY, pCaller);
}

protected func FxCurseFallingTimer(pClonk, iEffectNumber, iEffectTime)
{
  var rval = FxCurseTimer(pClonk, iEffectNumber, iEffectTime);
  if (rval != FX_OK) return rval;
  if (FxCurseIsActive(pClonk, iEffectNumber))
  {
    // Bei Bedarf stolpern
    if(pClonk->GetContact(0, -1, 8))
    {
      if(GetXDir(pClonk) || !(GetAction(pClonk)S="Walk") )
      {
        if(WildcardMatch(GetAction(pClonk), "*Magic") || WildcardMatch(GetAction(pClonk), "*Cast"))
        { 
          // Bei Magie-Aktion nicht so häufig stören	  
          if(!Random(120)) { ObjectSetAction(pClonk, "FlatUp"); SetXDir(0, pClonk); }
        }
        else
        {
          if(!Random(60)) { ObjectSetAction(pClonk, "KneelDown"); SetXDir(0, pClonk); }
          if(!Random(60)) { ObjectSetAction(pClonk, "FlatUp"); SetXDir(0, pClonk); }
        }
      }
    }
    // In Flug nur Tumble
    else
    {
      if(!Random(50)) { ObjectSetAction(pClonk, "Tumble"); }	  
    }
  }
  return FX_OK;
}

protected func FxCurseFallingInfo(pClonk, iEffectNumber)
{
  if (FxCurseIsActive(pClonk, iEffectNumber)) return ("{{CFAL}} $SpellInfo$");
} 

/* Zaubercombo */

public func GetSpellClass() { return(WATR); }
public func GetSpellCombo() { return ("462"); } // (1: Backward; 2: Down; 3: Forward; 4: Throw; 5: Jump; 6: Dig)
