/*-- Turret --*/

#strict
#include WEPN

local light, light2, lightson;
local aimmode;

/* Turret as a weapon */

public func FMData1(int data)
{
	if(data == FM_Name)			return("$Cannon$");
	if(data == FM_Icon)			return(TBUL);	
	if(data == FM_AmmoID)		return(GRAM);
	if(data == FM_AmmoLoad)		return(1);

	if(data == FM_Reload)		return(30);
	if(data == FM_Recharge)		return(10);

	return(Default(data));
}

public func FMData2(int data)
{
	if(data == FM_Name)			return("$Missile$");
	if(data == FM_Icon)			return(HMIS);	
	if(data == FM_AmmoID)		return(MIAM);
	if(data == FM_AmmoLoad)		return(2);

	if(data == FM_Reload)		return(100);
	if(data == FM_Recharge)		return(30);

	return(Default(data));
}

public func Fire1()				 // fire the cannon
{ 
	// dir
	var r = Normalize(GetR(),-180);
	if(GetDir() == DIR_Right) r = Normalize(r-180,-180);

	// where does it come out?
	var x = +Cos(r,-28);
	var y = -Sin(r,28);
	var strength = 100;
	var xdir = -Cos(r,strength) + GetXDir(GetActionTarget());
	var ydir = -Sin(r,strength) + GetYDir(GetActionTarget());

	// launch!
	var bullet = CreateObject(TBUL,x,y+3,GetController(GetActionTarget()));
	bullet->Launch(xdir, ydir);

	var realr;
	if(GetDir() == DIR_Left) realr = r-90;
	else realr = r-90;
	// Effects
	MuzzleFlash(90,this(),x,y,realr,RGB(255,255,0));
	Sound("TankCannon");


	DoImpulse(-(GetDir()*2-1)*38);

}

public func Fire2()				 // fire the missiles!
{ 
	// dir
	var r = Normalize(GetR(),-180);
	if(GetDir() == DIR_Right) r = r-180;

	// where does it come out?
	var x = +Cos(r,-16);
	var y = -Sin(r,16);
	var ax = +Cos(r,+4);
	var ay = -Sin(r,-4);
	var xdir = (ax - x)*2;
	var ydir = (ay - y)*2;

	var rocket = CreateObject(HMIS,x,y,GetController(GetActionTarget()));
	rocket->Launch(r-90);

	// effects

	CreateParticle("Thrust",ax,ay,xdir/2,ydir/2,80,RGBa(255,200,200,0),0,0);

	for(var i=0; i<20; ++i) {
		var rand = RandomX(-10,+10);
		CreateParticle("Smoke2",ax+Sin(r,rand),ay-Cos(r,rand),
						 RandomX(0,2*xdir),RandomX(0,2*ydir),
						 RandomX(80,140),RGBa(220,200,180,0),0,0);
	}

	Sound("Rocket",0,rocket);

	// the turret 
	DoImpulse(-(GetDir()*2-1)*16);

}

/* aiming */

local aimangle;

public func MaxAimUp() { return(50); }
public func MaxAimDown() { return(110); }

public func SetAimAngle(int a) {
	aimangle = 90-Abs(a);
}

public func AimStop() {
	aimangle = 10;
}

/* engine effect */

protected func Engine() {
	if(GetAction() eq "Explosive") return();
	// set matrix values
	if(GetActionTarget()->Contents() && !(GetActionTarget()->EMPShocked())) {
		SetObjDrawTransform (1000, 0, 0, 0, 1000, Sin(GetActTime()*50,200));
	}
}

/* Explode */

public func Explosive() {
	// if the tank is broken, the turret is flung
	// into the air and will explode if it comes to the ground
	SetAction("Explosive");
	var rand = RandomX(-30,30);
	SetXDir(rand);
	SetYDir(-80);
	SetRDir(rand/4);
	SetClrModulation(RGB(60,60,60));
}

protected func BurnBurnBurn() {
	// call if it is explosive and comes to the ground
	Explode(30);
}

/* Physics	*/

public func DoImpulse(int impulse) {	// impulse to the chassis
	SetRDir(GetRDir() + impulse);
}

public func SetImpulse(int impulse) {
	SetRDir(impulse);
}

private func Timer() {			// set the rotation according to im- and repulse
	SetRDir(BoundBy((GetRDir()*9 + Repulse()*9)/10,-20,20));
}

private func Repulse() {		// repulse from the pneumatic system

	var dir = GetDir()*2-1;
	if(light) light->ChangeR(dir*90+GetR());

	var rdiff = Normalize(GetR() + dir*aimangle - GetR(GetActionTarget()),-180);
	var repulse = 0;

	// the greater the angle, the greater the repulse
	if(rdiff > 0)
		repulse = -rdiff/2;
	else 
		repulse = -rdiff;

	// if small enough, leave it

	return(repulse);
}

public func AttachTo(attachto) {	// attach to tank

	SetDir(DIR_Left);
	SetAction("Attach",attachto);
	light = AddLightCone(1000,RGBa(255,255,255,255), this());
	light->ChangeR((GetDir()*2-1)*90+GetR());
	light->ChangeSizeXY(2000,4000);
	light2 = AddLight(200, RGBa(255,255,255,255), this());
	lightson = false;
	aimmode = false;
	aimangle = 10;
	SetR(aimangle);
}

public func SwitchLights()
{
	TurnLights(!lightson);
}

public func TurnLights(bool on)
{
	lightson = on;
	var alpha = 255;
	if(lightson) alpha = 90;
	light->ChangeColor(RGBa(255,255,255,alpha));
	light2->ChangeColor(RGBa(255,255,255,255));
}

public func Remove() {			// remove
	if(GetAction() eq "Idle")
		RemoveObject();
}

public func Destruction() {
	if(light) light->RemoveObject();
	if(light2) light2->RemoveObject();
}

/* Turning */

public func Turn() {
	SetAction("Turn",GetActionTarget());
	// reset rotation
	SetR(8);
	SetRDir(0);

	if(GetDir() == DIR_Left) {
		SetR(-Normalize(GetR(),-180));
		SetDir(DIR_Right);
	}
	else {
		SetR(+Normalize(GetR(),-180));
		SetDir(DIR_Left);
	}
}

protected func ChangeDir() {

	if(lightson) {
		var at = GetActionTarget();
		var dir = GetDir()*2-1;
		var x = Cos(GetPhase(at)*6,4000);
		if(x < 0) {
			x = -x;
			dir = -dir;
		}
		var alpha = (Abs(x)*200/4000);
		
		light->ChangeSizeXY(2000, x);
		light->ChangeR(Normalize(GetR(at)-90*dir,-180) + dir*10);
		light->ChangeColor(RGBa(255,255,255,BoundBy(180-alpha,90,255)));
		
		if(GetDir() == DIR_Right)
			light2->ChangeColor(RGBa(255,255,255,BoundBy(alpha-10,40,255 )));
	}

	var at = GetActionTarget();
	if(GetAction(at) eq "Turn" || GetAction(at) eq "TurnStalled") {
		return(SetAction("Turn"));
	}
	SetAction("Attach",at);
	SetR(Normalize(GetR(at),-180) - (GetDir()*20-10));
	SetRDir(0);
}


public func NoWeaponChoice() { return(true); }
