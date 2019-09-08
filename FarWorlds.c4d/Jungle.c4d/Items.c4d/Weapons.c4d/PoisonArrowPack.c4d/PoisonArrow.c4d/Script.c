/*-- Giftpfeil --*/

#strict

#include ARRW

/* Pfeileigenschaften */
public func PackTo() { return(PARP); }

public func ArrowStrength() { return(8); }
public func ArrowTumbleRandom() {return(1);}
public func ArrowStopsOnHit() { return(1); }

private func DecPhysical(string szPhysical, int iDec, int iMode, object pObject)
{
  SetPhysical(szPhysical,Min(GetPhysical(szPhysical,0,pObject)-iDec,100000),iMode,pObject);
}

/* Treffer */
private func HitTarget(object pTarget,par2)
{
  if(!_inherited(pTarget,par2))return();
  
  AddEffect("PoisonCurse",pTarget,182,37*30,0,GetID());
  ChangeDef(ARW2);
}

/* Vergiftungseffekt */

func FxPoisonCurseStart(object pTarget,int iEffectNumber, int iTemp)
{
  // Attributsverlust
  DecPhysical("Dig" ,  10000,PHYS_StackTemporary,pTarget);
  DecPhysical("Walk",  20000,PHYS_StackTemporary,pTarget);
  DecPhysical("Jump",  10000,PHYS_StackTemporary,pTarget);
  DecPhysical("Throw", 10000,PHYS_StackTemporary,pTarget);
  DecPhysical("Swim",  10000,PHYS_StackTemporary,pTarget);
  DecPhysical("Scale", 10000,PHYS_StackTemporary,pTarget);
  DecPhysical("Hangle",10000,PHYS_StackTemporary,pTarget);
  DecPhysical("Fight", 20000,PHYS_StackTemporary,pTarget);
  DecPhysical("Breath",30000,PHYS_StackTemporary,pTarget);
  // temporäre Effekte ignorieren
  if(iTemp) return();
  // Hautfarbe leicht abändern :)
  SetObjectBlitMode(4,pTarget);
  SetClrModulation(RGB(200,255,200),pTarget);
}

func FxPoisonCurseTimer() { return(-1); }

func FxPoisonCurseStop(object pTarget,int iEffectNumber,int iReason, bool fTemp)
{
  // Attribute wiederherstellen
  ResetPhysical(pTarget, "Breath");
  ResetPhysical(pTarget, "Fight");
  ResetPhysical(pTarget, "Hangle");
  ResetPhysical(pTarget, "Scale");
  ResetPhysical(pTarget, "Swim");
  ResetPhysical(pTarget, "Throw");
  ResetPhysical(pTarget, "Jump");
  ResetPhysical(pTarget, "Walk");
    ResetPhysical(pTarget, "Dig");
  if(fTemp) return();
  // Hautfarbe wieder zurückändern
  if(!iReason) { SetObjectBlitMode(0,pTarget); SetClrModulation(RGB(255,255,255),pTarget); }
  return(FX_OK);
}

func FxPoisonCurseEffect(string szNewEffectName, object pTarget, int iEffectNumber, int iNewEffectNumber)
{
  // keine weiteren Vergiftungen zulassen während diese noch ausgestanden wird
  if(szNewEffectName eq "PoisonCurse") return(-1);
  // wenn man vergiftet ist, ist das Amulett des Athleten wirkungslos
  if(szNewEffectName eq "PhysicalBless") return(-1);
}
