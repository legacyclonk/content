/*-- Sichtbare Fallen --*/

#strict

protected func Initialize()
{
  var iFind = FindObjects(Find_OCF(OCF_CrewMember|OCF_Alive),Find_Not(Find_Func("IsTrapper")));
  for(var iObj in iFind)
    if(!GetEffect("SeekTraps",iObj)) AddEffect("SeekTraps",iObj,1,3,0,ST4V);
  return(1);
}

protected func Activate(int iByPlayer)
{
  MessageWindow(GetDesc(), iByPlayer);
  return(1);
}

private func Destruction()
{
  var iFind = FindObjects(Find_OCF(OCF_CrewMember|OCF_Alive),Find_Not(Find_Func("IsTrapper")));
  for(var iObj in iFind)
    if(GetEffect("SeekTraps",iObj)) RemoveEffect("SeekTraps",iObj);
  return(1);
}

public func FxSeekTrapsStart(object pTarget, int iEffectNumber, int iTemp) { return(1); }

public func FxSeekTrapsTimer(object pTarget, int iEffectNumber, int iTime)
{
  if(!GetAlive(pTarget)) return(0);
  var i, iR, iTim;
  var iTraps = FindObjects(Find_Func("IsTrap"),Find_Func("IsArmed"),Find_Distance(pTarget->~TrapSeeRange()/2,GetX(pTarget),GetY(pTarget)),Find_NoContainer());
  for(var iObj in iTraps) {
    if(!(iObj->~MakeVisibleTo(GetOwner(pTarget),pTarget))) continue;
    iTim = CreateObject(TIM1,AbsX(GetX(iObj)),AbsY(GetY(iObj)),GetOwner(pTarget));
    SetVisibility(VIS_Owner,iTim);
    SetCategory(GetCategory(iTim)|8388608,iTim);
    Schedule("RemoveObject();",15,0,iTim);
    for(i = 0; i < 15; i++) {
      iR = Random(360);
      CreateParticle("NoGravSpark",AbsX(GetX(iObj)+Cos(iR,15)),AbsY(GetY(iObj)+Sin(iR,15)),0,0,30,RGBa(250,150,150,100),iTim);
    }
  }
  return(1);
}

public func FxSeekTrapsStop(object pTarget, int iEffectNumber, int iReason, bool fTemp)
{
  if(fTemp) return(0);
  if(iReason == 4) return(-1);
}