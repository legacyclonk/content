/* Wipftrank */

#strict

protected func Hit() { Sound("Crystal*"); }

public func IsPotion() { return(1); }
public func BrewTime() { return(6*35); }

public func EffectDuration() { return(18*35); } // ~18s

public func Physical(i) {
  if(i==0) return("Walk");
  if(i==1) return("Jump");
  if(i==2) return("Dig");
}

public func Glow(iTime)   { return(RGB(
  80+Cos(iTime,15),
  80+Cos(iTime,15),
  80
  ));
}

public func Spark() { return(RGBa(196,196,127,120)); }

public func Light(iTime) { return(RGBa(RandomX(56,176),RandomX(56,100),0,120)); }
public func LightSpeed() { return(14); }
public func LightSize()  { return(60); }

func Initialize() { AddEffect("Potion",this(),100,1,0,PFIR); }

protected func Activate(object pClonk) {
  // Sound
  Sound("Drink");
  // Aktion
  pClonk->~Drink(this());
  // Effekt starten (zeitverzögert)
  Schedule("AddEffect(\"WipfPotion\",this(),160,1,0,PWIP,30000,18000,50000,PWIP->EffectDuration())",80,0,pClonk);
  // fertig..
  RemoveObject();
  return(1);
}

// EffectVars: alle ungeraden - Änderung (1,3,5)
//             9 - EffectTime

protected func FxWipfPotionStart(object pTarget, int iEffectNumber, int iTemp, int iChange0, int iChange1, int iChange2, int iTime)
{
  if(!iTemp) {
    EffectVar(9,pTarget,iEffectNumber) = iTime;
    AddEffect("Obj3D",pTarget,300,1,0,PFIR,PWIP);
  }

  var i, iOld, iChange;
  // Für nichttemporäre Aufrufe wird iChangeX übergeben, und auf den Änderungswert aufgerechnet
  // Die Änderung kann in temporären Aufrufen auch ungleich iChange sein
  for(i=0; i<3; ++i) if(Physical(i))
  {
    iOld = GetPhysical(Physical(i),0,pTarget);
    iChange = (EffectVar(i*2+1,pTarget,iEffectNumber)) += Par(3+i);
    SetPhysical(Physical(i),iOld+iChange,PHYS_StackTemporary,pTarget);
  }
}

protected func FxWipfPotionTimer(object pTarget, int iNumber, int iTime) {
  if(!Contained(pTarget))
  {
    var iRndX=RandomX(-9,9);
    CreateParticle("NoGravSpark", GetX(pTarget)+iRndX, GetY(pTarget)+RandomX(8,10), -iRndX/4, -Random(10), 20, RGBa(Random(56)+120,Random(56)+50, 0, Random(150)),pTarget);
  }  
  // Zeit abgelaufen!
  if(iTime >= EffectVar(9,pTarget,iNumber)) return(-1);
}

private func FxWipfPotionEffect(string szNewEffectName) { if(szNewEffectName eq "WipfPotion") return(-2); }

private func FxWipfPotionAdd(object pTarget, int iNumber, string szNewEffectName, int iNewEffectTimer, int iChange0, int iChange1, int iChange2, int iTime) {
  // Effekt übernehmen
  EffectVar(9,pTarget,iNumber) += iTime;
}

protected func FxWipfPotionStop(object pTarget, int iEffectNumber, int iReason, bool fTemp)
{
  // Physicals wiederherstellen
  for(var i=0; i<3; ++i) if(Physical(i))
    ResetPhysical(pTarget, Physical(i));

  if(fTemp) return();

  for(var num,i=0; num=GetEffect("Obj3D",pTarget,i); ++i)
    if(EffectVar(0,pTarget,num) == PWIP)
      { RemoveEffect(0,pTarget,num); break; }
}
