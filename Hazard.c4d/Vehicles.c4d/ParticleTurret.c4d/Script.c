/*-- Particle Cannon --*/

#strict 2
#include FLAK

private func CalculateRifle(&oX, &oY)
{
	var angle = Crosshair->GetAngle() + GetR();

	// Korrektur für Mitte (speziell für diese Grafik)
	var correction = CenterFix();
	
	var x = +Sin(angle, 25-(correction/5));
	var y = -Cos(angle, 15+correction/20);
	
	// circle x,y
	// min->max breite: 0->4
	// min->max höhe: 1->2
	var cx, cy;
	cx = correction/25;
	cy = 2 - correction/90;
	
	x += Sin(30+180*Rifle, cx);
	y += Cos(60+180*Rifle, cy);
	
	if(++Rifle > 1)
		Rifle = 0;
	
	oX = x;
	oY = y-3;
}

/** Waffenzeugs **/

public func FMData1(int data)
{
	if(data == FM_Name)			return("$Auto$");
	if(data == FM_AmmoID)		return(ENAM);

	if(data == FM_AmmoLoad)		return(40);
	if(data == FM_AmmoRate)		return(1);
	if(data == FM_AmmoUsage)	return(8);

	if(data == FM_Reload)		return(100);
	if(data == FM_Recharge)		return(80);

	if(data == FM_Damage)		return(30);
	if(data == FM_Auto)			return(false);

	return(Default(data));
}

public func Fire1()
{
	Sound("ParticleShot");
	ScheduleCall(0,"FireLaser",6,5);
}

public func FMData2() {}


public func FireLaser() {
  var user = GetUser();
  if(!user) return;
  var x,y;
  CalculateRifle(x,y);
  
  var angle = Crosshair->GetAngle();
  var laser = CreateObject(LASR,x,y,GetController(user));
  laser->SetClrModulation(RGB(100,150,255));
  laser->Set(angle,8,450,20,this());

  // Effekte
  MuzzleFlash(250,this,x,y,angle,RGBa(100,100,255,0));

  // Sound
  //Sound("LaserShot",0,laser);
}

public func LaserStrike(object pTarget) {	// Laser trifft
  DoDmg(GetFMData(FM_Damage, 1), DMG_Energy, pTarget);
  
  // geht durch...
  return 0;
}

public func ControlLeft()
{
    [$TxtLeft$|Image=TAN2:1]
	_inherited(...);
}

public func ControlRight()
{
    [$TxtRight$|Image=TAN2:2]
	_inherited(...);
}

public func ControlThrow()
{
    [$TxtFire$|Image=TAN2:0]
	_inherited(...);
}