/*-- Gatling --*/

#strict 2

local pCrosshair, iRotation;
local iDirection;

protected func Initialize()
{
  SetAction("Ready");
  SetDir(Random(2));
  iRotation = 90;
  SetGraphics("Head",this(),GetID(),1,2,"Head");
  SetGraphics("Body",this(),GetID(),2,1);
  UpdateCrosshair();
  UpdateGraphics();
}

public func Check()
{
  var iXDir = GetXDir(this(),100);
  var pClonk = FindObject(0,0,0,0,0,OCF_CrewMember,"Push",this(),NoContainer());
  if(!iXDir || !pClonk) {
    if(iDirection == GetDir(this())) return(1);
    UpdateCrosshair();
    UpdateGraphics();
    iDirection = GetDir(this());
    return(1);
  }
  if(FrameCounter() & 1)
    if(!FindPullingHorse())
      SetXDir(BoundBy(iXDir,-7,7),this(),100); // Sehr leichte Bewegung
  if(BoundBy(iXDir,0,1) != iDirection) {
    SetDir(1-iDirection);
    UpdateGraphics();
    UpdateCrosshair();
    iDirection = GetDir(this());
  }
  return(1);
}

/* Aktionen */

public func TurnLeft()
{
  if(GetDir() == DIR_Left) return(0);
  if(GetXDir(this(),100) > 0) return(0);
  SetDir(DIR_Left);
  iDirection = DIR_Left;
  UpdateGraphics();
  UpdateCrosshair();
  return(0);
}

public func TurnRight()
{
  if(GetDir() == DIR_Right) return(0);
  if(GetXDir(this(),100) < 0) return(0);
  SetDir(DIR_Right);
  iDirection = DIR_Right;
  UpdateGraphics();
  UpdateCrosshair();
  return(0);
}

public func UpdateGraphics(int iDir)
{
  if(!iDir) iDir = GetDir()*2-1;
  var iR = iRotation-90;
  SetObjDrawTransform(-Cos(iR,1000)*iDir,-Sin(iR,1000)*iDir,(Sin(iRotation,5000)+ 6000)*iDir,
		      -Sin(iR,1000),      Cos(iR,1000),     -Cos(iRotation,5000)- 6000,this(),1);
  SetObjDrawTransform(-1000*iDir,0,0,0,1000,0,this(),2);
  return(1);
}

public func UpdateCrosshair(int iDir)
{
  if(Contained()) {
    if(pCrosshair) RemoveObject(pCrosshair);
    return(0);
  }
  if(!pCrosshair) {
    pCrosshair = CreateObject(GC4V,0,0,GetOwner(this()));
    ObjectSetAction(pCrosshair,"Crosshair",this());
  }
  if(!iDir) iDir = GetDir()*2-1;
  SetVertexXY(0,(-Sin(iRotation,40)-8)*iDir,Cos(iRotation,40),pCrosshair);
}

protected func Departure(object pContainer)
{
  UpdateCrosshair();
  return(1);
}

protected func Entrance(object pContainer)
{
  UpdateCrosshair();
  return(1);
}

protected func Destruction() { if(pCrosshair) RemoveObject(pCrosshair); }

public func Grabbed(object pByObject, bool fGrab)
{
  if(!pCrosshair) return(0);
  var iPlr = GetOwner(pByObject), i = iPlr/32, i2 = 2**iPlr;
  if(fGrab + ((Local(i,pCrosshair) & i2) == 0) == 1) return(0);
  if(fGrab) {
    AddEffect("IntIsGrabbingGTLG",pByObject,1,1,this(),0,this());
    Local(i,pCrosshair) += i2;
    return(0);
  }
  if(!FindObject2(Find_Owner(iPlr),Find_Action("Push"),Find_ActionTarget(this()),Find_Exclude(pByObject)))
    Local(i,pCrosshair) -= i2;
}

public func FxIntIsGrabbingGTLGStart(object pTarget, int iEffectNumber, int iTemp, object pGatling)
{
  if(iTemp) return(0);
  EffectVar(0,pTarget,iEffectNumber) = pGatling;
  // EVar1: Bewegung
  return(1);
}

public func FxIntIsGrabbingGTLGTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  if(GetAction(pTarget) != "Push") return(-1);
  if(GetActionTarget(0,pTarget) != EffectVar(0,pTarget,iEffectNumber)) return(-1);
  var iDir;
  if(iDir = EffectVar(1,pTarget,iEffectNumber))
    EffectVar(0,pTarget,iEffectNumber)->~ControlConf(iDir,pTarget);
  return(1);
}

