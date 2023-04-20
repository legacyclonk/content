#strict 2
#include WEPN

// Flugeigenschaften
local mxspeed, myspeed, xacc, yacc, xbreak, ybreak; // m(ax)
// Flugzustände
local xspeed, yspeed, txspeed, tyspeed; // t(arget)
// Sensoreigenschaften
local shx, shy1, shy2;
local svx1, svx2, svy;
// Sensorzustände
local sb1, sb2;
// Triebwerke
local x1, x2, x3, x4, y1, y2, y3, y4, z1, z2, z3, z4;
// RAUS HIER
local entrance;
local quit;
// neigung (Ziel)
local pitch;
// upgrade
local shieldupgrade;
// lights
local light, light2, lightson;

public func IsThreat()		{ return CrewInside(); }

func Initialize()
{
	_inherited();
	quit = CreateArray();
	// Eingang und Ausgang
	entrance = CreateObject(_ENT)->AttachTo(this,4);
	SetAction("Landed");
	// Flugeigenschaften
	mxspeed = 175; xacc = 5; xbreak = 3*xacc;
	myspeed = 100; yacc = 5; ybreak = 3*yacc;
	// Sensoren horizontal
	shx = Abs(GetVertex(0,0))+25;
	shy1 = GetVertex(2,1)+5;
	shy2 = 2*GetVertex(4,1)-shy1;
	// Sensoren vertikal
	svx1 = GetVertex(2,0);
	svx2 = GetVertex(3,0);
	svy = GetVertex(2,1);
	// Triebwerke
	x1 = x2 = -42; x3 = x4 = -40;
	y1 = -13; y2 = 13; y3 = -15; y4 = 15;
	z1 = z2 = -2; z3 = z4 = 15;
	shieldupgrade = false;
	
	light = AddLightCone(YLightSize(),RGBa(255,255,255,255), this());
	light->ChangeR((GetDir()*2-1)*90+GetR());
	light->ChangeSizeXY(YLightSize(),XLightSize());
	// attach to 5th vertex
	light->SetActionData(5);
	light2 = AddLight(YLightSize()/10, RGBa(255,255,255,255), this());
	// attach to invalid vertex (center)
	light2->SetActionData(6);
	lightson = false;
	
	AddDamageEffect(this);
}

/* Kontrolle */

