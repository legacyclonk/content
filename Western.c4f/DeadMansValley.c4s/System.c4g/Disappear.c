/*-- Verschwindende Revolver --*/

#strict
#appendto REVR

global func FxDisappearTimer(pTarget, iNumber)
{
  if(Contained(pTarget)) return(EffectVar(0, pTarget, iNumber)=0);
  EffectVar(0, pTarget, iNumber)++;
  if(EffectVar(0, pTarget, iNumber)>2000) return(RemoveObject(pTarget));
}

func Initialize(a,b,c,d,e,f,g,h)
{
  AddEffect("Disappear", this(), 1, 1);
  return( _inherited(a,b,c,d,e,f,g,h));
}

/* Man braucht nur einen Revolver */
public func CarryLimit ()
{
  return(1);
}
