/*-- Novodekan --*/

#strict

func Activate(object pByObj) {
  if(pByObj->GetEnergy() >= pByObj->GetPhysical("Energy") / 1000) return(0);

  if(GetEffect("*Heal*", pByObj)) return(0);

  AddEffect("NovodekanHeal",pByObj,20,1,0,GetID(),HealAmount(),HealRate());
  Sound("Medi");
  RemoveObject();
  return(1);
}

func HealRate() { return(7); }
func HealAmount() { return(100); }

func FxNovodekanHealStart(object pTarget, int iEffectNumber, int iTemp, int iHealAmount, int iHealRate) {
  if(iTemp)
    if(GetPhysical("Walk", 2, pTarget) <= GetPhysical("Walk", 1, 0, GetID(pTarget))*5/10)
      return();
  //Log("*HeilStart*");
  EffectVar(0,pTarget,iEffectNumber) = iHealAmount; //Heilung pro Frame
  EffectVar(1,pTarget,iEffectNumber) = iHealRate; //Zeitdauer der Heilung

  // Lähmung
  EffectVar(2, pTarget, iEffectNumber) = GetPhysical("Walk", 1, 0, GetID(pTarget))*3/10;
  EffectVar(3, pTarget, iEffectNumber) = GetPhysical("Jump", 1, 0, GetID(pTarget))*3/10;
  EffectVar(4, pTarget, iEffectNumber) = GetPhysical("Scale", 1, 0, GetID(pTarget))*3/10;
  EffectVar(5, pTarget, iEffectNumber) = GetPhysical("Hangle", 1, 0, GetID(pTarget))*3/10;

  SetPhysical("Walk", GetPhysical("Walk", 0, pTarget)-EffectVar(2, pTarget, iEffectNumber), 2, pTarget);
  SetPhysical("Jump", GetPhysical("Jump", 0, pTarget)-EffectVar(3, pTarget, iEffectNumber), 2, pTarget);
  SetPhysical("Scale", GetPhysical("Scale", 0, pTarget)-EffectVar(4, pTarget, iEffectNumber), 2, pTarget);
  SetPhysical("Hangle", GetPhysical("Hangle", 0, pTarget)-EffectVar(5, pTarget, iEffectNumber), 2, pTarget);
}

func FxNovodekanHealTimer(object pTarget, int iEffectNumber, int iEffectTime) {
//Log("*heil*");
//Effekt
  var width= GetDefCoreVal("Width",0,GetID(pTarget)),
      height = GetDefCoreVal("Height",0,GetID(pTarget));
  width -= width/3;
  height -= height/3;
  width /= 2;
  height /= 2;
  CreateParticle("PxSpark",GetX(pTarget)+RandomX(-width, width),GetY(pTarget)+RandomX(-height,height),0,-10,Random(25)+10,RGBa(255,0,255,50));
//*Heile mach*
  if(!(iEffectTime % EffectVar(1, pTarget, iEffectNumber)))
  {
    DoEnergy(1, pTarget);
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

public func FxNovodekanHealStop(target, no, reason, temp)
{
  SetPhysical("Walk", GetPhysical("Walk", 0, target)+EffectVar(2, target, no), 2, target);
  SetPhysical("Jump", GetPhysical("Jump", 0, target)+EffectVar(3, target, no), 2, target);
  SetPhysical("Scale", GetPhysical("Scale", 0, target)+EffectVar(4, target, no), 2, target);
  SetPhysical("Hangle", GetPhysical("Hangle", 0, target)+EffectVar(5, target, no), 2, target);
}

public func IsEquipment() { return(true); }

// KI-Behandlung
protected func AI_Inventory(object pClonk)
{
  // Benutzen, wenn der Clonk weniger als 2/3 Leben hat
  if(pClonk->GetEnergy() < pClonk->GetPhysical("Energy") * 2/3 / 1000)
    // Benutzen (verzögert)
    ScheduleCall(this, "Activate", 1, 0, pClonk);
  // übernehmen wir
  return(1);
}
