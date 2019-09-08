/* Munitionsschachtel */

#strict

#appendto AMBO

global func FxDisappearStart(object pTarget, int iNumber, bool fTmp, int iTime)
{
  if(fTmp) return();
  EffectVar(1, pTarget, iNumber) = iTime;
}

global func FxDisappearTimer(pTarget, iNumber)
{
  if(Contained(pTarget)) return(EffectVar(0, pTarget, iNumber)=0, SetClrModulation(0, pTarget));
  EffectVar(0, pTarget, iNumber)++;
  SetClrModulation(RGBa(255,255,255, EffectVar(0, pTarget, iNumber)*255/EffectVar(1, pTarget, iNumber)), pTarget);
  if(EffectVar(0, pTarget, iNumber)>EffectVar(1, pTarget, iNumber)) return(RemoveObject(pTarget));
}

protected func Initialize()
{
  AddEffect("Disappear", this(), 1, 1, 0, 0, 1000);
  ScheduleCall(this(), "Initialized", 2);
}

public func Initialized()
{
  if(Contained()) return();
  if(GetY()>0) return();	
  var pBallon = CreateObject(BALO);
  AddEffect("Ballon", this(), 1, 1, 0, GetID(), pBallon);
}

func FxBallonStart(pTarget, iNumber, fTmp, pBallon)
{
  if(fTmp) return();
  EffectVar(0, pTarget, iNumber) = pBallon;
}

func FxBallonTimer(pTarget, iNumber)
{
  if(!EffectVar(0, pTarget, iNumber)) return(-1);
  SetPosition(GetX(EffectVar(0, pTarget, iNumber)), GetY(EffectVar(0, pTarget, iNumber))+20, pTarget);
  SetYDir(0, pTarget);
}
