/* Tank functionality */

#strict

/* Adds the tanks physics to the vehicle ----

The vehicle may not have any vertices. There are only four virtual vertices
 (see below). The line
   return(_inherited());
 has to be added as the last line of the function Initialize to get it working.
 
 The control, when the vehicle should stop, go, jump, turn (SetDir) is up to you. 
 For a player controlled vehicle, it could look like this:
 
	 public func ContainedLeft() {
		if(GetDir() != DIR_Left) {
			SetDir(DIR_Left);
		}
		else if(!IsDriving()) Go();
}
 
 The rest of the configuration is explained below.

Interface:
Jump() - Jumps if it is driving on the ground
Go() - Drives on
Stop() - Stops
For changing the direction just use SetDir();

*/

/* Physics */
/* ------------------------------------------------------------------------- */

// Jumping - Jump()
public func JumpStrengthX() { return(40); }	// Strength
public func JumpStrengthY() { return(30); }
public func JumpDelay()		{ return(50); }	// minimum Delay until next Jump

// The vehicle has 4 virtual vertices. One left, one right, one on top, one on bottom
public func VertexLeft()	{ return(24); }
public func VertexRight()	{ return(24); }
public func VertexTop()		{ return(12); }
public func VertexBottom()	{ return(20); }
// VertexBottom should be bigger than XOffsetToWheels otherwise
// the vehicle may loose contact all the time

// X- and Y-Offset to wheels (which are 2) from the center: assumes that the distance
// to the wheels are the same from the center.
public func XOffsToWheels() { return(18); }
public func YOffsToWheels() { return(15); }

public func MaxSpeed()		{ return(1800); }	// maximum speed
public func BrakeStrengh()	{ return(200); }	// speed of braking
public func Acceleration()	{ return(200); }	// speed of speeding up

public func TurnAction()	{ return("Turn"); }	// name of turning-action. Leave out if there is none.

//public func CatLanding()	{ return(5); }

public func SpiderMode()	{ return(true);}	//experimental drive-on-walls and ceiling
public func MaxRotation()	{ return(70); }		// max. rotation of landscape it can stand (0 for no max rotation)

/* Callbacks */
/* ------------------------------------------------------------------------- */

public func OnGo() {}
public func OnStop() {}
public func OnJumpStart() {}
public func OnJumpEnd() {}

/* Initialization */

local stop;
local speed;

local left, right, top;
local groundl, groundm, groundr;
local xr,yr, xm, ym, xl, yl;
local falling;
// more precise r (*rp)
local rpos;
// rotation precision
local rp;

protected func Initialize() {
	stop = true;
	speed = 0;
	falling = false;
	rp = 100;
	
	AddEffect("Physics",this(),1,1,this());
	
	return(_inherited());
}


/* Jumping */

public func IsJumping() { return(GetEffect("JumpDelay",this())); }

public func Jump() {
	if(!IsTurning())
		if(IsDriving())
			if(!TopContact() && !LeftContact() && !RightContact())
				if(BottomContact())
					if(!IsJumping()) {
						var dir = (GetDir()*2-1)*1;
						
						var strx = JumpStrengthX() * dir;
						var stry = JumpStrengthY();
						var r = GetR(0,rp);
						
						SetXDir(+Sin(r,stry,rp)+Cos(r,strx,rp));
						SetYDir(-Cos(r,stry,rp)+Sin(r,strx,rp));
						SetRDir(dir);
						AddEffect("JumpDelay",this(),1,1,this(),0,JumpDelay());
						
						OnJumpStart();
						
						return(true);
					}
}

protected func FxJumpDelayTimer(object pTarget, int iEffectNumber, int iEffectTime) {
	// Effekt
	pTarget->~ThrustEffect(iEffectTime);
	
	EffectVar(0,pTarget,iEffectNumber)--;
	if(EffectVar(0,pTarget,iEffectNumber) <= 0) return(-1);
}

protected func FxJumpDelayStart(object pTarget, int iEffectNumber, int iTemp, int iTime) {
	EffectVar(0,pTarget,iEffectNumber) = iTime;
}

public func ThrustEffect(int iEffectTime) { }

protected func FxPhysicsTimer(object pTarget, int iEffectNumber, int iEffectTime) {
	
	SetR(GetR(0,rp),0,rp);
	
	ContactCheck();			// Kontakt prüfen
	
	// on ground?
	if(IsStuck()) {
		StuckStop();
		falling = false;
	}
	else if(BottomContact()) {
		if(falling) {
			falling = false;
			OnJumpEnd();
		}
		// driving
		if(IsDriving()) Drive();
		// Fix rotation and pos
		Stand();
	}
	else {
		// else fall
		Fall();
		falling = true;
	}
	
}

/* Find ground/walls */

