#strict

local w, l, r, c, iTime, pDmgCallback, pAttach, pLight, iMaxDist, dx, dy;
local reflections;

protected func Initialize()
{
  SetObjectBlitMode(1);
  SetClrModulation(RGB(255,0,0));
}

/* Interface (public functions) */

public func SetAngle(int iAngle) {
   r = iAngle-180;
}
public func SetWidth(int iWidth) {
   w = 1000*iWidth/GetActMapVal("Facet","Laser",0,2);
}
public func SetMaxDistance(int iDist) {
   iMaxDist = iDist;
}
public func SetContinuous(int iDelay) {
  if(!iDelay) iDelay = 1;
  AddEffect("Continuous", this(), 1,iDelay, this());
}

public func Set(int iAngle, int iWidth, int iDist, int iT, object pDmg, object pAtt, int iReflect)
{
  // optional parameters
  if(!iWidth) iWidth = 3;
  if(!iDist) iDist = 300;
  // iT = 0: no out-fading
  iTime = iT;
	reflections = iReflect; // how often the Ray should be reflected

  // reset fade-timer
  c=0;
  // new attach and damage callback objects (optional)
  pDmgCallback = pDmg;
  pAttach = pAtt;
  if(pAttach) {
    dx = GetX()-GetX(pAttach);
    dy = GetY()-GetY(pAttach);
  }

  // create light
  pLight = CreateObject(LALI,0,0,-1);

  SetMaxDistance(iDist);
  SetAngle(iAngle);
  SetWidth(iWidth);
  HitCheck();
  DrawTransform();
  
 	// we are stuck?
	if(Stuck())
		RemoveObject();
}

public func GetAngle()  { return(r+180); }
public func GetWidth()  { return(w*GetActMapVal("Facet","Laser",0,2)/1000);  }
public func GetLength() { 
  if(GetAction()eq"Idle")
    return(l*GetActMapVal("Facet","Laser",0,3)/1000);
  else
    return(l*GetActMapVal("Facet",GetAction(),0,3)/1000);
}

public func GetMaxDistance() { return(iMaxDist); }

public func LaserEnd(&x, &y) {
  x = +Sin(GetAngle(),GetLength());
  y = -Cos(GetAngle(),GetLength());
}

/* intern */

private func HitCheck() {

  // calculate length
  var mx=-Sin(r,iMaxDist),
      my=+Cos(r,iMaxDist),
      ml=0;
  while(++ml) {
    if(GBackSolid(mx*ml/iMaxDist,my*ml/iMaxDist)) break;
    if(ml>=iMaxDist) break;
  }
  // graphic
  SetLaser(ml == iMaxDist);

  l = 1000*ml/GetActMapVal("Facet",GetAction(),0,3);

  LaserEnd(mx,my); 
  
  // no hit check
  if(!pDmgCallback)
    return();
  
  if(ml < iMaxDist)
    pDmgCallback ->~ LaserHitLandscape(mx,my);

  // clonk = user of the weapon that fires (the shooter). Will be given with clonk
  var clonk;
  clonk = pDmgCallback->~GetUser();
  if(!clonk) clonk = pDmgCallback;
	
	var temp_l, hitobject;
	
  // search points on the line for clonks etc.
  // step for step
  for(pObj in FindObjects(Find_OnLine(0, 0, mx, my), Sort_Distance(0,0),
              Find_Exclude(this()),
              Find_Exclude(pAttach),
              Find_Exclude(pDmgCallback),
              Find_Exclude(Contained(pDmgCallback)),
              Find_NoContainer(),
              Find_Or(
              	Find_Func("IsBulletTarget",GetID(),this(),clonk),
              	Find_OCF(OCF_Alive)),
              Find_Func("CheckEnemy",this())))
  {
  	// we have to save this before, if the object is removed on hit
  	temp_l = 1000*ObjectDistance(pObj)/GetActMapVal("Facet",GetAction(),0,3);;
  	if(pDmgCallback ->~ LaserStrike(pObj,c))
  	{
  		SetLaser(0);
  		l = temp_l;
  		hitobject = true;
  		break;
  	}
  }
  if(ml>0 && ml != iMaxDist && reflections && !hitobject && !Stuck())
		CheckGround(ml);
}

private func NotZero(int a) {
  if(!a) return(1);
  else return(a);
}

private func SetLaser(bool end) {
  if(end) {
    if(GetAction() ne "LaserEnd") {
      SetAction("LaserEnd");
    }
  }
  else {
    if(GetAction() ne "Laser") {
      SetAction("Laser");
    }
  }
}

