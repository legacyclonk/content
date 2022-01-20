/*--  GubblBlubbl  --*/

#strict

local pd;
local idAlien;
local hive;
local myalien;
local li, a;
local vein, hasvein;

public func MaxDamage()		{ return(25); }

public func IsAlien()			{ return(true); }
public func IsBulletTarget()	{ return(true); }
public func NoWarp()			{ return(true); }

public func GetAlienID()	{ return(idAlien); }
public func GetMyHive()		{ return(hive); }
public func HasVein()		{ return(hasvein); }
public func GetVeinTarget()	{ return(vein); }

public func Hatch()
{
	if(!idAlien) return();
	
	var alien = CreateObject(idAlien, 0, 0, GetOwner());
	if(hive) hive->AddAlien(alien);

	// Aufbrechen
	Sound("AlienEggBreak*");
	DoDamage(100);
}

// Was werd ich wohl mal werden, wenn ich groß bin?
public func Set(id idA, object by)
{
	idAlien = idA;
	hive = by;
	myalien = 0;
	
	// machen wir mal ein kleines Ding da rein
	SetGraphics(0,0,HBBL,1,GFXOV_MODE_Action,"Alien");
	SetClrModulation(RGBa(255,255,255,200),0,1);
	SetObjDrawTransform(myalien,0,0,0,myalien,0,0,1);
}

public func SetVeinTo(object to) {
	
  // wir ham schon...
  if(vein) return();
  if(to->GetVeinTarget() == this()) return();
  
  var distance = ObjectDistance(to);
  
  // zu nah
  if(distance < 50) return();

  vein = to;
  hasvein = true;
  
  SetGraphics(0,0,HBBL,2,GFXOV_MODE_Action,"Vein");
  SetClrModulation(RGBa(li,li,li,30),0,2);
  
  ShowVein();
}

public func RemoveVein() {
	vein = 0;
	hasvein = false;
	SetGraphics(0,0,0,2);
}

private func ShowVein(int widthMod) {

  if(!vein) return();
  if(!widthMod) widthMod = 1000;

  var angle = Angle(GetX(),GetY(),GetX(vein),GetY(vein))+90;
  var distance = ObjectDistance(vein);
  
  var w = 1000*distance/140;
  var l = widthMod;
  
  var fsin = -Sin(angle, 1000), fcos = Cos(angle, 1000);

  var xoff = -distance/2;
  var yoff = 0;

  var width = +fcos*w/1000, height = +fcos*l/1000;
  var xskew = +fsin*l/1000, yskew = -fsin*w/1000;

  var xadjust = +fcos*xoff + fsin*yoff;
  var yadjust = -fsin*xoff + fcos*yoff;
  
  // set matrix values
  SetObjDrawTransform (
    width, xskew, xadjust,
    yskew, height, yadjust,
	0, 2
  );
}

protected func Initialize()
{
	pd = Random(90);
	DoCon(RandomX(0,25));
	
	// Wir sehen klasse aus!
	SetAction("Bubble");
	SetPhase(Random(7));
	
	// vorne oder hinten
	li = 200;
	var alpha = 0;
	var size = RandomX(50,80);
	var front = 0;
	
	if(Chance(60)) {
		SetCategory(GetCategory() | C4D_Foreground);
		li = 255;
		alpha += 60;
		front = true;
		
	}
	
	// ein bischen Farbmodulation
	a = 40+alpha/2;
	
	SetClrModulation(RGBa(li,li,li,a));
	
	// Aleinnest-hintergrund
	var color = RGBa(li,li,li,alpha);
	AlienHive(size, RandomX(-5,5), RandomX(-5,5), color,this(), front);

}

protected func BubbleBobble() {

	// Effekte
	
	var t = GetActTime()*20;
	
	var pulse = Abs(Pulse(t));
	
	var wdt = 1000+pulse/7;
	var hgt = 1000+Abs(Pulse(t+pd))/7;

	SetObjDrawTransform(wdt,0,0,0,hgt,0,this(),0);
	
	var lo = BoundBy(li-myalien/8,50,255);
	
	SetClrModulation(RGBa(lo,lo,lo,a));
	
	// Eier Bayer
	if(idAlien) {
		myalien += RandomX(1,3);
			
		if(myalien >= 1200) {
			Hatch();
		}
		SetObjDrawTransform(myalien*wdt/1000,0,0,0,myalien*wdt/1000,0,0,1);
		SetClrModulation(RGBa(255,255,255,200-myalien/8),0,1);
	}
	
	// Ader Makaber!
	if(hasvein) {
		if(!vein) RemoveVein();
		else {
			ShowVein(800+Pulse(3*t/2)/20);
		}
	}
}

global func Pulse(int pos) {
	var pulse = Sin(3*pos/5,1000);
	var fire = 1000-Abs(Sin(pos/7+90,1000));
	return( (pulse * fire ) / 1000);
}

// Aua!
public func Damage( int iChange) {
  if(GetDamage() > MaxDamage()) {
    // Effekt
  	AlienSplatter(60*GetCon()/100);
	AlienBlood(60);
	
  GameCallEx("OnAlienDeath", this, -1);
    RemoveObject();
  }
}

public func OnDmg(int iDmg, int iType)
{
  //Melee
  if(iType == DMG_Melee)
    return(50);
  if(iType == DMG_Bio) //Bioresistent.
    return(90);
  if(iType == DMG_Energy) //fast Energieresistent
    return(50);
  if(iType == DMG_Fire)
    return(-50);
}

public func IgnoreFriendlyFire() { return(1); }
public func IsThreat() { return(true); }