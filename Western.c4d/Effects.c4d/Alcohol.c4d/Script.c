/*-- Alkohol --*/

#strict 2

/* Globale Funktionen */

global func DoAlcohol(object pClonk, int iStrength)  // trinkender Clonk, Stärke des Getränks
{
  // Kein Clonk ausgewählt?
  if(!pClonk) {
    pClonk = this();
    // Effekt kann nicht ausgeführt werden
    if(!pClonk) return(false);
  }
  // Alkoholeffekt hinzufügen
  return(AddEffect("Alcohol",pClonk,101,1,0,ALCE,iStrength));
}

global func GetAlcohol(object pClonk)  // betrunkener Clonk
{
  // Kein Clonk ausgewählt?
  if(!pClonk) {
    pClonk = this();
    // Effekt kann nicht abgefragt werden
    if(!pClonk) return(0);
  }
  // Betrunkenheit zurückgeben
  return(EffectVar(0,pClonk,GetEffect("Alcohol",pClonk)));
}

/* Effekt */

public func FxAlcoholStart(object pTarget, int iEffectNumber, int iTemp, int iAlcohol)
{
  if(iTemp) return(0);
  // Der Alkoholwert
  EffectVar(0,pTarget,iEffectNumber) = iAlcohol;
}

public func FxAlcoholTimer25(object pTarget, int iEffectNumber, int iEffectTime)
{
  var i, iAlcohol;
  // zu viel Alkohol zieht Lebensenergie ab
  if(EffectVar(0,pTarget,iEffectNumber) >= 1400) {
    DoEnergy((1400-EffectVar(0,pTarget,iEffectNumber))/10,pTarget);
    EffectVar(0,pTarget,iEffectNumber) = 1399;
  }
  iAlcohol = EffectVar(0,pTarget,iEffectNumber);

  /* Verwirrungseffekte */

  // Im Container zeigt die Verwirrung kein Effekt
  if(Contained(pTarget)) return(1);

  // Alkoholeinfluß ist sichtbar
  CreateParticle("AlcoholBubble",GetX(pTarget),GetY(pTarget)-9,RandomX(-3,3),-7,25+Min(iAlcohol/60,10),RGBa(50,100,155,0));

  // Wenig Alkohol -> Zielen verruckelt manchmal, Springt verschiedenen weit
  if(WildcardMatch(GetAction(pTarget),"*AimRifle*") && !Random(4)) {
    // Damit JnR keine Vorteile erhält
    i = 1; if(GetPlrCoreJumpAndRunControl(GetController(pTarget))) i = 3;
    if(Random(2)) WINC->~AimUp(pTarget,i,"ControlConf",WINC);
    WINC->~AimDown(pTarget,i,"ControlConf",WINC);
  }
  if(GetProcedure(pTarget) == "FLIGHT" && GetAction(pTarget) != "Tumble")
    SetXDir(GetXDir(pTarget)+RandomX(-3,3),pTarget);

  // Mehr Alkohol -> Manchmal dreht der Clonk um, stolpert
  if(iAlcohol < 300) return(1);
  if(!Random(15)) {
    SetDir(!GetDir(pTarget),pTarget);
    //SetComDir(3+(GetDir(pTarget)*4),pTarget);
    // Fadenkreuz anpassen
    if(WildcardMatch(GetAction(pTarget),"*Rifle*")) WINC->~ActualizePhase(pTarget);
  }
  if(!Random(20) && GetContact(pTarget,2,8))
    Fling(pTarget,GetDir(pTarget)*2-1,-1);

  // Viel Alkohol -> Er kann nicht mehr richtig springen, fällt vom Pferd
  if(iAlcohol < 600) return(1);
  if(!Random(15) && WildcardMatch(GetAction(pTarget),"*Ride*"))
    Fling(pTarget,RandomX(-5,5),-2);
  if(!Random(5) && WildcardMatch(GetAction(pTarget),"*Jump*"))
    Fling(pTarget,GetXDir(pTarget,1),GetYDir(pTarget,1));

  // Zu viel Alkohol -> Siehe oben!
  return(1);
}

public func FxAlcoholTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  // Alkoholgehalt nimmt ab
  EffectVar(0,pTarget,iEffectNumber) -= (iEffectTime - EffectVar(1,pTarget,iEffectNumber));
  EffectVar(1,pTarget,iEffectNumber) = iEffectTime;

  // Kein Alkohol mehr vorhanden -> Abbruch
  if(EffectVar(0,pTarget,iEffectNumber) <= 0) return(-1);

  if(!(iEffectTime % 25)) EffectCall(pTarget,iEffectNumber,"Timer25",pTarget,iEffectNumber);
  // Sichtschwankungen
  if(GetCursor(GetOwner(pTarget)) == pTarget) {
    EffectVar(2,pTarget,iEffectNumber) = 0;
    SetViewOffset(GetOwner(pTarget),Sin(iEffectTime*10,EffectVar(0,pTarget,iEffectNumber)/100));
  }
  // Damit andere Sichtschwankungen noch erhalten bleiben
  else if(!EffectVar(2,pTarget,iEffectNumber)) {
    EffectVar(2,pTarget,iEffectNumber) = 1;
    SetViewOffset(GetOwner(pTarget));
  }
}

public func FxAlcoholStop(object pTarget, int iEffectNumber, int iReason, bool fTemp)
{
  if(fTemp) return(0);
  // Sichtschwankungen bei Bedarf zurücksetzen
  if(!EffectVar(2,pTarget,iEffectNumber))
    SetViewOffset(GetOwner(pTarget));
  return(0);
}

public func FxAlcoholEffect(string szNewEffectName)
{
  if(WildcardMatch(szNewEffectName, "*Alcohol*")) return(-2);
}

// Alkoholeffekte überladen sich
public func FxAlcoholAdd(object pTarget, int iEffectNumber, string szNewEffectName, int iNewEffectTimer, int iAlcohol)
{
  EffectVar(0,pTarget,iEffectNumber) += iAlcohol;
}