protected func Laser() {		// fade out
  // attach to obj
  if(pAttach) {
		var wx,wy;
		pAttach->~WeaponEnd(wx,wy);
		if(wx || wy)
			SetPosition(GetX(pAttach) + wx, GetY(pAttach) + wy);
		else
	    SetPosition(GetX(pAttach)+dx,GetY(pAttach)+dy);
	   
		SetPosition(GetX(),GetY(),pLight);
  }

  // animation & effect
  SetPhase(Random(4));
  if(GetAction() eq "Laser" && !reflections) {
    var x,y;
    LaserEnd(x,y);
    var color = GetClrModulation();
    LaserLight(20+GetWidth()*2,color,x,y);
    var a,r,g,b;
    SplitRGBaValue(color,r,g,b,a);
    a = BoundBy(a+64,180,255);
    CreateParticle("PSpark",x,y,0,0,300+GetWidth()*30,RGBa(r,g,b,a));
  }

  if(!iTime) return();

  // fade out (only if time != 0)
  var a = 200*c/iTime;
  var rgba = SetRGBaValue(GetClrModulation(),a,0);
  SetClrModulation(rgba);
  ++c;

  if(c >= iTime)
    RemoveObject();
}

protected func Destruction() {
  if(pLight) RemoveObject(pLight);
}


private func DrawTransform() {		// draw line

  var cl = l;
  cl = l*GetActMapVal("Facet","Laser",0,3)/GetActMapVal("Facet",GetAction(),0,3);

  var fsin = -Sin(r, 1000), fcos = Cos(r, 1000);

  var xoff = -(GetActMapVal("Facet",GetAction(),0,2)*w/1000)/2;
  var yoff = 0;

  var width = +fcos*w/1000, height = +fcos*cl/1000;
  var xskew = +fsin*cl/1000, yskew = -fsin*w/1000;

  var xadjust = +fcos*xoff + fsin*yoff;
  var yadjust = -fsin*xoff + fcos*yoff;

  // set matrix values
  SetObjDrawTransform (
    width, xskew, xadjust,
    yskew, height, yadjust
  );
  
  // light
  if(pLight) {
    var alphamod, sizemod;
    CalcLight(alphamod, sizemod);

    var light_width = w*(5+sizemod/100);
    var light_height = cl/2;
    xoff = -(GetActMapVal("Facet","Laser",LALI,2)*light_width/1000)/2;
    xadjust = +fcos*xoff + fsin*yoff;
    yadjust = -fsin*xoff + fcos*yoff;
    width =  +fcos*light_width/1000;
    height = +fcos*light_height/1000;
    xskew =  +fsin*light_height/1000;
    yskew =  -fsin*light_width/1000;
    
    var a,r_,g,b;
    SplitRGBaValue(GetClrModulation(),r_,g,b,a);
    SetClrModulation(RGBa(r_,g,b,Min(a+160+alphamod,180)),pLight);
    SetPosition(GetX(),GetY(),pLight);
    
    SetObjDrawTransform (
      width, xskew, xadjust,
      yskew, height, yadjust, pLight
    );
  }
}

private func FxContinuousTimer(object target, int effect, int time) {
  // ein bischen Hax: wenn der Laser bleibt, wird auch der Winkel
  // angepasst, falls der Target AimAngle hat (Clonk hat das)
  var angle;
  if(pAttach) {
    if(angle = (pAttach->~AimAngle(1)))
      SetAngle(angle);
    if((GetDir(pAttach) == DIR_Right) && (dx < 0)) dx *= -1;
    if((GetDir(pAttach) == DIR_Left) && (dx > 1))  dx *= -1;
    }
  HitCheck();
  DrawTransform();
}


// uber-cool reflection script
func CheckGround(int length)
{
	var x,y;
	LaserEnd(x,y);
	
	var r = Normalize(LocalN("r"),0);
	var angle1, angle2; // Winkel des Materials
	var angle1set, angle2set;
	var rout; //ausgangswinkel
	var i;

	var oldx = x, oldy = y;
	// Adjust position a little bit
	for(i = 0; oldx == x && oldy == y; i++)
	{
		x = oldx+Sin(r, i);
		y = oldy-Cos(r, i);
	}

	// Search for the angle of the solid material
	for(i = 0; i < 180; i++)
	{
		if(GBackSolid(x+Sin(r+i,5), y-Cos(r+i,5)) && !angle1set)
		{
			angle1set = true;
		}
		if(GBackSolid(x+Sin(r-i,5), y-Cos(r-i,5)) && !angle2set)
		{
			angle2set = true;
		}	
		if(!angle2set)
		{
			angle1 = r-i;
		}
		if(!angle1set)
		{
			angle2 = r+i;
		}
	}

	//nichts gefunden? wir müssen im Material feststecken.
	if(!angle1set || !angle2set)
		return();
	
	var angle = Normalize(Angle(Sin(angle1,5),-Cos(angle1,5),Sin(angle2,5),-Cos(angle2,5)),0);
	
	// calculate the angle of the new laser
	rout = Normalize(angle*2 - (r+180),0);

	oldx = x; oldy = y;
	// adjust position
	for(i = 0; GBackSolid(x,y); i++)
	{
		x = oldx+Sin(rout, i);
		y = oldy-Cos(rout, i);
		if(x < 0 || x > LandscapeWidth()) return;
		if(y < 0 || y > LandscapeHeight()) return;
	}

	// create new laser
	var newlaser = CreateObject(GetID(), x,y, GetOwner());
	newlaser->SetClrModulation(GetClrModulation());
	newlaser->Set(rout, GetWidth(), iMaxDist-length, iTime, pDmgCallback, pAttach, reflections-1);
}
