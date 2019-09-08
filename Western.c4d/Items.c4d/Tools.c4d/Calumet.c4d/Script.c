/*-- Calumet --*/

#strict 2

protected func Hit() { Sound("WoodHit*"); }

public func Activate(pClonk)
{
  if(WildcardMatch(GetAction(pClonk), "*Walk"))
    if(!GetEffect("IntSmoking",pClonk))
      AddEffect("IntSmoking",pClonk,1,1,0,GetID());
  return(1);
}

protected func FxIntSmokingStart(object pTarget, int iEffectNumber, int iTemp)
{
  // Temporäre Aufrufe ignorieren
  if(iTemp) return(0);
  // Clonk muss rauchen
  if(!ObjectSetAction(pTarget, "StartSmoke"))
    // Ansonsten wird der Effekt nicht erzeugt
    return(-1);
  // Noch die Dauer der Aktion notieren
  EffectVar(0,pTarget,iEffectNumber) = GetActMapVal("Length","StartSmoke",GetID(pTarget))*GetActMapVal("Delay","StartSmoke",GetID(pTarget));
  var iAction = GetActMapVal("NextAction","StartSmoke",GetID(pTarget));
  EffectVar(0,pTarget,iEffectNumber) += 3*GetActMapVal("Length",iAction,GetID(pTarget))*GetActMapVal("Delay",iAction,GetID(pTarget));
  EffectVar(1,pTarget,iEffectNumber) = iAction;
  // Fertig
  return(1);
}

protected func FxIntSmokingTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  // Zeit abgelaufen?
  if(iEffectTime >= EffectVar(0,pTarget,iEffectNumber))
    // Weg mit dem Effekt
    return(-1);
  // meditiert nicht mehr? (Fling, kämpft mit Clonk usw)
  if(GetAction(pTarget) != EffectVar(1,pTarget,iEffectNumber) && GetAction(pTarget) != "StartSmoke")
    // Abbruch
    return(-1);
}

protected func FxIntSmokingStop(object pTarget, int iEffectNumber, int iReason, bool fTemp)
{
  // Temporäre Aufrufe ignorieren
  if(fTemp) return(0);
  // Wieder stehen
  ObjectSetAction(pTarget,"Walk");
  // Fertig
  return(1);
}


/* Globale Funktionen */

global func DoSmoke(object pClonk, int iStrength)
{
  // Rauscheffekt hinzufügen
  AddEffect("Smoke",pClonk,101,25,0,CALU,iStrength);
}


/* Effekt */ // Rauchen bringt einen ähnlichen Effekt wie Alcohol

public func FxSmokeStart(object pTarget, int iEffectNumber, int iTemp, int iSmoke)
{
  if(iTemp) return(0);
  // Der Rauschwert
  EffectVar(0,pTarget,iEffectNumber) = iSmoke;
//  if(!EffectVar(2,pTarget,iEffectNumber)) EffectVar(2,pTarget,iEffectNumber) = GetEnergy(pTarget);
}

public func FxSmokeTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  // Rauchgehalt nimmt ab
  EffectVar(0,pTarget,iEffectNumber) -= (iEffectTime - EffectVar(1,pTarget,iEffectNumber));
  EffectVar(1,pTarget,iEffectNumber)  =  iEffectTime;

  // Kein Rauch mehr vorhanden -> Abbruch
  if(EffectVar(0,pTarget,iEffectNumber) <= 0) return(-1);

  var i, iSmoke;
  // zu viel Rauch zieht Lebensenergie ab
  if(EffectVar(0,pTarget,iEffectNumber) >= 1400) {
    DoEnergy((1400-EffectVar(0,pTarget,iEffectNumber))/10,pTarget);
    EffectVar(0,pTarget,iEffectNumber) = 1399;
    // Raucher informieren
    RemoveEffect("IntSmoking",pTarget);
  }
  iSmoke = EffectVar(0,pTarget,iEffectNumber);

  /* Verwirrungseffekte */

  // wenig Rausch -> Zielen verruckelt manchmal, Springt verschiedenen weit
  if(WildcardMatch(GetAction(pTarget),"*AimRifle*") && !Random(4)) {
    // Damit JnR keine Vorteile erhält
    i = 1; if(GetPlrCoreJumpAndRunControl(GetController(pTarget))) i = 3;
    if(Random(2)) WINC->~AimUp(pTarget,i,"ControlConf",WINC);
    WINC->~AimDown(pTarget,i,"ControlConf",WINC);
  }
  if(GetAction(pTarget) == "Jump")
    SetXDir(GetXDir(pTarget)+RandomX(-3,3),pTarget);

  // mehr Rausch -> Manchmal dreht der Clonk um, stolpert, fällt vom Pferd
  if(iSmoke >= 100 && !Random(15)) {
    SetDir(!GetDir(pTarget),pTarget); SetComDir(3+(GetDir(pTarget)*4),pTarget);
    // Fadenkreuz anpassen
    if(WildcardMatch(GetAction(pTarget),"*Rifle*")) WINC->~ActualizePhase(pTarget);
  }
  if(iSmoke >= 300 && !Random(20) && GetContact(pTarget,2,8))
    Fling(pTarget,GetDir(pTarget)*2-1,-1);
  if(iSmoke >= 500 && !Random(15) && WildcardMatch(GetAction(pTarget),"*Ride*"))
    Fling(pTarget,RandomX(-5,5),-2);

  // viel Rausch -> er kann nicht mehr richtig springen, manchmal lässt er Dinge fallen
  if(iSmoke >= 700 && !Random(5) && WildcardMatch(GetAction(pTarget),"*Jump*"))
    Fling(pTarget,GetXDir(pTarget,1),GetYDir(pTarget,1));
