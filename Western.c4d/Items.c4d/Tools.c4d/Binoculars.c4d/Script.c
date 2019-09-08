/*-- Fernglas --*/

#strict 2

local pViewObj;

public func ViewRange(bool fMode) { if(!fMode) return(600); return(400); }

protected func Initialize(object pByObj)
{
  pViewObj = CreateContents(TIM1,this());
  SetPlrViewRange(ViewRange(),pViewObj);
  if(Contained()) return(Entrance(Contained()));
  SetOwner(-1,pViewObj);
  return(1);
}

protected func Hit() {
  Sound("MetalHit*");
  return(1);
}

public func Activate(object pByObj)
{
  if(GetProcedure(pByObj) != "WALK") return(0);
  if(Contained(pByObj)) return(0);
  if(!GetEffect("IntBinoculars",pByObj))
    AddEffect("IntBinoculars",pByObj,1,1,0,GetID(),this(),ViewRange(true));
  else
    RemoveEffect("IntBinoculars",pByObj);
  return(1);
}

public func ControlThrow(object pByObj)
{
  var iEffect = GetEffect("IntBinoculars",pByObj);
  if(!iEffect) return(0);
  if(Contained(pByObj)) return(0);
  if(GetPlrDownDouble(GetController(pByObj))) return(0);
  if(GetProcedure(pByObj) == "PUSH") return(0);
  return(EffectCall(pByObj,iEffect,"Extra"));
}

public func Entrance(object pContainer)
{
  if(!pViewObj) return(1);
  if(!GetDefCrewMember(GetID(pContainer)))
    SetOwner(-1,pViewObj);
  else
    SetOwner(GetOwner(pContainer),pViewObj);
  return(1);
}

public func Departure(object pObj)
{
  if(pViewObj) SetOwner(-1,pViewObj);
  RemoveEffect("IntBinoculars",pObj);
  return(1);
}

protected func Destruction()
{
  if(pViewObj) RemoveObject(pViewObj);
  if(Contained())
    RemoveEffect("IntBinoculars",Contained());
  return(1);
}

protected func FxIntBinocularsStart(object pTarget, int iEffectNumber, int iTemp, object pBinoculars, int iViewRange)
{
  if(iTemp) return(0);
  var pObj = CreateObject(TIM1,0,0,GetOwner(pTarget));
  if(!iViewRange) iViewRange = 400;
  SetPlrViewRange(iViewRange,pObj);
  SetPosition(GetX(pTarget),GetY(pTarget),pObj);
  SetPlrView(GetOwner(pTarget),pObj);
  SetVisibility(VIS_None,pObj);
  SetPicture(73,0,64,64,pBinoculars);
  EffectVar(0,pTarget,iEffectNumber) = pObj;
  EffectVar(1,pTarget,iEffectNumber) = pBinoculars;
  EffectVar(5,pTarget,iEffectNumber) = iViewRange;
  return(1);
}

protected func FxIntBinocularsExtra(object pTarget, int iEffectNumber)
{
  if(!EffectVar(2,pTarget,iEffectNumber)) {
    EffectVar(2,pTarget,iEffectNumber) = Min(GetEffect(0,pTarget,iEffectNumber,6)*16,1600);
    return(1);
  }
  EffectVar(3,pTarget,iEffectNumber) = (!EffectVar(3,pTarget,iEffectNumber));
  return(1);
}

protected func FxIntBinocularsTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  var iX;
  // Im Gebäude gibt es keine Sicht
  if(Contained(pTarget)) {
    if(EffectVar(4,pTarget,iEffectNumber) == 2) return(1);
    // War er gerade noch beim Einstellen der Sicht?
    if(!EffectVar(2,pTarget,iEffectNumber))
      // Abbruch
      return(-1);
    // Sicht deaktivieren
    SetPlrViewRange(0,EffectVar(0,pTarget,iEffectNumber));
    // Ist der Clonk überhaupt gerade angewählt?
    if(GetCursor(GetOwner(pTarget)) == pTarget)
      SetPlrView(GetOwner(pTarget),pTarget);
    // Dateien speichern
    EffectVar(3,pTarget,iEffectNumber) = 1;
    EffectVar(4,pTarget,iEffectNumber) = 2;
    return(1);
  }
  else if(EffectVar(4,pTarget,iEffectNumber) == 2) {
    // Sicht wiederherstellen
    SetPlrViewRange(EffectVar(5,pTarget,iEffectNumber),EffectVar(0,pTarget,iEffectNumber));
    EffectVar(4,pTarget,iEffectNumber) = 0;
  }
  // Position anpassen
  if(EffectVar(2,pTarget,iEffectNumber))
    iX = EffectVar(2,pTarget,iEffectNumber)*(GetDir(pTarget)*2-1);
  else {
    iX = iEffectTime*16*(GetDir(pTarget)*2-1);
    if(iEffectTime >= 100) EffectVar(2,pTarget,iEffectNumber) = 1600;
  }
  SetPosition(BoundBy(GetX(pTarget)+iX,20,LandscapeWidth()-20),GetY(pTarget),EffectVar(0,pTarget,iEffectNumber));
  // Schaut er gerade nicht in die Ferne?
  if(EffectVar(3,pTarget,iEffectNumber)) {
    if(EffectVar(4,pTarget,iEffectNumber)) return(1);
    if(GetCursor(GetOwner(pTarget)) == pTarget)
      SetPlrView(GetOwner(pTarget),pTarget);
    EffectVar(4,pTarget,iEffectNumber) = 1;
    return(1);
  }
  // Sicht anpassen
  if(GetCursor(GetOwner(pTarget)) == pTarget)
    SetPlrView(GetOwner(pTarget),EffectVar(0,pTarget,iEffectNumber));
  EffectVar(4,pTarget,iEffectNumber) = 0;
  return(1);
}

protected func FxIntBinocularsStop(object pTarget, int iEffectNumber, int iReason, bool fTemp)
{
  if(fTemp) return(0);
  if(EffectVar(0,pTarget,iEffectNumber)) RemoveObject(EffectVar(0,pTarget,iEffectNumber));
  SetPlrView(GetOwner(pTarget),pTarget);
  SetPicture(0,0,0,0,EffectVar(1,pTarget,iEffectNumber));
  return(1);
}

/* Objekt darf nur einmal im Inventar sein */
public func CarryLimit() { return(1); }

/* Kann in der Waffenschmiede hergestellt werden */
public func IsArmoryProduct() { return(1); }