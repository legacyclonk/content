/* Fischtrank */

#strict

protected func Hit() { Sound("Crystal*"); }

public func IsPotion() { return(1); }
public func BrewTime() { return(6*35); }

public func EffectDuration() { return(40*35); } // ~35s

public func Physical(i) {
  if(i==0) return("Swim");
  if(i==1) return("Breath");
}

public func Glow(iTime)   { return(RGB(
  127,
  127,
  160+Sin(iTime,40)
  ));
}

public func Spark() { return(RGBa(Random(3)*64,64,255,110)); }

public func Light(iTime) { return(RGBa(Random(3)*64,64,255,100)); }
public func LightSpeed() { return(5); }
public func LightSize()  { return(120); }

func Initialize() { AddEffect("Potion",this(),100,1,0,PFIR); }

protected func Activate(object pClonk) {
  // Sound
  Sound("Drink");
  // Aktion
  pClonk->~Drink(this());
  // Effekt starten (zeitverzögert)
  Schedule("AddEffect(\"FishPotion\",this(),160,35,0,PFIS,100000,60000,0,PFIS->EffectDuration())",80,0,pClonk);
  // fertig..
  RemoveObject();
  return(1);
}

// EffectVars: alle ungeraden - Änderung (1,3,5)
//             9 - EffectTime
protected func FxFishPotionStart(object pTarget, int iEffectNumber, int iTemp, int iChange0, int iChange1, int iChange2, int iTime)
{
  if(!iTemp) {
    EffectVar(9,pTarget,iEffectNumber) = iTime;
    AddEffect("Obj3D",pTarget,300,1,0,PFIR,PFIS);
  }

  var i, iOld, iChange;
  // Für nichttemporäre Aufrufe wird iChangeX übergeben, und auf den Änderungswert aufgerechnet
  // Die Änderung kann in temporären Aufrufen auch ungleich iChange sein
  for(i=0; i<3; ++i) if(Physical(i))
  {
    iOld = GetPhysical(Physical(i),0,pTarget);
    iChange = EffectVar(i*2+1,pTarget,iEffectNumber) += Par(3+i);
    SetPhysical(Physical(i),iOld+iChange,PHYS_StackTemporary,pTarget);
  }
}

protected func FxFishPotionTimer(object pTarget, int iNumber, int iTime) {
  // Zeit abgelaufen!
  if(iTime >= EffectVar(9,pTarget,iNumber)) return(-1);
}

private func FxFishPotionEffect(string szNewEffectName) { if(szNewEffectName eq "FishPotion") return(-2); }

private func FxFishPotionAdd(object pTarget, int iNumber, string szNewEffectName, int iNewEffectTimer, int iChange0, int iChange1, int iChange2, int iTime) {
  // Effekt übernehmen
  EffectVar(9,pTarget,iNumber) += iTime;
}

protected func FxFishPotionStop(object pTarget, int iEffectNumber, int iReason, bool fTemp)
{
  // Physicals wiederherstellen
  for(var i=0; i<3; ++i) if(Physical(i))
    ResetPhysical(pTarget, Physical(i));

  if(fTemp) return();
  for(var num,i=0; num=GetEffect("Obj3D",pTarget,i); ++i)
    if(EffectVar(0,pTarget,num) == PFIS)
      { RemoveEffect(0,pTarget,num); break; }
}