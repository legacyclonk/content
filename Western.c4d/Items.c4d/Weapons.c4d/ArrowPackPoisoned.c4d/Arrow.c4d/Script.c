/*-- Giftpfeil --*/

#strict

#include ARRW

/* Pfeileigenschaften */
public func PackTo() { return(TARP); }
public func ArrowStrength() { return(2+Random(2)); }
public func ArrowTumbleRandom() { return(3); }

public func InFlight()
{
  CreateParticle("MSpark",0,0,0,0,20,RGB(0,255,0));
  return(_inherited(...));
}

/* Treffer */
public func HitTarget(pObject)
{
  if(!_inherited(pObject)) return(false);

  // Vergiften
  if(GetAlive(pObject))
    AddEffect("Poison",pObject,182,10,0,GetID(),GetController(),600);
  ChangeDef(ARWI);
  return(1);
}

/* Vergiftung */

public func FxPoisonStart(object pClonk, int iEffectNumber, int iTemp, int iPlr, int iTime)
{
  // temporäre Effekte ignorieren
  if(iTemp) return(false);
  var idType = GetID(pClonk);
  if(GetDefHeight(idType) > GetDefWidth(idType)) EffectVar(0,pClonk,iEffectNumber) = GetDefHeight(idType);
  else EffectVar(0,pClonk,iEffectNumber) = GetDefWidth(idType);
  EffectVar(5,pClonk,iEffectNumber) = iPlr+1;
  if(!iTime) iTime = 600;
  EffectVar(6,pClonk,iEffectNumber) = iTime;
}

public func FxPoisonTimer(object pClonk, int iEffectNumber, int iEffectTime)
{
  if(iEffectTime >= EffectVar(6,pClonk,iEffectNumber)) return(-1);
  if(GBackLiquid(GetX(pClonk),GetY(pClonk))) return(-1);

  CreateParticle("PoisonWave",GetX(pClonk),GetY(pClonk),0,-1,EffectVar(0,pClonk,iEffectNumber)*5+50,RGBa(0,255,0),pClonk,1);
  CreateParticle("PSpark",    GetX(pClonk),GetY(pClonk),0,-1,EffectVar(0,pClonk,iEffectNumber)*5+50,RGBa(0, 55,0),pClonk,1);
         
  if(!Random(2))
    DoEnergy(-1,pClonk,0,38,EffectVar(5,pClonk,iEffectNumber));
  return(1);
}

public func FxPoisonStop(object pClonk, int iEffectNumber, int iReason, bool fTemp)
{
  if(fTemp) return(false);
  return(1);
}

public func FxPoisonEffect(szNewEffectName, object pClonk, int iEffectNumber, int iNewEffectNumber, int iPlr, int iTime)
{
  // Wenn ein weiterer Effekt hinzukommt, stellt sich die Dauer wieder zurück auf den Anfang
  if(szNewEffectName eq "Poison") {
    EffectVar(5,pClonk,iEffectNumber) = iPlr+1;
    if(!iTime) iTime = 600;
    EffectVar(6,pClonk,iEffectNumber) = GetEffect(0,pClonk,iEffectNumber,6)+iTime;
    return(-1);
  }
}

/* Kann vom Indianer in Handarbeit hergestellt werden */
public func IsIndianHandcraft() { return(1); }