public func BottomContact()	{
	if(IsJumping()) {
		if(GetEffect("JumpDelay",this(),0,6) < 10)
			return(false);
	}
	return(groundm < VertexBottom() || groundl < VertexBottom() || groundr < VertexBottom());
}
public func TopContact()	{ return(top < VertexTop()); }
public func LeftContact()	{ return(left < VertexLeft()); }
public func RightContact()	{ return(right < VertexRight()); }
public func IsStuck()		{ return(groundm < VertexBottom()-15); }

private func ContactCheck() {

	right =	FindGround(-4,2);
	top = FindGround(0,1);
	left =	FindGround(-4,3);
	
	groundm = GroundPos(0,0,xm,ym);
	groundl = GroundPos(-12,0,xl,yl);
	groundr = GroundPos(+12,0,xr,yr);
}

protected func FindGround(int x, int dir, bool downisdown) {
	var xpos, ypos;
	return(GroundPos(x,dir,xpos,ypos,downisdown));
}

protected func GroundPos(int x, int dir, &xpos, &ypos, bool downisdown) {

	var startx, starty, distx, disty, rev;

	var r = GetR(0,rp);
	if(downisdown) r = 0;
	
	// Ground that is further away than about 30px is not relevant for the tank
	var dist;
	// down = 0, up = 1, right = 2, left = 3
	if(dir == 0) { dist = VertexBottom()*2; rev = 0; }
	if(dir == 2) { dist = VertexRight()*2; rev = 0; }
	if(dir == 1) { dist = -VertexTop()*2; rev = 1; }
	if(dir == 3) { dist = -VertexLeft()*2; rev = 1; }
	
	// up or down
	if(dir == 0 || dir == 1) {
		startx = Cos(r,x,rp);
		starty = Sin(r,x,rp);
		distx = Sin(r,-dist,rp);
		disty = Cos(r,dist,rp);
	}
	// left or right
	if(dir == 2 || dir == 3) {
		// a bit further down
		startx = Sin(r,-x,rp);
		starty = Cos(r,+x,rp);
		distx = Cos(r,dist,rp);
		disty = Sin(r,dist,rp);
	}
	
	var i = 0;
	var phase = 0;
	//  search...
	while(Abs(i) <= Abs(dist)) {
		var newx = startx+distx*i/dist;
		var newy = starty+disty*i/dist;

		// landscape borders
		if(GetX()+newx < 0 || GetX()+newx >= LandscapeWidth() || GetY()+newy >= LandscapeHeight()) {
			if(phase == 0) { phase = 1; continue; }
			else break;
		}
		// sky found... keh
		if(phase == 0) {
			if(rev ^ GBackSolid(newx,newy)) { phase = 1; continue; }
			++i;
		}
		// solid found... keh
		if(phase == 1) {
			if(rev ^ !GBackSolid(newx,newy)) break;
			--i;
		}
	}

	// i is our man!
	var groundx = startx+distx*i/dist;
	var groundy = starty+disty*i/dist;
	
	xpos = groundx;
	ypos = groundy;
	
	if(rev) i = -i;
	
	// if at the bottom and it is open: fall down
	if(GetScenBottomOpen())
		if(groundy+GetY()==LandscapeHeight())
			return(1000);

	// returns the y-pos where there is ground ... 1000 if not found
	return(i);
}

/*  Rotation and Position */

public func SetRAt(int angle, int xoffs, int offs, int prec) {
	if(!prec) prec = 1;
	var r = GetR(0,prec);
	//rotate
	var ox = Sin(r,offs,prec)-Cos(r,xoffs,prec);
	var oy = Cos(r,offs,prec)+Sin(r,xoffs,prec);
	SetR(angle,0,prec);
	r = GetR(0,prec);
	var nx = Sin(r,offs,prec)-Cos(r,xoffs,prec);
	var ny = Cos(r,offs,prec)+Sin(r,xoffs,prec);
	
	var xpos = nx-ox;
	var ypos = -ny+oy;
	
	SetPosition(GetX()+xpos,GetY()+ypos);
}

private func FixPosition() {
	var gp = YOffsToWheels();
	// set the distance to ground to YOffsToWheels
	//var ground, xr,yr, xm, ym, xl, yl;
	//var groundm = GroundPos(0,0,xm,ym)-gp;
	//var groundl = GroundPos(-XOffsToWheels(),0,xl,yl)-gp;
	//var groundr = GroundPos(+XOffsToWheels(),0,xr,yr)-gp;
	var ground;
	
	// which point
	var min = Min(groundl,Min(groundm,groundr));
	var best = 0;
	if(min == groundm) { ground = groundm-gp; best = 0; }
	if(min == groundl) { ground = groundl-gp; best = -1; }
	if(min == groundr) { ground = groundr-gp; best = +1;}
	
	var groundx = Sin(GetR(0,rp),-ground,rp);
	var groundy = Cos(GetR(0,rp),ground,rp);
	
	SetPosition(GetX()+groundx,GetY()+groundy);
	
	// set r at the highest
	/*var angle = 0;
	var max = Max(groundl,Max(groundm,groundr));
	if(max == groundm) angle = Angle(xl,yl,xr,yr,rp)-90*rp;
	if(max == groundl) angle = Angle(xm,ym,xr,yr,rp)-90*rp;
	if(max == groundr) angle = Angle(xl,yl,xm,ym,rp)-90*rp;
	
	SetRAt(angle,best*XOffsToWheels(),YOffsToWheels(),rp);
	*/
	//Message("best: %d - angle: %d - %d,%d,%d",this(),best,angle,groundl,groundm,groundr);
	
}

