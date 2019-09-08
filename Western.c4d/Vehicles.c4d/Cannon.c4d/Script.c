/*-- Kanone --*/

#strict 2

local pCrosshair, iRotation;
local iDirection;

protected func Initialize()
{
  SetAction("Ready");
  SetDir(Random(2));
  iRotation = 80;
  SetGraphics("Head",this(),GetID(),1,2,"Head");
  SetGraphics("Body",this(),GetID(),2,6);
  SetPhase(0);
  UpdateCrosshair();
  UpdateGraphics();
}

protected func UpdateTransferZone()
{
	// Abwaertskompatibilitaet
	if (ActIdle()) Initialize();
	return true;
}

// Eigentlich sollen nur Pferde die Kanone ziehen
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
  // Bewegung und ein schiebender Clonk
  if(FrameCounter() & 1)
    // Kein Pferd vorhanden?
    if(!FindPullingHorse())
      SetXDir(BoundBy(GetXDir(),-1,1)); // Nur leichte Bewegung
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

public func UpdateGraphics()
{
  var iDir = GetDir()*2-1;
  SetObjDrawTransform(-Sin(iRotation,1000)*iDir,Cos(iRotation,1000)*iDir,(6700-Sin(iRotation+58,3200))*iDir,
		       Cos(iRotation,1000),     Sin(iRotation,1000),     -1300-Sin(iRotation+58,3200),this(),1);
/*
  SetObjDrawTransform(-Sin(iRotation,1000)*iDir,Cos(iRotation,1000)*iDir,(10000-Cos(iRotation,10000,2)+2000)*iDir,
		       Cos(iRotation,1000),     Sin(iRotation,1000),       7500-Cos(iRotation, 7500,2)-5200,this(),1);
*/
  return(1);
}

public func UpdateCrosshair(int iDir)
{
  if(Contained() || GetAction() != "Ready") {
    if(pCrosshair) RemoveObject(pCrosshair);
    return(0);
  }
  if(!pCrosshair) {
    pCrosshair = CreateObject(GC4V,0,0,GetOwner(this()));
    ObjectSetAction(pCrosshair,"Crosshair",this());
  }
  if(!iDir) iDir = GetDir()*2-1;
  var iAngle = iRotation+58;
  SetVertexXY(0,(-5-Sin(iRotation,40))*iDir,3+Cos(iRotation,40),pCrosshair);
//  SetVertexXY(0,(Sin(iAngle,3)-7-Sin(iRotation,38))*iDir,1+Sin(iAngle,3)+Cos(iRotation,38),pCrosshair);
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
    AddEffect("IntIsGrabbingCCAN",pByObject,1,1,this(),0,this());
    Local(i,pCrosshair) += i2;
    return(0);
  }
  if(!FindObject2(Find_Owner(iPlr),Find_Action("Push"),Find_ActionTarget(this()),Find_Exclude(pByObject)))
    Local(i,pCrosshair) -= i2;
}

public func FxIntIsGrabbingCCANStart(object pTarget, int iEffectNumber, int iTemp, object pGatling)
{
  if(iTemp) return(0);
  EffectVar(0,pTarget,iEffectNumber) = pGatling;
  // EVar1: Bewegung
  return(1);
}

public func FxIntIsGrabbingCCANTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  if(GetAction(pTarget) != "Push") return(-1);
  if(GetActionTarget(0,pTarget) != EffectVar(0,pTarget,iEffectNumber)) return(-1);
  var iDir;
  if(iDir = EffectVar(1,pTarget,iEffectNumber))
    EffectVar(0,pTarget,iEffectNumber)->~ControlConf(iDir,pTarget);
  return(1);
}

public func FxIntIsGrabbingCCANStop(object pTarget, int iEffectNumber, int iReason, bool fTemp)
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

// Da die Kanone nun in einen besseren Schusswinkel zu stellen ist,
// erhält sie nun eine andere Steuerung für JnR'ler

protected func ControlUp(object pClonk)
{
  // Zielen: Hoch
  [$TxtAimup$|Image=CAN1:2]
  if(GetPlrCoreJumpAndRunControl(GetController(pClonk))) {
    var iEffect = GetEffect("IntIsGrabbingCCAN",pClonk);
    if(!iEffect)
      iEffect = AddEffect("IntIsGrabbingCCAN",pClonk,1,1,this(),0,this());
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
    var iEffect = GetEffect("IntIsGrabbingCCAN",pClonk);
    if(!iEffect)
      iEffect = AddEffect("IntIsGrabbingCCAN",pClonk,1,1,this(),0,this());
    EffectVar(1,pClonk,iEffect) = 1;
    return(1);
  }
  return(AimDown(pClonk,4,"ControlConf"));
}

protected func ControlUpReleased(object pClonk)
{
  var iEffect = GetEffect("IntIsGrabbingCCAN",pClonk);
  if(!iEffect) return(0);
  if(EffectVar(1,pClonk,iEffect) == -1) EffectVar(1,pClonk,iEffect) = 0;
  return(1);
}

protected func ControlDigReleased(object pClonk)
{
  var iEffect = GetEffect("IntIsGrabbingCCAN",pClonk);
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
  iRotation = BoundBy(iRotation + iChange, 60, 110);
  UpdateCrosshair();
  UpdateGraphics();
//  Sound("CatapultSet");
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
  if(Fire()) return(1);
  if(szMessage) {
    iPlr = GetOwner(pClonk);
    if(iPlr == -1) return(1);
    PlayerMessage(iPlr, szMessage, this());
    Sound("Error",0,this(),0,iPlr+1);
    return(1);
  }
  return(0);
}

/* Laden und Feuern */

