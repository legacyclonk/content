/* Monstertrank */

#strict

protected func Hit() { Sound("Crystal*"); }

public func IsPotion() { return(1); }
public func BrewTime() { return(6*35); }

public func EffectDuration() { return(34*35); } // ~34s

public func Physical(i) {
  if(i==0) return("Fight");
  if(i==1) return("Energy");
  if(i==2) return("Push");
}

public func Glow(iTime)   { return(RGB(
  80,
  80+Cos(iTime,20),
  80
  ));
}

public func Spark() { return(RGBa(127,255,0,120)); }

public func Light(iTime) { return(RGBa(127+Random(2)*64,255,0,RandomX(140,240))); }
public func LightSpeed() { return(3); }
public func LightSize()  { return(120); }

func Initialize() { AddEffect("Potion",this(),100,1,0,PFIR); }


protected func Activate(object pClonk) {
  // Sound
  Sound("Drink");
  // Aktion
  pClonk->~Drink(this());
  // Effekt starten (zeitverzögert)
  Schedule("AddEffect(\"MonsterPotion\",this(),160,35,0,PMON,90000,70000,100000,PMON->EffectDuration())",80,0,pClonk);
  // fertig..
  RemoveObject();
  return(1);
}

// EffectVars: alle ungeraden - Änderung (1,3,5)
//             9 - EffectTime

protected func FxMonsterPotionStart(object pTarget, int iEffectNumber, int iTemp, int iChange0, int iChange1, int iChange2, int iTime)
{
  if(!iTemp) {
    EffectVar(9,pTarget,iEffectNumber) = iTime;
    AddEffect("Obj3D",pTarget,300,1,0,PFIR,PMON);
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
  if(iTemp) return();
  SetClrModulation(RGB(128,255,128), pTarget);
}

protected func FxMonsterPotionTimer(object pTarget, int iNumber, int iTime) {
  // Zeit abgelaufen!
  if(iTime >= EffectVar(9,pTarget,iNumber)) return(-1);
}

private func FxMonsterPotionEffect(string szNewEffectName) { if(szNewEffectName eq "MonsterPotion") return(-2); }

private func FxMonsterPotionAdd(object pTarget, int iNumber, string szNewEffectName, int iNewEffectTimer,int iChange0, int iChange1, int iChange2, int iTime) {
  // Effekt übernehmen
  EffectVar(9,pTarget,iNumber) += iTime;
}

protected func FxMonsterPotionStop(object pTarget, int iEffectNumber, int iReason, bool fTemp)
{
  // Physicals wiederherstellen
  for(var i=0; i<3; ++i) if(Physical(i))
    ResetPhysical(pTarget, Physical(i));
  if(fTemp) return();

  for(var num,i=0; num=GetEffect("Obj3D",pTarget,i); ++i)
    if(EffectVar(0,pTarget,num) == PMON)
      { RemoveEffect(0,pTarget,num); break; }

  SetClrModulation(RGB(255,255,255), pTarget);
}
