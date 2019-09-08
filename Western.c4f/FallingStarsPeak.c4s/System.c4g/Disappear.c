/* Hilfsfunktionen */

#strict

#appendto WINC
#appendto REVR

protected func Initialize()
{
  AddEffect("KeepInside", this(), 1, 1, this());
}

protected func RejectEntrance(pNewContainer)
{
  var idID = GetID(pNewContainer);
  if(idID != COWB && idID != BNDT) return(1);
  return(0);
}

func FxKeepInsideStart(pTarget, iNumber, fTmp)
{
  if(fTmp) return();
  EffectVar(0, pTarget, iNumber) = Contained();
}

func FxKeepInsideTimer(pTarget, iNumber, fTmp)
{
  if(!EffectVar(0, pTarget, iNumber)) EffectVar(0, pTarget, iNumber) = Contained();
  if(Contained()) return();
  if(EffectVar(0, pTarget, iNumber))
    if(GetAlive(EffectVar(0, pTarget, iNumber)))
      return(Enter(EffectVar(0, pTarget, iNumber)));
  RemoveObject(pTarget);
}