// Dies kann zu fehlern usw. führen, zudem werden die Objekte nach dem Ablegen gleich wieder eingesammelt
//  if(iSmoke >= 900 && !Random(25) && Contents(0,pTarget))
//    Exit(Contents(0,pTarget),AbsX(GetX(pTarget)),AbsY(GetDefBottom(pTarget)),RandomX(-10,10),RandomX(-1,1),0,RandomX(-10,10));
  // Raucheinfluß ist sichtbar
//  if(!Contained(pTarget)) pTarget->~Smoke(0,-2,8);
  // Damit wenigstens hier noch die Rauchzeit zu sehen ist
  if(!Contained(pTarget)) pTarget->~Smoke(0,-2,iSmoke/100+2);

  // Rauch wirkt sich auch auf die Körperlichen Eigenschaften aus
  if(iSmoke < 600) i = 30*iSmoke;
  else i = 42000-40*iSmoke;

  if(EffectVar(2,pTarget,iEffectNumber)) {
    ResetPhysical(pTarget,"Walk");
    ResetPhysical(pTarget,"Jump");
  }
  EffectVar(2,pTarget,iEffectNumber) = 1;
  SetPhysical("Walk",GetPhysical("Walk",0,pTarget)+i  ,3,pTarget);
  SetPhysical("Jump",GetPhysical("Jump",0,pTarget)+i/2,3,pTarget);
}

public func FxSmokeDamage(object pTarget, int iEffectNumber, int iDmgEngy, int iCause)
{
  // Nicht bei Heilungen
  if(iDmgEngy > 0) return(iDmgEngy);
  // Da Pfeile und Munition Energieänderung durch Scriptaufruf sind,
  // wirkt es vorerst auf alle Schäden
//  if(iCause != 33 && iCause != 34 && iCause != 35 && iCause != 38 && iCause != 40) return(iDmgEngy);
  var iSmoke = EffectVar(0,pTarget,iEffectNumber);
  // Raucher bekommen weniger Schaden
  if(iSmoke < 600) return(iDmgEngy*(1800-iSmoke)/1800);
  return(iDmgEngy*Min(iSmoke+200,1200)/1200);
//  return(iDmgEngy*3/4);
}

public func FxSmokeStop(object pTarget, int iEffectNumber, int iReason, bool fTemp)
{
  if(fTemp) return(0);
  if(!EffectVar(2,pTarget,iEffectNumber)) return(0);
  ResetPhysical(pTarget,"Walk");
  ResetPhysical(pTarget,"Jump");
  return(0); 
}

public func FxSmokeEffect(name)
{
  if(WildcardMatch(name, "*Smoke*")) return(-2);
}

public func FxSmokeAdd(target, no, name, timer, smoke)   // Alkoholeffekte überladen sich
{
  EffectVar(0, target, no) += smoke;
}

public func GetResearchBase() { return(MTIP); }

/* Kann von Indianern erforscht werden */
public func IsIndianResearch() { return(1); }

/* Kann vom Indianer in Handarbeit hergestellt werden */
public func IsIndianHandcraft() { return(1); }