public func ContextLight(object pCaller) {
  [$SwitchLight$|Image=FLSH]
  SwitchLights();
  return(1);
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


protected func ContainedThrow(object driver)
{
    [$TxtFire$|Image=SAN1:6]
	if(EMPShocked()) return(true);
	ControlThrow(driver);
	return true;
}

protected func ContainedLeft()
{
    [$TxtLeft$|Image=SAN1:0]
	if (IsFlying()) {
		txspeed = -mxspeed;
		tyspeed = yspeed;
		if (GetDir() == DIR_Right) {
			Turn();
		}
	}
	return 1;
}

protected func ContainedLeftDouble()
{
    [$TxtLeft$|Image=SAN1:0]
	if (IsFlying() || IsTurning()) {
		tyspeed = 0;
	}
	return 1;
}

protected func ContainedRight()
{
    [$TxtRight$|Image=SAN1:1]
	if (IsFlying()) {
		txspeed = mxspeed;
		tyspeed = yspeed;
		if (GetDir() == DIR_Left) {
			Turn();
		}
	}
	return 1;
}

protected func ContainedRightDouble()
{
    [$TxtRight$|Image=SAN1:1]
	if (IsFlying() || IsTurning()) {
		tyspeed = 0;
	}
	return 1;
}

protected func ContainedUp()
{
    [$TxtUp$|Image=SAN1:2]
	if (IsFlying() || IsTurning()) {
		tyspeed = -myspeed;
	}
	return 1;
}

protected func ContainedUpDouble()
{
    [$TxtUp$|Image=SAN1:2]
	var i;
	if (IsFlying()  || IsTurning()) {
		txspeed = 0;
	}
	else if (GetAction() == "Landed") {
		// There's a hole in the sky, through which things can fly.
		SetAction("Fly1");
		ContainedUp(Par(0));
		// Musik!
		Sound("Takeoff");
		Sound("Jet",0,0,75,0,+1);
		// die Rostlaube wieder ausbuddeln
		SetR(0);
		i=20;
		while(Stuck() && i--)
			SetPosition(GetX(),GetY()-1);
	}
	return 1;
}

protected func ContainedDown(pController)
{
    [$TxtDown$|Image=SAN1:3]
	if (IsFlying()  || IsTurning()) {
		tyspeed = myspeed;
	} else {
		ExitPassenger(pController);
	}
	return 1;
}

protected func ContainedDownDouble()
{
    [$TxtDown$|Image=SAN1:3]
	if (IsFlying()  || IsTurning()) {
		txspeed = 0;
	}
	return 1;
}

protected func ContainedDig()
{
    [$TxtStop$|Image=SAN1:5]
	if (IsFlying()  || IsTurning()) {
		if (tyspeed) tyspeed = 0;
		else if (txspeed) txspeed = 0;
	}
	return 1;
}

protected func ContainedDigDouble(object driver)
{
    [$TxtFireMode$|Image=SAN1:4]
	if(EMPShocked()) return(true);
	if (IsFlying()  || IsTurning()) {
		if (txspeed) txspeed = 0;
	}
	ControlDigDouble(driver);
	return 1;
}

/* Eingangskontrolle: nur einer zugelassen */


protected func Ejection(object by) {
	if(!CrewInside())
	{
		Message("@",this);
		CancelReload();
		if(IsDark()) TurnLights(false);
    DoScore(GetOwner(),+GetValue());
    SetOwner(NO_OWNER);
	}
}

// nur CrewMember dürfen hier rein
protected func RejectCollect(id idObj, object pObj) {
	if(GetOCF(pObj) & OCF_CrewMember) return(0);
	return(1);
} 

protected func Collection2(object by) {
	SetEntrance(0);
	if(!(GetOCF(by) & OCF_CrewMember)) return;
	DriverIsOwner(by);
  if(IsDark()) TurnLights(true);
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

private func ExitPassenger(object pObj)
{
	if (GetAction() == "Landed") {
		quit[GetLength(quit)] = pObj;
		SetAction("DoorOpening");
	} else
	if (GetAction() == "DoorOpening") {
		quit[GetLength(quit)] = pObj;
	} else
	if (GetAction() == "DoorOpen") {
		Exit(pObj, GetVertex(4,0), GetVertex(4,1)+10);
	}
}

/* Flugverhalten */

private func Timer()
{

	//var cx, pos, x , y, dx, dy, d, i, sy1, sy2, sx, cy1, cy2, ox, oy;
	
	if (IsFlying()  || IsTurning()) {
		var dxspeed, dyspeed;
		dxspeed = txspeed; dyspeed = tyspeed;
		
		var xdir = (xspeed>0)-(xspeed<0);
		var ydir = (yspeed>0)-(yspeed<0);
		
		var r = GetR();
		if (!xdir) {
			xdir = -1+2*GetDir();
			if (IsTurning())
				xdir *= -1;
		}
		
		// Frontsensoren abtasten
		var x11 = GetX() + xdir*Cos(r,shx) - Sin(r,shy1) + xdir*(xspeed**2)/(xacc*2*35);
		var x12 = x11 +xdir*3;
		var x21 = GetX() + xdir*Cos(r,shx) - Sin(r,shy2) + xdir*(xspeed**2)/(xacc*2*35);
		var x22 = x21 +xdir*3;
		var y1 = GetY() + Cos(r,shy1)+xdir*Sin(r,shx) + yspeed*Abs(xspeed)/xacc/35;
		var y2 = GetY() + Cos(r,shy2)+xdir*Sin(r,shx) + yspeed*Abs(xspeed)/xacc/35;
		//CastParticles("MSpark", 10, 5, x11-GetX(), y1-GetY(), 40, 60, RGBa(255,0,0,0));
		//CastParticles("MSpark", 10, 5, x21-GetX(), y2-GetY(), 40, 60, RGBa(255,0,0,0));
		if (!PathFree(x11,y1,x12,y1) || !PathFree(x21,y2,x22,y2)) {
			dxspeed = 0;
		}
		// Landschaft zuende
		var w = LandscapeWidth();
		if(x12 >= w || x12 <= 0 || x22 >= w || x22 <= 0) dxspeed = 0;
		
		// Bodensensoren
		var x30 = GetX() + xdir*Cos(r,svx1) - Sin(r,svy);
		var y30 = GetY() + xdir*Sin(r,svx1) + Cos(r,svy);
		var y31 = y30 + ydir*(yspeed**2)/(yacc*2*25);
		var x40 = GetX() + xdir*Cos(r,svx2) - Sin(r,svy);
		var y40 = GetY() + xdir*Sin(r,svx2) + Cos(r,svy);
		var y41 = y40 + ydir*(yspeed**2)/(yacc*2*25);
		
		//CreateParticle("JetSpark", x30-GetX(), y30-GetY(), 0, 0, 60, RGBa(0,0,255,100));
		//CreateParticle("JetSpark", x40-GetX(), y40-GetY(), 0, 0, 60, RGBa(0,0,255,100));
		
		//CreateParticle("JetSpark", x30-GetX(), y31-GetY(), 0, 0, 60, RGBa(255,0,0,100));
		//CreateParticle("JetSpark", x40-GetX(), y41-GetY(), 0, 0, 60, RGBa(255,0,0,100));
		
		if(!PathFree(x30,y30,x30,y31) || !PathFree(x40,y40,x40,y41)) {
			if(ydir > 0)
				dyspeed = Min(0,dyspeed);
			else
				dyspeed = Max(0,dyspeed);
		}
		
		// Beschleunigung
		xdir = (dxspeed > xspeed)-(dxspeed < xspeed);
		xspeed += xdir*Min(xacc, Abs(dxspeed - xspeed));
		ydir = (dyspeed > yspeed)-(dyspeed < yspeed);
		yspeed += ydir*Min(yacc, Abs(dyspeed - yspeed));
		
		// Geschwindigkeit setzen
		SetXDir(xspeed,0, 35);
		SetYDir(yspeed,0, 35);

		var withspeed = 0;
		var withoutspeed = 0;
		
		// Dies setzt die Drehung bei horizontaler Geschwindigkeit fest
		if(xspeed) {
			if(xspeed > 0)
				withspeed = Abs(xspeed)*(Angle(-1,0,xspeed,yspeed)-90)/mxspeed;
			else if (xspeed<0)
				withspeed = Abs(xspeed)*(Angle(1,0,xspeed,yspeed)-270)/mxspeed;
		}
		// und dies für keine horizontale Geschwindigkeit
		if(Abs(xspeed) < mxspeed) {
			var maxangle = 15;
			var target = (GetDir()*2-1)*maxangle*yspeed/myspeed;

			var r = Normalize(GetR(),-180);
			
			var speed = 1;
			var change = 0;
			if(r < target) change = speed;
			else if(r > target) change = -speed;
			
			withoutspeed = BoundBy(r+change,-maxangle,+maxangle);
			DebugMessage("target: %d, rotation: %d, change: %d, max: %d ... so: %d","Shuttle",target,r,change,maxangle,withoutspeed);

		}
		var psi = GetPhase()+2;
		var at = -(GetDir()*2-1)*50*(30-2*psi)/30;
		
		SetRAt(withspeed*Abs(xspeed)/mxspeed + withoutspeed*(mxspeed-Abs(xspeed))/mxspeed,at,19);
		SetRDir(0);
		
		// Effekte *bümz*
		if (IsFlying())
			EffectsFly();
		else
			EffectsTurn();

		// Hover-Effect
		EffectsHover();
		// Dust-Effect
		EffectsDust();

		// Phase control
		// the faster te higher the pitch
		if (IsFlying()) {
			if(GetYDir() < 0 && GetAction() != "Fly2") SetAction("Fly2");
			if(GetYDir() > 0 && GetAction() != "Fly1") SetAction("Fly1");

			var maxphase = GetActMapVal("Length",GetAction())-1; // cant be bigger than length of action-1
			pitch = Abs(maxphase*yspeed/myspeed);

			Pitch();
		}
		
		UpdateLights();
	}
}

public func SetRAt(int angle, int x, int y) {
	var r = GetR();
	//rotate
	var ox = Sin(r,y) - Cos(r,x);
	var oy = Cos(r,y) + Sin(r,x);
	SetR(angle);
	r = GetR();
	var nx = Sin(r,y) - Cos(r,x);
	var ny = Cos(r,y) + Sin(r,x);
	
	SetPosition(GetX()+nx-ox,GetY()-ny+oy);
}

protected func Pitch() {
	var cphase = GetPhase();

	if(pitch > cphase) SetPhase(cphase+1);
	else if(pitch < cphase) SetPhase(cphase-1);
}

public func Turn() {
	var phase = GetPhase();

	if(GetYDir() > 0) SetAction("Turn1");
	else SetAction("Turn2");

	SetPhase(phase/3);
}

/* Kontakt */

protected func ContactBottom()
{

	if (IsFlying()) {
		if (/*!txspeed && !xspeed &&*/ tyspeed>=0) {
			SetAction("Landed");
			Sound("Jet",0,0,75,0,-1);
			yspeed = tyspeed = 0;
			SetXDir(0);
			SetYDir(0);
		}
	}
}

/* Action Callbacks */

private func DoorOpenStart()
{
	for (var pObj in quit) {
		if (Contained(pObj) == this)
			ExitPassenger(pObj);
	}
}

private func TurnEnd()
{
	SetDir(!GetDir());
	SetVertex(4,0,(-1+2*GetDir())*25,0,1);
}

protected func SoundOpenDoor() {
	Sound("OpenDoor");
}

protected func SoundCloseDoor() {
	Sound("CloseDoor");
}


/* Effects */

private func CreateEngineParticle(int iX, int iY, int iSize, bool fBehind) {
	// the slower the shuttle the more alpha
	var alpha = 65+120*(mxspeed-Abs(xspeed))/mxspeed;
	var size = iSize*5;
	CreateParticle("JetSpark", iX, iY, GetXDir(), GetYDir(), size, RGBa(155,255,205,alpha), this, fBehind);
}

private func CreateHoverParticle(int iX, int iY, int iSize, int iSpeed, int alpha, bool fBehind) {
	var r = GetShuttleR();
	var tx = 0;
	var ty = iSpeed+GetYDir();
	var xr = Cos(r,tx)-Sin(r,ty);
	var yr = Cos(r,ty)+Sin(r,tx);
	var num = 1;
	if(IsTurning()) {
	  var p = GetPhase();
	  if(p > 15) p = 29 - p;
	  
	  if(p < 4) num = 1;
	  else if(p < 7) num = 2;
	  else if(p < 10) num = 3;
	  else if(p < 13) num = 2;
	  else num = 1;
	}
	CreateParticle(Format("HoverSpark%d",num), iX, iY, xr, yr, iSize*5, RGBa(155,255,205,Min(255,140+alpha)), this, fBehind);
}

private func CreateRotatedParticle(int x, int y, int z, int psi, int phi, int r, int size, bool b)
{
	var _x = x*Cos(psi,10000) + y*Sin(psi,100)*Cos(phi,100) - z*Sin(psi,100)*Sin(phi,100);
	var _y = z*Cos(phi,10000) + y*Sin(phi,10000);
	x = Cos(r,_x)-Sin(r,_y);
	y = Cos(r,_y)+Sin(r,_x);
	CreateEngineParticle(x/10000,y/10000, size, b);
}

private func GetPointOnShuttle(&x,&y) {
	var r, tx, ty, psi;

	r = GetShuttleR();
	psi = GetPhase()+2; // azimuth
	tx = (-1+2*GetDir())*x*(30-2*psi)/30;
	ty = y;
	x = Cos(r,tx)-Sin(r,ty);
	y = Cos(r,ty)+Sin(r,tx);
}

private func GetShuttleR() {
	var r = GetR();
	
	if(IsTurning())
	  r += -(70-Abs(GetPhase()-14)*70/15)*(GetDir()*2-1);
	
	return r;
}

private func EffectsHover() {
	
	// wobbling
	var r = Sin(FrameCounter()*40,100, 10);
	var yoff=Cos(FrameCounter()*35,1200, 10);
  var fsin=Sin(r, 1000, 100), fcos=Cos(r, 1000, 100);
  // set matrix values
  SetObjDrawTransform (
    +fcos, +fsin, 0,
    +fsin * (GetDir()*2-1), +fcos, yoff
  );
	
	// Hover effect
	var x = RandomX(-5,5);
	var y = z1+RandomX(24,26);
	
	GetPointOnShuttle(x,y);

	// Particles in front or behind the shuttle
	var behind;
	if(GetAction() == "Turn1" && GetPhase() < 14) behind = true;
	else if(GetAction() == "Turn2" && GetPhase() > 14) behind = true;
	else behind = false;
	
	CreateHoverParticle(x, y+yoff/1000, 140, RandomX(10,20), Abs(75*xspeed/mxspeed), behind);
}

private func EffectsDust() {

	// Dust effect
	var mat,i;
	
	// maximum distance in which the shuttle appears
	var maxdistance = 150;
	
	// search for ground (yomisei: please use your sensor-function for that as soon as you finished it)
	for(i=10; i<maxdistance; i+=5) {
		if(GBackSolid(0,i)) {
			mat = GetMaterial(0,i);
			break;
		}
	}
	
	// ground in distance
	if(i<maxdistance) {
	
		// check if digable
		if(CheckDust(mat)) {
		
			// determine material color
			var rand = Random(3);
			var r = GetMaterialColor(mat,rand,0);
			var g = GetMaterialColor(mat,rand,1);
			var b = GetMaterialColor(mat,rand,2);
			
			// all values dependend on distance
			var size = RandomX(20,300-i/2);
			var alpha = Min(255,120+i);
			var pos = RandomX(0,30);
			// the nearer the dust to the center, the faster it is blown aside 
			CreateParticle("Dust",-pos,i,(-50+pos)+GetXDir()/2,RandomX(-5,5),size,RGBa(r,g,b,alpha));
			CreateParticle("Dust",+pos,i,(+50-pos)+GetXDir()/2,RandomX(-5,5),size,RGBa(r,g,b,alpha));
		}
	}
}

private func EffectsTurn()
{
	var r=GetR(), psi, phi, behind;
	
	// phase of flying is three times more exact
	var phase = GetPhase();
	if(IsFlying()) phase /= 3;
	
	psi = 6*(phase+1); // azimuth
	phi = -(70-Abs(phase-14)*70/15); // bank
	if (GetDir() == DIR_Left) {
		psi -= 180;
		//behind = true;
	}
	// determine if the particles should appear in front or behind the shuttle
	if(GetAction() == "Turn1") behind = true;
	else if(GetAction() == "Fly1") behind = true;
	else behind = false;
	
	CreateRotatedParticle(x1,y1,z1,psi,phi,r,20,behind); //+Abs(txspeed)*8/mxspeed,behind);
	CreateRotatedParticle(x2,y2,z2,psi,phi,r,22,behind); //+Abs(txspeed)*12/mxspeed,behind);
	CreateRotatedParticle(x3,y3,z3,psi,phi,r,20,behind); //+Abs(txspeed)*8/mxspeed,behind);
	CreateRotatedParticle(x4,y4,z4,psi,phi,r,22,behind); //+Abs(txspeed)*12/mxspeed,behind);
}

private func EffectsFly()
{
	if (xspeed) {

		// 4 if one can see it
		if(GetPhase() > 0) {
			EffectsTurn();
		}
		else {

			var tx1 = x1;
			var tx2 = x3;
			var ty1 = z1;
			var ty2 = z3;

			GetPointOnShuttle(tx1,ty1);
			GetPointOnShuttle(tx2,ty2);

			CreateEngineParticle(tx1, ty1, 4+Abs(xspeed)*18/mxspeed);
			CreateEngineParticle(tx2, ty2, 4+Abs(xspeed)*18/mxspeed);

		}
	}
}

private func GetDmgEffectPos(&x, &y) {

	x = -RandomX(14,36);
	y = z1+RandomX(-5,15);

	GetPointOnShuttle(x,y);
}

/* Licht */

public func XLightSize() { return 6000; }
public func YLightSize() { return 3000; }

private func UpdateLights() {
	if(lightson) {
	
		// phase of flying is three times more exact
		var p = GetPhase();
		if(IsFlying()) p /= 3;
		
		// position of the front (goes from +4000 to -4000)
		// at the same time size of the light
		var x = Cos(p*6,XLightSize());
		var absX = Abs(x);
		
		// The direction in which the shuttle seems to be
		var dir = GetDir()*2-1;
		var aniDir = dir;
		if(x < 0) aniDir = -dir;
		
		// maximum angle in which he shuttle seems to pitch during turning
		var maxshuttlepitch = 22;
		
		// direction of Z-turning
		var rotationdir = -1;
		if(GetAction() == "Turn2") rotationdir = 1;
		
		// rotation of the light
		var lightr = aniDir*90+(absX*maxshuttlepitch/XLightSize()-maxshuttlepitch)*rotationdir;
		
		// alpha of the light (vanishes whil turning to Z)
		var alpha = (absX*200/XLightSize());

		// finally set the light
		light->ChangeR(lightr+GetR());
		light->ChangeSizeXY(YLightSize(), absX);
		light->ChangeColor(RGBa(255,255,255,BoundBy(180-alpha,90,255)));
		// light flash from front-lights
		if(GetAction() == "Turn1")
			light2->ChangeColor(RGBa(255,255,255,BoundBy(alpha-10,40,255 )));
		// The position of the light-attach
		SetVertex(5,0,(x/100)*dir,0,1);
	}
}

/* Status */

public func IsTurning() { return WildcardMatch(GetAction(),"Turn*"); }
public func IsFlying() { return WildcardMatch(GetAction(),"Fly*"); }

public func IsBulletTarget() { return true; }

/* Upgrade */

public func IsUpgradeable(id idObj) {
	// Shield upgrade
	if(idObj == HSHD && !shieldupgrade)
		return("$UpgradeShield$");
}

public func Upgrade(id idObj) {

	if(idObj == HSHD && !shieldupgrade) {
		shieldupgrade = true;
		AddEffect("HazardShield", this(), 99, 3, 0, HSHD, 700);
		return true;
	}
	return false;
}

/* Damage */

public func MaxDamage() { return 350; }

public func OnDmg(int iDmg, int iType) {

	if(shieldupgrade) {
		if(iType == DMG_Energy)		return(60);
		if(iType == DMG_Explosion)	return(20);
	}

	if(iType == DMG_Melee)		return(60);
	if(iType == DMG_Bio)		return(80);
	if(iType == DMG_Fire)		return(80);
	if(iType == DMG_Explosion)	return(0);
	if(iType == DMG_Energy)		return(-20);

	return(30);
}

public func OnHit(int iDmg, int iType) {

	// shield flare for energy damage
	if(iType != DMG_Energy) return(0);
	EffectCall(this(), GetEffect("HazardShield", this()), "BulletHit");
}

public func Damage() {
	if(GetDamage() < MaxDamage()) return;

	// Exit all clonks, otherwise they would be removed
	var bla;
	while(bla = Contents()) bla->Exit();
	
	// create debris
	for(var i=0; i<3; ++i) {
		var obj = CreateObject(DSHT);
		obj -> Init(i);
	}
	
	// boom
	Explode(50);
}

protected func Destruction() {
	if(light) light->RemoveObject();
	if(light2) light2->RemoveObject();
}

/* EMP */

public func EMPShock() {
	// can't shoot anymore (see containedthrow)
	EMPShockEffect(50*35);
	
	mxspeed = 120;
	myspeed = 70;
	yacc = 2; ybreak = 3*yacc;
	
	PauseReload();

	return(1);
}

public func EMPShockEnd() {
	mxspeed = 175;
	myspeed = 100;
	yacc = 5; ybreak = 3*yacc;

	ResumeReload();
}

public func IsMachine() { return true; }

/* Waffe */

public func ReadyToFire() {
	if(GetFireMode() != 2)
		return IsFlying();
	// speziell für FireMode = 2
	return IsFlying() || IsTurning();
}

public func NoWeaponChoice() { return true; }

public func FMData1(int data)
{
	if(data == FM_Name)			return("$Missile$");
	if(data == FM_AmmoID)		return(MIAM);
	if(data == FM_AmmoLoad)		return(2);
	if(data == FM_AmmoRate)		return(2);

	if(data == FM_Reload)		return(160);
	if(data == FM_Recharge)		return(10);
	if(data == FM_Auto)			return(true);

	if(data == FM_Damage)		return(25);
	
	return(Default(data));
}

public func FMData2(int data)
{
	if(data == FM_Name)			return("$Bomb$");
	if(data == FM_AmmoID)		return(GRAM);
	if(data == FM_AmmoLoad)		return(3);
	if(data == FM_AmmoUsage)	return(1);

	if(data == FM_Reload)		return(120);
	if(data == FM_Recharge)		return(10);
	if(data == FM_Auto)			return(true);

	if(data == FM_Damage)		return(45);
	
	return(Default(data));
}

public func Fire1()
{ 
	var x = 25;
	var y = z1+30;

	GetPointOnShuttle(x,y);
	
	var speed = Sqrt(GetXDir()*GetXDir()+GetYDir()*GetYDir());
	//Message("%d",this,speed);
	var r = (GetDir()*2-1)*90+GetR();
	var rocket = CreateObject(MISS,x,y,GetOwner(GetUser()));
	rocket->Launch(r, GetFMData(FM_Damage, 1),Max(20,speed));
	
	Sound("Rocket",0,rocket);
}

public func Fire2()
{ 
	var x = 0;
	var y = 30;

	GetPointOnShuttle(x,y);
	
	var r = (GetDir()*2-1)*90+GetR();
	var bomb = CreateObject(ABMB,x,y,GetOwner(GetUser()));
	bomb->Launch(GetXDir(),GetYDir());
	bomb->SetExplo(40);
}

/* HUD */

public func UpdateCharge(object driver) {
	
	var hud = driver->GetHUD();
	if(hud) hud->Update(this(), driver->AmmoStoring(),driver);
	return true;
}

public func GetFlagAttachVertex() { return 7; }
public func CanCarryFlag() { return true; }

public func CrewInside()
{
  for(var i=0; Contents(i); ++i)
    if(Contents(i)->GetOCF() & OCF_CrewMember)
      return true;
  return false;
}