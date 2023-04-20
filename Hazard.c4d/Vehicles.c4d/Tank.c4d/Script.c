/*-- Tank --*/

#strict

#include L_TK	// tank physics

/* Initialization */
/* ------------------------------------------------------------------------- */

public func Initialize() {			// Initialization

	// Action
	SetAction("Drive");
	SetDir(DIR_Left);

	// Create turret
	NewTurret(TurretID());

	// Create chassis
	NewChassis(ChassisID());
	
	shieldupgrade = false;
	aiming = false;
	
	// Damage effect
	AddDamageEffect(this);
	
	return(_inherited());
}

/* Turret and Chassis */
/* ------------------------------------------------------------------------- */

local turret, chassis;				// turret, chassis

public func TurretID() { return(TRRT); }
public func ChassisID() { return(CHSS); }

public func NewTurret(id idTurret) {		// Set new turret
	// delete old turret if there were one
	if(turret) RemoveObject(turret);

	// create the new one
	turret = CreateObject(idTurret,0,5,GetOwner());
	turret -> AttachTo(this());
	SetObjectOrder(this(), turret);
}

public func NewChassis(id idChassis) {		// set new chassis
	// delete old chassis if there were one
	if(chassis) RemoveObject(chassis);

	// create the new one
	chassis = CreateObject(idChassis,0,18,GetOwner());
	chassis -> AttachTo(this());
	SetObjectOrder(this(), chassis);
}

// the turret has the lights

public func ContextLight(pCaller) {
  [$SwitchLight$|Image=FLSH]
  turret->~SwitchLights();
  return(1);
}

/* Aiming */
/* ------------------------------------------------------------------------- */

local aiming;
local crosshair;
local aimmode;

public func ReadyToAim() { return(!IsJumping()); }
public func AimOverride() { return(IsJumping()); }
public func IsAiming() { return(aiming); }

public func MaxAimUp() {
	return(turret->~MaxAimUp());
}

public func MaxAimDown() {
	return(Min(110,turret->~MaxAimDown()));
}

// almost copied from hazard...:

public func StartAiming() {     // Anfangen zu zielen
	Stop();
	
	crosshair = CreateObject(HCRH,0,0,-1);
	crosshair->Init(this());
	crosshair->SetAngle(80*(GetDir()*2-1));
	
	if(IsTurning()) crosshair->SetVisibility(VIS_None);

	// Callback
	aiming = true;
	turret->~AimStart();
}

public func StopAiming() {      // Zielen beenden
  
	if(crosshair) RemoveObject(crosshair);
 
	// Callback
	aiming = false;
	turret->~AimStop();
}

// copied from hazard

public func ContainedConf(int conf)
{
  // Feineres Zielen mit JnR-Steuerung möglich
  if(GetPlrCoreJumpAndRunControl(GetController()))
    DoAiming(conf * AIM_Step / 5);
  else
    DoAiming(conf * AIM_Step);
}

protected func ContainedUpdate(object clonk, int comdir, bool dig, bool throw)
{
  if(IsAiming())
  {
    AimUpdate(this(), comdir, 1, "ContainedConf");
    return(1);
	}
	if(IsTurning())
		if(comdir != COMD_Stop)
		{
			Go();
			return(1);
		}
	if(IsDriving())
		if(comdir == COMD_Stop)
		{
			Stop();
			return(1);
		}
		
  return(_inherited(clonk, comdir, dig, throw));
}

public func DoAiming(int iChange) {
  //zielen wir überhaupt?
  if(!IsAiming())
    return();

  var angle = Abs(crosshair->GetAngle()) + iChange;

  // Winkel anpassen, wenn keine freie Auswahl (klassische Steuerung)
  if(!GetPlrCoreJumpAndRunControl(GetController()))
    angle = angle-angle%AIM_Step;

  // Winkel wird zu groß?
  if(angle > MaxAimDown() || angle < MaxAimUp())
    return();

  if(GetDir() == DIR_Left)
    angle = 360-angle;
  crosshair->SetAngle(angle);

  UpdateAiming();
}