public func FxIntIsGrabbingGTLGStop(object pTarget, int iEffectNumber, int iReason, bool fTemp)
{
  if(fTemp) return(0);
  EffectVar(0,pTarget,iEffectNumber)->~Grabbed(pTarget,0);
  return(1);
}

/* Steuerung */

protected func ControlLeft(object pClonk)
{
  // Umdrehen
  return(TurnLeft());
}

protected func ControlRight(object pClonk)
{
  // Umdrehen
  return(TurnRight());
}

// Da die Gatling in einen besseren Schusswinkel zu stellen ist,
// erhält sie nun eine andere Steuerung für JnR'ler

protected func ControlUp(object pClonk)
{
  // Zielen: Hoch
  [$TxtAimup$|Image=CAN1:2]
  if(GetPlrCoreJumpAndRunControl(GetController(pClonk))) {
    var iEffect = GetEffect("IntIsGrabbingGTLG",pClonk);
    if(!iEffect)
      iEffect = AddEffect("IntIsGrabbingGTLG",pClonk,1,1,this(),0,this());
    EffectVar(1,pClonk,iEffect) = -1;
    return(1);
  }
  return(AimUp(pClonk,4,"ControlConf"));
}

protected func ControlDig(object pClonk)
{
  // Zielen: Runter
  [$TxtAimdown$|Image=CAN1:0]
  if(GetPlrCoreJumpAndRunControl(GetController(pClonk))) {
    var iEffect = GetEffect("IntIsGrabbingGTLG",pClonk);
    if(!iEffect)
      iEffect = AddEffect("IntIsGrabbingGTLG",pClonk,1,1,this(),0,this());
    EffectVar(1,pClonk,iEffect) = 1;
    return(1);
  }
  return(AimDown(pClonk,4,"ControlConf"));
}

protected func ControlUpReleased(object pClonk)
{
  var iEffect = GetEffect("IntIsGrabbingGTLG",pClonk);
  if(!iEffect) return(0);
  if(EffectVar(1,pClonk,iEffect) == -1) EffectVar(1,pClonk,iEffect) = 0;
  return(1);
}

protected func ControlDigReleased(object pClonk)
{
  var iEffect = GetEffect("IntIsGrabbingGTLG",pClonk);
  if(!iEffect) return(0);
  if(EffectVar(1,pClonk,iEffect) == 1) EffectVar(1,pClonk,iEffect) = 0;
  return(1);
}

protected func ControlConf(int iConf, object pClonk)
{
  // Feinere Abstufung mit JnR möglich
  var iChange;
  if(GetPlrCoreJumpAndRunControl(GetController(pClonk)))
    iChange = iConf * 2;
  else
    iChange = iConf * 5;
  iRotation = BoundBy(iRotation + iChange, 50, 130);
  UpdateCrosshair();
  UpdateGraphics();
}

protected func ControlThrow(object pClonk)
{
  [$TxtFire$|Image=CAN1:1]
  // Der Clonk will doch bestimmt nur etwas nachladen: nicht vorzeitig abfeuern
  var iPlr, pObject, szMessage;
  if(pObject = Contents(0,pClonk))
    if((szMessage = AllowLoad(GetID(pObject))) == 0)
      return(0);

  if(GetAction() != "Ready") return(0);

  // Abfeuern wenn möglich, sonst Objekt herausnehmen
  if(Fire(pClonk)) return(1);
  if(szMessage) {
    iPlr = GetOwner(pClonk);
    if(iPlr == -1) return(1);
    PlayerMessage(iPlr, szMessage, this());
    Sound("Error",0,this(),0,iPlr+1);
    return(1);
  }
  return(0);
}

protected func ControlThrowReleased(object pClonk)
{
  var iEffect = GetEffect("IntIsShooting",this());
  if(iEffect) RemoveEffect(0,this(),iEffect);
  return(1);
}

/* Laden und Feuern */

public func RejectCollect(id idObj, object pObj) { return(AllowLoad(idObj) != 0); }

private func AllowLoad(id idObj)
{
  // Nur Trommelmagazine
  if(!idObj->~IsGatlingPack()) return(Format("$TxtPossibleContents$"));
  // Maximal 1 Stück
  if(ContentsCount()) return(Format("$TxtFull$"));
  // Laden o.k.
  return(0);
}

protected func Collection(object pObj)
{
  RemoveEffect("IntIsShooting",this());
  AddEffect("IntIsReloading",this(),1,175,this());
  Sound("GatlingReload",0,this());
  return(1);
}

protected func Ejection(object pObj)
{
  RemoveEffect("IntIsReloading",this());
  Sound("GatlingReload",0,this(),0,0,-1);
  return(1);
}

