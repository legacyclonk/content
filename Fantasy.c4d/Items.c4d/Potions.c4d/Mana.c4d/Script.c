/* Manatrank */

#strict

protected func Hit() { Sound("Crystal*"); }

public func IsPotion() { return(1); }
public func BrewTime() { return(6*35); }

public func Intensity() { return(+40); }

public func Glow(iTime)   { return(RGB(
  80+Cos(Sin(iTime,200),35),
  60+Sin(Cos(iTime,300),25),
  80-Cos(iTime*2,30)
  ));
}

public func Spark() { return(RGBa(0,64+Random(3)*64,255,110)); }

func Initialize() { AddEffect("Potion",this(),100,1,0,PFIR); }

protected func Activate(object pClonk) {
  // Sound
  Sound("Drink");
  // Aktion
  pClonk->~Drink(this());
  // keine magische Energie vorhanden
  if(!GetPhysical("Magic",0,pClonk) || !DoMagicEnergy(+1,pClonk))
    Schedule("PlayerMessage(GetOwner(),\"$Nothing$\",this())",80,0,pClonk);
  // Effekt zeitverzögert starten
  else 
    Schedule("AddEffect(\"ManaPotion\",this(),210,1,0,PMAN,PMAN->Intensity())",80,0,pClonk);
  DoMagicEnergy(-1,pClonk);
  // fertig..
  RemoveObject();
  return(1);
}

private func FxManaPotionStart (object pTarget, int iNumber, int iTemp, int iIntensity)
{
  if(iTemp) return();
  EffectVar(0,pTarget,iNumber)=iIntensity;
}
  
private func FxManaPotionTimer(object pTarget, int iNumber, int iTime) 
{
  if(!(iTime%5))
  {
    if(!--EffectVar(0,pTarget,iNumber)) return(-1);
    if(!(pTarget->DoMagicEnergy(+1))) return(-1);
  }  
  var iX, iY;
  if(!Contained(pTarget))
  {
    for(var i=12; i>0; --i)
    {
      var iAng=Random(360);
      iX=Sin(iAng, 15);
      iY=Cos(iAng, 15);
      CreateParticle("PSpark", GetX(pTarget)+iX, GetY(pTarget)+iY, -iX/7+RandomX(-3,3), -iY/7+RandomX(-3,3), 25+Random(15), PMAN->Glow(iTime)|20<<24, pTarget);
    }
  }  
  return(1);
  //pClonk->DoMagicEnergy(Min(PMAN->Intensity(),GetPhysical("Magic",0)/1000-GetMagicEnergy()));
}

private func FxManaPotionEffect(string szNewEffectName)
{
  // gleichen Effekt zusammenfassen (Trank zweimal hintereinander getrunken)
  if(szNewEffectName eq "ManaPotion") return(-2);
}

private func FxManaPotionAdd(object pTarget, int iNumber, string szNewEffectName, int iNewEffectTimer, int iTime) {
  // Effekt übernehmen
  EffectVar(0,pTarget,iNumber) += iTime;
}    