public func UpdateAiming() {
  if(!IsAiming())
    return();

  var a = crosshair->GetAngle();

  turret->~SetAimAngle(a);
}

/* Callbacks */
/* ------------------------------------------------------------------------- */

public func Damage() {							// Damage
	if(GetDamage() < MaxDamage()) return();

	Destroyed();
}

protected func Destruction() {					// Destruction
	if(turret) RemoveObject(turret);
	if(chassis) RemoveObject(chassis);
}

protected func ActivateEntrance(object by) {	// Entrance
  if(by->GetOCF() & OCF_CrewMember) {
    if(by == Contents()) {
      PauseTurn();
      Exit(by,0,20);
      return true;
    }
    else if(!ContentsCount()) {
      Enter(this, by);
      return true;
    }
  }
}

protected func Ejection(object by) {
	if(!EMPShocked()) {
		Sound("tank_stop",0,0,SLvl(0));
		Sound("tank",0,0,SLvl(0),0,-1);
		Sound("tank2",0,0,SLvl(1),0,-1);
	}
	if(!Contents())
	{
		turret->CancelReload();
		if(IsDark()) turret->~TurnLights(false);
		Message("@",this);
    DoScore(GetOwner(),+GetValue());
    SetOwner(NO_OWNER);
	}
}

protected func Collection2(object by) {
	SetEntrance(0);
	if(!(GetOCF(by) & OCF_CrewMember)) return;

	DriverIsOwner(by);
	if(!EMPShocked()) {
		Sound("tank_start",0,0,SLvl(0));
		Sound("tank",0,0,SLvl(0),0,1);
		ResumeTurn();
	}
	if(IsDark()) turret->~TurnLights(true);

}

// nur CrewMember dürfen hier rein
protected func RejectCollect(id idObj, object pObj) {
	if(GetOCF(pObj) & OCF_CrewMember) return(0);
	return(1);
} 

private func DriverIsOwner(object driver) {
  var owner = GetOwner(driver);
	DoScore(GetOwner(),+GetValue());
	SetOwner(owner);
	DoScore(owner,-GetValue());
	for(var i = 0; i < GetPlayerCount(); i++)
    if(i != owner)
      PlayerMessage(GetPlayerByIndex(i), "@%s",this(),Format("%s %s",GetTaggedPlayerName(owner),GetTaggedTeamName(owner)));
}

/* Sound  */
/* ------------------------------------------------------------------------- */

private func SLvl(bool drive) {
	if(!drive) return(50);
	if(drive) return(100);
} 

public func OnGo() {
	if(!IsTurning()) EngineSoundStart();
}

public func OnStop() {
	if(!IsTurning()) EngineSoundStop();
}

public func OnJumpStart() {
	EngineSoundStop();
	Sound("Flamer",0,0,80);
}

public func OnJumpEnd() {
	if(!stop) EngineSoundStart();
}

public func OnTurnStart() {
	if(stop) EngineSoundStart();
}

public func OnTurnEnd() {
	if(stop) EngineSoundStop();
}

private func EngineSoundStart()	{ Sound("tank2",0,0,SLvl(1),0,+1); }
private func EngineSoundStop()	{ Sound("tank2",0,0,SLvl(1),0,-1); }

/* Status  */
/* ------------------------------------------------------------------------- */

public func IsBulletTarget() { return(1); }

public func MaxDamage()		{ return(750); }

public func IsThreat()		{ return(!!ContentsCount()); }

/* Control */
/* ------------------------------------------------------------------------- */

public func ContainedLeft(object driver) {	// Left
    [$TxtLeft$|Image=TAN1:3]
	if(EMPShocked()) return(true);
	if(GetDir() != DIR_Left) {
		if(!IsTurning()) {
			if(BottomContact())
				Turn();
		}
	}
	else {
		if(!IsDriving())
			if(!IsAiming())
				Go();
	}
	return(true);
}