public func Fire(object pClonk)
{
  // keine Munition
  if(!Contents()) {
    Sound("Click");
    return(0);
  }

  // Noch am Nachladen
  if(IsReloading()) return(1);

  var iEffect = GetEffect("IntIsShooting",this());
  if(pClonk && GetPlrCoreJumpAndRunControl(GetController(pClonk))) {
    if(!iEffect) AddEffect("IntIsShooting",this(),1,8,this());
    return(1);
  }
  if(iEffect) RemoveEffect(0,this(),iEffect);
  else AddEffect("IntIsShooting",this(),1,8,this());

  return(1);
}

protected func FxIntIsShootingStart(object pTarget, int iEffectNumber, int iTemp)
{
  if(iTemp) return(0);
  Sound("GatlingShot", 0, pTarget, 100, 0, 1);
// Um Schnellfeuer zu vermeiden
//  pTarget->~Shoot();
  return(1);
}

protected func FxIntIsShootingTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  if(!FindObject2(Find_Action("Push"),Find_ActionTarget(pTarget))) return(-1);
  pTarget->~Shoot();
  return(1);
}

protected func FxIntIsShootingStop(object pTarget, int iEffectNumber, int iReason, bool fTemp)
{
  if(fTemp) return(0);
  Sound("GatlingShot", 0, pTarget, 100, 0, -1);
  return(1);
}

public func Shoot()
{
  var pObj, pProjectile;
  pObj = Contents(0,this());
  if(!pObj) {
    RemoveEffect("IntIsShooting",this());
    return(1);
  }
//  pProjectile = CreateContents(pObj->~UnpackTo(),this());
  pProjectile = CreateContents(pObj->~GatlingUnpackTo(),this());
  pObj->~DoPackCount(-1);
  // Austritt berechnen
  var iX, iY, iXDir, iYDir, iAngle, iR, iDir;
  iDir = GetDir()*2-1;
  iAngle = iRotation;
  iR = iAngle*10+RandomX(-20,20);
  iX =  Sin(iAngle-5,21)*iDir+GetDir()+6*iDir;
  iY = -Cos(iAngle-5,21)-6;
  iXDir =  Sin(iR,240,10)*iDir;
  iYDir = -Cos(iR,240,10);

  // Besitzer setzen
  SetOwner(GetController(),pProjectile);

  Exit(pProjectile,iX,iY,iAngle*iDir+90);
  SetXDir(iXDir,pProjectile,10);
  SetYDir(iYDir,pProjectile,10);
  pProjectile->~Launch(-1,10+Random(10));
  CreateParticle("MuzzleFlash",iX,iY,iXDir,iYDir,40,RGB(255,255,255));
  Smoke(iX,iY,RandomX(2,4));
  iX = (Sin(iAngle-45,2)+7)*iDir;
  iY = -Cos(iAngle-45,2)-8;
  iXDir =  Sin(iAngle-115,16)*iDir;
  iYDir = -Cos(iAngle-115,16);
  CreateParticle("Casing", iX, iY, iXDir+RandomX(-2,2), iYDir+RandomX(-2,2), 18, RGB(255,255,0));
  return(1);
}

/* Status */

public func GetAmmoCount()
{
  if(Contents()) return(Contents()->~PackCount());
  return(0);
}

public func IsReloading() { return(GetEffect("IntIsReloading",this()) != 0); }
public func IsShooting()  { return(GetEffect("IntIsShooting", this()) != 0); }

// Für Patronen
public func IsWeapon() { return(true); }

/* Dieses Objekt ist definitiv zu groß für ein Pferd */
//public func IsHorseWagon() { return(0); }
// Damit es sich aber wenigstens etwas transportieren lässt
public func IsHorseWagon() { return(1); }   // Stimmt zwar nicht, aber muss sein, damit anspannbar

private func FindPullingHorse(pHorse)
{
  return(FindObject2(Find_ActionTarget(this()),Find_Or(Find_Action("Pull"),Find_Action("Pull2"),Find_Action("Pull3"),Find_Action("Turn"))));
}

/* Verbindung */

public func Connect(pHorse)
{
  CreateObject(CHBM,0,0,GetOwner())->CHBM::Connect(pHorse,this());
  Sound("Connect");
  return(1);
}

public func Disconnect(pFromHorse)
{         
  var pBeam = FindObject(CHBM,0,0,0,0,0,"Connect",pFromHorse);
  if (pBeam) RemoveObject(pBeam);
  Sound("Connect");
  return(1);
}

/* Forschung */

public func GetResearchBase() { return(CCAN); }
public func GetResearchBase2() { return(WINC); }