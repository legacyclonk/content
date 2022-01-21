/*-- Medipack --*/

#strict

func Activate(object pByObj) {
  if(pByObj->GetEnergy() >= pByObj->GetPhysical("Energy") / 1000) return(0);

  if(GetEffect("*Heal*", pByObj)) return(0);

  var healrate = HealRate();
  if(!ObjectSetAction(pByObj, "Heal")) healrate *= 4;

  AddEffect("MedipackHeal",pByObj,200,1,0,GetID(),HealAmount(),healrate);
  Sound("Medi");
  RemoveObject();
  return(1);
}

func HealRate() { return(3); }
func HealAmount() { return(100); }

func FxMedipackHealStart(object pTarget, int iEffectNumber, int iTemp, int iHealAmount, int iHealRate) {
  if(iTemp)
    return();

  EffectVar(0,pTarget,iEffectNumber) = iHealAmount; //Heilung Insgesamt
  EffectVar(1,pTarget,iEffectNumber) = iHealRate; //1 Punkt Heilung alle iHealRate-Frames
}

func FxMedipackHealTimer(object pTarget, int iEffectNumber, int iEffectTime) {
  // Abbruch
  if(pTarget->GetAction() ne "Heal") return(-1);

//Log("*heil*");
//Effekt
  var width= GetDefCoreVal("Width",0,GetID(pTarget)),
      height = GetDefCoreVal("Height",0,GetID(pTarget));
  width -= width/3;
  height -= height/3;
  width /= 2;
  height /= 2;
  CreateParticle("PxSpark",GetX(pTarget)+RandomX(-width, width),GetY(pTarget)+RandomX(-height,height),0,-10,Random(25)+10,RGBa(255,0,0,50));
//*Heile mach*
  if(!(iEffectTime % EffectVar(1,pTarget,iEffectNumber)))
  {
    DoEnergy(1,pTarget);
    EffectVar(0,pTarget,iEffectNumber)--;
  }
  //Schon voll geheilt?
  if(GetEnergy(pTarget) >= GetPhysical("Energy",0,pTarget)/1000) {
    //Log("Geheilt!");
    return(-1); //Ja, Ende
  }
  //Och, schon aus? :(
  if(!EffectVar(0,pTarget,iEffectNumber)) {
    //Log("Timeout!");
    return(-1); //Heute ist nicht alle Tage, ich komm wieder, keine Frage!
  }
}

func FxMedipackHealDamage(object pTarget, int iEffectNumber, int iChange)
{
  if(iChange >= 0) return(iChange);

  RemoveEffect("MedipackHeal", pTarget);

  return(iChange);
}

func FxMedipackHealStop(object pTarget, int iEffectNumber, int iReason, bool fTemp)
{
  if(fTemp) return();

  if(pTarget->GetAction() eq "Heal") ObjectSetAction(pTarget, "Walk");
}

public func IsEquipment() { return(true); }

// KI-Behandlung
protected func AI_Inventory(object pClonk)
{
  // Benutzen, wenn der Clonk weniger als 2/3 Leben hat
  if(pClonk->GetEnergy() < pClonk->GetPhysical("Energy") * 2/3 / 1000)
    // Benutzen (verzögert einsetzen)
    ScheduleCall(this, "Activate", 1, 0, pClonk);
  // übernehmen wir
  return(1);
}