public func ContainedRight(object driver) {	// Right
    [$TxtRight$|Image=TAN1:4]
	if(EMPShocked()) return(true);
	if(GetDir() != DIR_Right) {
		if(!IsTurning())
			if(BottomContact())
				Turn();
	}
	else {
		if(!IsDriving())
			if(!IsAiming())
				Go();
	}
	return(true);
}

public func ContainedDown(object driver) {	// Down
    [$TxtAimDown$|Image=TAN1:2]
	if(IsAiming()) {
		AimDown(this(), 1, "ContainedConf");
		return(1);
	}
	if(IsDriving()) {
		Stop();
		return(1);
	}
	else {
		if(GetPlrDownDouble(GetOwner(driver))) {
			StartAiming();
		}
	}
	//PauseTurn();
	return(1);
}

public func ContainedUpDouble(object driver) {	// UpDouble
    [$TxtOut$|Image=TAN1:6]
	if(IsAiming()) {
		AimUp(this(), 1, "ContainedConf");
		return(1);
	}
	if(!IsDriving()) {
		PauseTurn();
		Exit(driver,0,20);
	}
}

public func ContainedLeftDouble(object driver) {	// LeftDouble
    [$TxtStopAim$|Image=TAN1:7]
	if(IsAiming()) {
		StopAiming();
		return(1);
	}
}

public func ContainedRightDouble(object driver) {	// RightDouble
    [$TxtStopAim$|Image=TAN1:7]
	if(IsAiming()) {
		StopAiming();
		return(1);
	}
}

public func ContainedUp(object driver) {	// Up
    [$TxtAimUp$|Image=TAN1:1]
	if(IsAiming()) {
		AimUp(this(), 1, "ContainedConf");
		return(1);
	}
	if(Jump()) {
		for(var i=0; i<20; ++i)
			CreateDustParticle(RandomX(-10,10),RandomX(20,300));

		//var dir = GetDir()*2-1;
		//Schedule(Format("MuzzleFlash(20, Object(%d), %d, -2, %d, %d)",ObjectNumber(this()),-21*dir -1, -90*dir, RGB(255,150,0)),2,5,this());
	}
}

public func ContainedThrow(object driver) {	// Throw (Fire)
    [$TxtFire$|Image=TAN1:0]
	if(EMPShocked()) return(true);
	// forward to turret: select the fire mode
	turret->ControlThrow(driver);
	return(true);
}

public func ContainedDigDouble(object driver) {	// Dig Double (fire mode)
    [$TxtFireMode$|Image=TAN1:5]
	if(EMPShocked()) return(true);
	// forward to turret: select the fire mode
	turret->ControlDigDouble(driver);
	return(true);
}

public func ControlCommand(string strCommand, object pTarget, int iTx, int iTy, object pTarget2, int iData, object pCmdObj) {
	if(strCommand eq "MoveTo") {
	
		if(IsAiming()) {
			//Winkel wird zu groß?
			var iAngle = Normalize(Angle(GetX(),GetY(),iTx,iTy)-GetR(),-180);
			
			// falsche Richtung? (nur drehen)
			if(iAngle < 0 && GetDir() == DIR_Right ||
			   iAngle > 0 && GetDir() == DIR_Left) {
			   
			   Turn();
			   // danach!
			   turret->ControlThrow(pCmdObj);
			   return(1);
			}
			
			iAngle = Abs(iAngle);
			iAngle = BoundBy(iAngle,MaxAimUp(),MaxAimDown());

			if(GetDir() == DIR_Left)
				iAngle = 360-iAngle;
			
			crosshair->SetAngle(iAngle);
			UpdateAiming();
			
			ScheduleCall(turret,"ControlThrow",5,0,pCmdObj);
		}
		else
		  turret->ControlThrow(pCmdObj);
		// do not exit
		return(1);
	}
}

/* Turning */
/* ------------------------------------------------------------------------- */

public func Turn() {
	SetAction("Turn");

	OnTurnStart();
	if(crosshair) crosshair->SetVisibility(VIS_None);
	
	chassis -> Turn();
	turret -> Turn();
}

