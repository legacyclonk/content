/* Heiltrank */

#strict

protected func Hit() { Sound("Crystal*"); }

public func IsPotion() { return(1); }
public func BrewTime() { return(6*35); }

public func Intensity() { return(+80); }

public func Glow(iTime)   { return(RGB(	// ähnlicher Fade wie vom Manatrank nur: rgb->bgr
  80-Cos(iTime*2,20),			// und weniger starkes fade
  60+Sin(Cos(iTime,200),20),
  80+Cos(Sin(iTime,200),20)
  ));
}

public func Spark() { return(RGBa(255,0,0,120)); }

func Initialize() { AddEffect("Potion",this(),100,1,0,PFIR); }

protected func Activate(object pClonk) {
  // Sound
  Sound("Drink");
  // Aktion
  pClonk->~Drink(this());
  // schon voll
  if(GetEnergy(pClonk) == GetPhysical("Energy",0, pClonk)/1000)
    Schedule("PlayerMessage(GetOwner(),\"$Nothing$\",this())",80,0,pClonk);
  // Effekt zeitverzögert starten
  else 
     Schedule("AddEffect(\"HealPotion\",this(),210,1,0,PHEA,PHEA->Intensity())",80,0,pClonk);
  // fertig..
  RemoveObject();
  return(1);
}

private func FxHealPotionStart (object pTarget, int iNumber, int iTemp, int iIntensity)
{
  if(iTemp) return();
  EffectVar(0,pTarget,iNumber)=iIntensity;
}
  
private func FxHealPotionTimer(object pTarget, int iNumber, int iTime) 
{
  if(!(iTime%5))
  {
    if(!--EffectVar(0,pTarget,iNumber)) return(-1);
    var prev_energy = pTarget->GetEnergy();
    pTarget->DoEnergy(+1);
    var after_energy = pTarget->GetEnergy();
    if(GetEnergy(pTarget) == GetPhysical("Energy",0, pTarget)/1000) return(FX_Execute_Kill);
    // Antiheilfluch: Todesheilung bei 1HP abbrechen
    if (after_energy < prev_energy && after_energy <=1) return(FX_Execute_Kill);
  }
  if(!Contained(pTarget))
  {  
    var iX, iY, iAng;
    for(var i=2; i>0; --i)
    {
      iAng=i*180+iTime%360*5;
      iX=Sin(iAng, 15);
      iY=Cos(iAng, 15);
      CreateParticle("NoGravSpark", GetX(pTarget)+iX, GetY(pTarget)+iY, iX/10+RandomX(-1,1)+Sin(iAng+90, 2), iY/10+RandomX(-1,1)+Cos(iAng+90, 2), 25+Random(15), PHEA->Glow(iTime),pTarget);
    }
  }    
  //pClonk->DoMagicEnergy(Min(PMAN->Intensity(),GetPhysical("Magic",0)/1000-GetMagicEnergy()));
}

private func FxHealPotionEffect(string szNewEffectName)
{
  // gleichen Effekt zusammenfassen (Trank zweimal hintereinander getrunken)
  if(szNewEffectName eq "HealPotion") return(-2);
}

private func FxHealPotionAdd(object pTarget, int iNumber, string szNewEffectName, int iNewEffectTimer, int iTime) {
  // Effekt übernehmen
  EffectVar(0,pTarget,iNumber) += iTime;
}  