public func RejectCollect(id idObj, object pObj) { return(AllowLoad(idObj) != 0); }

private func AllowLoad(id idObj)
{
  // Nur Kugeln und Pulver	
  if(idObj != GUNP && idObj != CABL)
    return(Format("$TxtPossibleContents$",GetName(0,GUNP),GetName(0,CABL)));		
  // Noch jede Menge Platz
  if(ContentsCount() < 4) return(0);
  // Niemals mehr als 8 Objekte laden: Wird gebraucht, wenn aus ControlThrow aufgerufen!
  //if (ContentsCount() >= 8) return(0);
  // Nicht mehr als 4 Schießpulver laden
  if(idObj == GUNP) {
    if(ContentsCount(GUNP) >= 4)
      return(Format("$TxtFull$", ContentsCount(GUNP), GetName(0, GUNP)));
  }
  // Nicht mehr als 4 Projektile laden
  else {
    if(ContentsCount(CABL) >= 4)
      return(Format("$TxtFull$", ContentsCount(CABL), GetName(0, CABL)));
  }
  // Laden o.k.
  return(0);
}

protected func CannonPower(object pObj) { return(120); }

public func Fire(bool fAuto)//object pClonk)
{
  // Schießpulver und Projektil suchen
  var pGunpowder = FindContents(GUNP), pProjectile = FindOtherContents(GUNP);
  // Schießpulver oder Projektil fehlt
  if(!pGunpowder || !pProjectile) {
    Sound("Click");
    return(0);
  }
  // Noch am Nachladen
  // Eins zurueckgeben, damit man beim Nachladen nicht ins Inventar kommt
  if(GetEffect("IntReload", this())) return(1);
  // Schießpulver verbrauchen
  RemoveObject(pGunpowder);

  // Austritt berechnen
  var i, iX, iY, iXDir, iYDir, iAngle, iR, iDir;
  iDir = GetDir()*2-1;
  iAngle = iRotation+58;
  iR = iRotation*10+RandomX(-15,15);
  iX = ( 7-Sin(iAngle,3)+Sin(iRotation,16))*iDir+GetDir();
  iY =  -1-Sin(iAngle,3)-Cos(iRotation,16)-GetDefOffset(GetID(pProjectile),1);
  iXDir =  Sin(iR,CannonPower(),10)*iDir;
  iYDir = -Cos(iR,CannonPower(),10)-5;
  
  // Besitzer setzen
  SetOwner(GetController(), pProjectile);
  
  // Projektil abfeuern
  Exit(pProjectile,iX,iY,Random(360));
  SetXDir(iXDir,pProjectile,10);
  SetYDir(iYDir,pProjectile,10);

  // Sicht verfolgen (wenn kein automatischer Schuss)
  if(!fAuto)
    if(GetPlrViewMode(GetController())!=2)
      SetPlrView(GetController(),pProjectile);
  // Sound
  Sound("Blast2");
  // Rauch & Feuer
  for(i = 0; i < 6; i++) {
    Smoke(iX+RandomX(-5,+5),iY+RandomX(-5,+5),   RandomX( 5,12));
    CreateParticle("Fire2", iX+RandomX(-5,+5),iY+RandomX(-8,+2),iXDir/10,iYDir/10,RandomX(5,12)*5,RGBa(255,255,255,50));
  }
  // Rückstoß  ( rausgenommen, weil immernoch Drehungen auftraten)
/*  var iDir = GetDir();
  SetXDir(-iXDir*2);
  SetDir(iDir);
  // Soll sich wirklich nicht in die andere Richtung drehen
  Schedule(Format("SetDir(%d)", iDir), 1, 2); */
  // Erfolgreich geschossen
  AddEffect("IntReload", this(), 1, 50, this());
  return(1);
}

/* Status */

public func IsHorseWagon() { return(1); }   // Stimmt zwar nicht, aber muss sein, damit anspannbar

private func FindPullingHorse(pHorse)
{
  while(pHorse = FindObject(0,-100,-100,200,200,OCF_Grab,0,0,NoContainer(),pHorse))
    if(GetAction(pHorse) == "Pull" || GetAction(pHorse) == "Pull2" || GetAction(pHorse) == "Pull3" || GetAction(pHorse) == "Turn")
      if(GetActionTarget(0,pHorse) == this())
        return(pHorse); 
  return(0);
}

/* Verbindung */

public func Connect(object pHorse)
{
  CreateObject(CHBM,0,0,GetOwner())->CHBM::Connect(pHorse,this());
  SetGraphics(0,this(),0,1);
  SetGraphics(0,this(),0,2);
  Schedule("SetAction(\"Drive0\")", 1);
  Sound("Connect");
  return(1);
}

public func Disconnect(pFromHorse)
{         
  var pBeam = FindObject(CHBM,0,0,0,0,0,"Connect",pFromHorse);
  if(pBeam) RemoveObject(pBeam);
  SetGraphics("Head",this(),GetID(),1,2,"Head");
  SetGraphics("Body",this(),GetID(),2,6);
  UpdateGraphics();
  SetAction("Ready");
  Sound("Connect");
  return(1);
}

public func Driving()
{
  // Kein Pferd
  if(!FindPullingHorse()) {
    SetGraphics("Head",this(),GetID(),1,2,"Head");
    SetGraphics("Body",this(),GetID(),2,6);
    UpdateGraphics();
    return(SetAction("Ready"));
  }
  // Bewegung
  if(GetXDir() != 0 && GetAction() != "Drive1")
    return(SetAction("Drive1"));
  if(GetXDir() == 0 && GetAction() != "Drive0")
    return(SetAction("Drive0"));
}