protected func ChangeDir() {

	// turn dir 
	if(GetPhase() == 15) {
		if(GetDir() == DIR_Left)
			SetDir(DIR_Right);
		else
			SetDir(DIR_Left);
			
		if(crosshair) crosshair->ChangeDir();
	}
	
	if(GetPhase() == 29) {
	
		if(crosshair) {
			crosshair->SetVisibility(VIS_Owner);
		}
		
		OnTurnEnd();
	}
}

public func ResumeTurn() {
	// resume turning only if paused
	if(GetAction() ne "TurnStalled") return();
	var p = GetPhase();
	SetAction("Turn");
	SetPhase(p);
}

public func PauseTurn() {
	// pause turning only if turning
	if(GetAction() ne "Turn") return();
	var p = GetPhase();
	SetAction("TurnStalled");
	SetPhase(p);
}

/* HUD */
/* ------------------------------------------------------------------------- */

public func ReadyToFire() { return(!IsTurning()); }

public func UpdateCharge(object driver) {

	if(!turret) return false;

	var hud = driver->GetHUD();
	if(hud)	hud->Update(turret, driver->AmmoStoring(),driver);
	return true;
}


/* Graphical effects */
/* ------------------------------------------------------------------------- */
// mid term: most sbould be general functions

protected func Timer() {
	
	DrivingAnimation();		// Animation beim Fahren
	DustEffect();			// Aufwirbeln von Staub
}

public func GetDmgEffectPos(&x, &y) {

	var turn = 100;
	if(GetAction() ne "Drive")
		turn = 100*Abs(15 - GetPhase())/15;
	x = Cos(GetR(),-(GetDir()*2-1)*13*turn/100)+Sin(GetR(),-4);
	y = Sin(GetR(),-(GetDir()*2-1)*13*turn/100)+Cos(GetR(),-4);
}

private func DustEffect() {

	if(IsDriving() || IsTurning()) {
		CreateDustParticle((GetDir()*2-1)*10,RandomX(20,300));
	}
}

private func CreateDustParticle(int dir, int size) {
		// Dust effect
		var groundx = Sin(GetR(),-YOffsToWheels()-3);
		var groundy = Cos(GetR(),+YOffsToWheels()+3);
		var mat = GetMaterial(groundx, groundy);

		if(CheckDust(mat)) {
		
			var rand = Random(3);
			var r = GetMaterialColor(mat,rand,0);
			var g = GetMaterialColor(mat,rand,1);
			var b = GetMaterialColor(mat,rand,2);
			var alpha = 160;
			var xdir = -Max(26-Abs(dir*2),0)*dir/Abs(dir);
			var x = -Sin(GetR(),YOffsToWheels()) + Cos(GetR(),-dir);
			var y = Cos(GetR(),YOffsToWheels()) + Sin(GetR(),-dir);
			CreateParticle("Dust",x,y,Cos(GetR(),xdir+RandomX(-2,2)),RandomX(-5,5),size,RGBa(r,g,b,alpha));
		}
}

private func DrivingAnimation() {
	if(IsDriving()) {
	
		if(!IsTurning()) {
		
			// Fling :D
			var dir = GetDir()*2-1;
			var r = GetR();
			var x = Cos(r,dir*20) -Sin(r,8);
			var y = Sin(r,dir*20) +Cos(r,8);

			for(var obj in FindObjects(Find_Category(C4D_Living),Find_NoContainer(),Find_AtPoint(x,y), Find_OCF(OCF_Alive)))
			{
				if(Hostile(GetOwner(obj),GetController()) || FriendlyFire())
				{
					// fling only stuff that is not flying (= that we did not send flying :-] )
					if(GetProcedure(obj) ne "FLIGHT")
					{
						Fling(obj,-dir,-2);
						DoDmg(20,DMG_Melee, obj, 0, GetController());
					}
				}
			}
		
			var maxphase = GetActMapVal("Length",GetAction());
			if(maxphase > GetPhase()+2)
				SetPhase(GetPhase()+2);
			else {
				if(maxphase == GetPhase()+1) SetPhase(1);
				else SetPhase(0);
			}
		}
	}
}

