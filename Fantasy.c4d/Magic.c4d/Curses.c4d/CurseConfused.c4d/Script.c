/* Verwirrungsfluch */

#strict

#include CAHE // Fluch

public func GetCurseName() { return "ControlConfusion"; }

protected func FxCurseControlConfusionStart(pClonk, iEffectNumber, iTemp, iX, iY, pCaller)
{
  return FxCurseStart(pClonk, iEffectNumber, iTemp, iX, iY, pCaller);
}

protected func FxCurseControlConfusionTimer(pClonk, iEffectNumber, iEffectTime)
{
  return FxCurseTimer(pClonk, iEffectNumber, iEffectTime);
}

protected func FxCurseControlConfusionInfo(pClonk, iEffectNumber)
{
  if (FxCurseIsActive(pClonk, iEffectNumber)) return ("{{CCNF}} $SpellInfo$");
} 

public func FxCurseControlConfusionControlRight(pClonk, iEffectNumber)
{
  if (!FxCurseIsActive(pClonk, iEffectNumber)) return;
  if(Contained(pClonk) || pClonk->IsRiding())
  {
    EffectVar(2, pClonk, iEffectNumber) = 1;
    var res = pClonk->ControlLeft();
    EffectVar(2, pClonk, iEffectNumber) = 0;
    return(res);
  }
  if(GetProcedure(pClonk)S="SCALE")
  {
    if(GetDir(pClonk)==1) { SetXDir(-10, pClonk); ObjectSetAction(pClonk, "Jump"); SetPosition(GetX(pClonk)-2, GetY(pClonk), pClonk); }
    else SetComDir(COMD_Stop, pClonk);
    return(1);	  
  }
  if(GetProcedure(pClonk)S="DIG")
  {
    SetComDir(GetComDir(pClonk)+1, pClonk); 	  
    return(1);	  
  }
  SetComDir(COMD_Left, pClonk);
  return(1);
}

public func FxCurseControlConfusionControlLeft(pClonk, iEffectNumber)
{
  if (!FxCurseIsActive(pClonk, iEffectNumber)) return;
  if(Contained(pClonk) || pClonk->IsRiding())
  {
    EffectVar(2, pClonk, iEffectNumber) = 1;
    var res = pClonk->ControlRight();
    EffectVar(2, pClonk, iEffectNumber) = 0;
    return(res);
  }
  if(GetProcedure(pClonk)S="SCALE")
  {
    if(GetDir(pClonk)==0) { SetXDir(+10, pClonk); ObjectSetAction(pClonk, "Jump"); SetPosition(GetX(pClonk)+2, GetY(pClonk), pClonk); }
    else SetComDir(COMD_Stop, pClonk);
    return(1);	  
  }
  if(GetProcedure(pClonk)S="DIG")
  {
    SetComDir(GetComDir(pClonk)-1, pClonk); 	  
    return(1);	  
  }
  SetComDir(COMD_Right, pClonk);
  return(1);
}

/* Zaubercombo */

public func GetSpellClass() { return(WATR); }
public func GetSpellCombo() { return ("451"); } // (1: Backward; 2: Down; 3: Forward; 4: Throw; 5: Jump; 6: Dig)
