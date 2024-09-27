/*-- AmmoPack --*/

#strict

public func IsBonus() { return(true); }

func Color() { return(RGB(90,90,90)); }

func Activate(object obj) {
  var i,id;
  while(id = GetDefinition(i++))
    DoAmmo(id,DefinitionCall(id,"MaxAmmo")/5,obj);
  
  AddEffect("AmmoBonus",obj,1,5);
  Sound("AmmoPack", 1, 0,0, GetOwner(obj)+1);

  return(true);
}

global func FxAmmoBonusTimer(object pTarget, int iEffectNumber, int iEffectTime) {
  CreateParticle("XSpark",GetX(pTarget)+Sin(EffectVar(0,pTarget,iEffectNumber)*60,12),GetY(pTarget)+Cos(EffectVar(0,pTarget,iEffectNumber)*60,12),0,0,40,RGB(90,90,90),pTarget);
  if(++EffectVar(0,pTarget,iEffectNumber) > 6) {
    CastParticles("XSpark",5,15,GetX(pTarget),GetY(pTarget),40,60,RGB(90,90,90),0,pTarget);
    return(-1);
  }
}