public func ThrustEffect(int iEffectTime) {
	if(!BottomContact()) {
		if(iEffectTime < 10) {
			  var angle = GetR();
			  var xoff = -(GetDir()*2-1)*8;
			  var yoff = +YOffsToWheels();
			  var x = -Sin(angle,yoff)+Cos(angle,xoff);
			  var y = +Cos(angle,yoff)+Sin(angle,xoff);

			  CreateParticle("Thrust",x,y,-GetXDir()/2,-GetYDir()/2,RandomX(140,200),RGBa(255,200,200,80),0,0);
			  CreateParticle("Smoke2",x,y,-GetXDir()/2,-GetYDir()/2,RandomX(180,220),RGBa(220,200,180,0),0,0);
			  
			  // Stichflamme...
			  if(iEffectTime % 3) {
				  var dir = GetDir()*2-1;
				  xoff = -dir*23;
				  yoff = -3;
				  x = -Sin(angle,yoff)+Cos(angle,xoff);
				  y = +Cos(angle,yoff)+Sin(angle,xoff);
				  MuzzleFlash(20, this, x,y,angle-dir*90, RGB(255,150,0));
			  }
		}
	}
}

/* Damage */
/* ------------------------------------------------------------------------- */

public func OnDmg(int iDmg, int iType) {
	// full resistance to bio and fire damage
	// vulnerable to explosions and energy
	// and a bit resistant vs everything else
	if(shieldupgrade) {
		if(iType == DMG_Energy)		return(60);
		if(iType == DMG_Explosion)	return(20);
	}
	
	if(iType == DMG_Energy)		return(-20);
	if(iType == DMG_Bio)		return(80);
	if(iType == DMG_Melee)		return(60);
	if(iType == DMG_Fire)		return(80);
	if(iType == DMG_Explosion)	return(0);
	
	return(30);
}

public func OnHit(int iDmg, int iType) {

	// nur Energie
	if(iType != DMG_Energy) return(0);
	// Schild flackert auf
	EffectCall(this(), GetEffect("HazardShield", this()), "BulletHit");
}

public func Destroyed() {
	// exit all drivers
	var bla;
	while(bla = Contents()) bla->Exit();
	
	// remove turret and chassis
	if(turret) {
		turret->Explosive();
		turret = 0;
	}
	if(chassis) {
		chassis->RemoveObject();
	}
	
	// bumm
	var tank = CreateObject(DTNK,0,19,-1);
	tank->SetAction("Be");
	tank->SetR(GetR());
	tank->SetDir(GetDir());
	Explode(40);
}

/* EMP */
/* ------------------------------------------------------------------------- */

public func IsMachine() { return(1); }

public func EMPShock() {
	// at emp-shock: stop, pause turning and assign a shock-effect
	Stop();
	PauseTurn();
	if(turret) turret->PauseReload();
	EMPShockEffect(30*35);
	Sound("tank_stop",0,0,SLvl(0));
	Sound("tank",0,0,SLvl(0),0,-1);
	Sound("tank2",0,0,SLvl(1),0,-1);
	return(1);
}

public func EMPShockEnd() {
	// at the end of the shock-effect: the tank may continue to turn
	if(Contents()) {
		ResumeTurn();
		if(turret) turret->ResumeReload();
		Sound("tank_start",0,0,20);
		Sound("tank",0,0,20,0,1);
	}
}

/* Upgrade */
/* ------------------------------------------------------------------------- */

local shieldupgrade;

public func IsUpgradeable(id idObj) {
	if(idObj == HSHD && !shieldupgrade)
		return("$UpgradeShield$");
	return(false);
	}

public func Upgrade(id idObj) {
	if(idObj == HSHD) {
		shieldupgrade = true;
		AddEffect("HazardShield", this(), 99, 3, 0, HSHD, 400);
		return(1);
	}
}

public func CanCarryFlag() { return true; }