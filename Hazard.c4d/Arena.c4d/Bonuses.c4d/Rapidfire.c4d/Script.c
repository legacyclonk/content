/*-- Rapid Fire --*/

#strict

public func IsBonus() { return(true); }

public func Color() { return(RGB(0,255,0)); }

public func Activate(object obj) {
  if(!AddEffect("RapidBonus",obj,190,3,obj))
    return(0);
  Sound("Rapidfire", 1, 0,0, GetOwner(obj)+1);
  return(1);
}

global func FxRapidBonusTimer(object pTarget, int iEffectNumber, int iEffectTime) {
  var bar = EffectVar(0,pTarget,iEffectNumber);
  var foo = (12*bar)/180,
      off=4,
      v = 90 < bar && bar < 270;
  if(bar > 180) {
    foo = -(13-foo);
    off=-7;
  }
  CreateParticle("PSpark",Sin(bar,12),off+Cos(bar,foo),0,0,35,RGBa(50,255,50,20),pTarget,v);
  CreateParticle("PSpark",Sin(bar,-12),(off+Cos(bar,foo)),0,0,35,RGBa(50,255,50,20),pTarget,v);

  EffectVar(0,pTarget,iEffectNumber) += 15;
  if(EffectVar(0,pTarget,iEffectNumber) > 360)
    EffectVar(0,pTarget,iEffectNumber) = 0;
  
  if(iEffectTime > BONUS_Duration)
    return(-1);
}

global func FxRapidBonusEffect(string szNewEffectName, object pTarget, int iEffectNumber, int iNewEffectNumber) {
  if(szNewEffectName S= "RapidBonus")
  {
    return(-1);
  }
}

global func FxRapidBonusFMData(object pTarget, int iEffectNumber, int iData, value) {
  //Log("Rapidfire");
  if(iData == FM_Recharge) return(Max(1,value/2));
  if(iData == FM_Reload) return(Max(1,value/2));
  return(value);
}