private func GetLandscapeAngle() {
	var x1, y1, x2, y2;
	var left = GroundPos(-XOffsToWheels(),0,x1,y1);
	var right = GroundPos(+XOffsToWheels(),0,x2,y2);
	var angle = Angle(x1,y1,x2,y2,rp)-90*rp;
	angle = Normalize(angle,-180*rp,rp);
	return(angle);
}

private func CheckRotation(int angle) {
	// checks desired rotation against max rotation
	var mrot = MaxRotation()*rp;
	
	if(GetDir() == DIR_Left) {
		if(angle > mrot || angle < Min(-90*rp,-mrot)) return(false);
	}
	else {
		if(angle < -mrot || angle > Max(90*rp,mrot)) return(false);
	}
	return(true);
}

private func FixR(bool d) {
		var angle = GetLandscapeAngle();

		if(!d || CheckRotation(angle)) {
		
			// max rotation
			var mrot = MaxRotation()*rp;
			if(mrot) {
				if(GetDir() == DIR_Left) {
					var buntstift = Min(-90*rp,-mrot);
					if(angle > mrot) angle = mrot;
					else if(angle < buntstift) angle = buntstift;
				}
				else {
					var buntstift = Max(90*rp,mrot);
					if(angle < -mrot) angle = -mrot;
					else if(angle > buntstift) angle = buntstift;
				}
			}
			SetRAt(angle,0,YOffsToWheels(),rp);

		}
}

/* Standing */

private func Stand() {
	SetRDir(0);
	
	FixR(true);
		
	FixPosition();
	
	FixR(true);

}

/* Falling */

protected func Fall() {
	// Y Dir
	// jumping into ceiling
	if(TopContact()) {
		var num;
		if(num = GetEffect("JumpDelay",this(),0))
			RemoveEffect(0,this(),num);
		SetYDir(Max(GetYDir(0,1000),500),0,1000);
	}
	SetYDir(GetYDir(0,1000)+GetGravity()*2,0,1000);
	
	if(LeftContact() || RightContact()) {
		SetXDir(0);
	}
	
	FixR(true);
}

/* Driving */

public func IsDriving() { return(!stop || speed); }
public func Go() { stop = false; OnGo(); }
public func Stop() { stop = true; OnStop(); }

public func IsTurning() { return(GetAction() eq TurnAction()); }

private func Drive() {

	if(!CheckRotation(GetLandscapeAngle()))
	{
		Fall();
		return;
	}

	// stopping
	if(stop) speed = Max(speed-BrakeStrengh(),0);
	// driving
	else speed = Min(speed+Acceleration(),MaxSpeed());
	
	// when turning
	var curspeed = speed;
	if(IsTurning()) {
		var len = GetActMapVal("Length",TurnAction());
		var turn = Abs((GetPhase()-len/2));
		curspeed = Sin(turn*180/len,speed);
	}
	
	// drive faster down, slower up 
	//speed = Sin((GetR(0,rp)*(GetDir()*2-1)+45),2*speed,rp);
	
	// drive
	var dir = (GetDir()*2-1);
	SetXDir(Cos(GetR(0,rp),curspeed,rp)*dir,0,1000);
	SetYDir(Sin(GetR(0,rp),curspeed,rp)*dir,0,1000);
	
	// should we stop?
	// stop if driving into a wall
	if(LeftContact()) if(GetDir() == DIR_Left)		StuckStop();
	if(RightContact()) if(GetDir() == DIR_Right)	StuckStop();
	// on the ceiling??
	if(TopContact()) if(BottomContact())			StuckStop();
}

private func StuckStop() {
	Stop();
	speed = 0;
	SetXDir();
	SetYDir();
}

// interface to outside

public func SetR(int r, object obj, int prec)
{
	if(!prec) prec = 1;
	if(obj == this || !obj) rpos = r*rp/prec;
	return inherited(r/prec,obj);
}

public func GetR(object obj, int prec)
{
	if(!prec) prec = 1;
	if(obj == this || !obj) return rpos*prec/rp;
	return inherited(obj);
}
