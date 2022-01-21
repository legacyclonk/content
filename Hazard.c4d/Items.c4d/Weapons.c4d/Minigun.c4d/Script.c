/*-- Minigun --*/

#strict

#include WEPN

// Anzeige in der Hand
public func HandSize() { return(900);  }
public func HandX()    { return(11000); }
public func HandY()    { return(-3000); }
public func BarrelYOffset(){return(-1000); }

// Daten für ersten Feuermodus
public func FMData1(int data)
{
  if(data == FM_Name)       return("Standard");
  if(data == FM_AmmoID)     return(STAM);
  if(data == FM_AmmoLoad)   return(50);

  if(data == FM_Reload)     return(260);
  if(data == FM_Recharge)   return(3);

  if(data == FM_AmmoUsage)  return(1);
  if(data == FM_AmmoRate)   return(2);
  if(data == FM_Auto)       return(true);
  if(data == FM_Aim)        return(-1);

  if(data == FM_Condition) return(!GetUpgrade(KSLM));

  if(data == FM_Damage)     return(4);

  return(Default(data));
}

public func BotData1(int data)
{
  if(data == BOT_Range)    return(500);
  if(data == BOT_Power)    return(BOT_Power_LongLoad);

  return(Default(data));
}

// Daten für ersten Feuermodus
public func FMData2(int data)
{
  if(data == FM_Name)       return("$Bouncing$");
  if(data == FM_AmmoID)     return(STAM);
  if(data == FM_AmmoLoad)   return(50);

  if(data == FM_Reload)     return(220);
  if(data == FM_Recharge)   return(3);

  if(data == FM_AmmoUsage)  return(1);
  if(data == FM_AmmoRate)   return(2);
  if(data == FM_Auto)       return(true);
  if(data == FM_Aim)        return(-1);

  if(data == FM_Condition) return(GetUpgrade(KSLM));

  if(data == FM_Damage)     return(4);

  return(Default(data));
}

public func BotData2(int data)
{
  if(data == BOT_Range)    return(500);
  if(data == BOT_Power)    return(BOT_Power_LongLoad);

  return(Default(data));
}

// Feuern im ersten Feuermodus
public func Fire1()		// ungenaues aber heftiges Minigunfeuer
{ 
  var user = Contained();
  // Winkel: maximal 5° anvisieren, Streuung von 8°
  var angle = user->AimAngle(5)+RandomX(-4,+4)-2*(GetDir(user)*2-1);
  // Schuss an der Mündung der Waffe erstellen...
  var x,y;
  user->WeaponEnd(x,y);
  var ammo = CreateObject(SHT1,x,y,GetController(user));
  ammo->Launch(angle,300,600,4,100,GetFMData(FM_Damage, 1));
  // Effekte
  MuzzleFlash(RandomX(20,50),user,x,y,angle);
  
  user->~WeaponBegin(x,y);
  var dir = GetDir(user)*2-1;
  BulletCasing(x,y,-dir*Cos(angle-35*dir,40+Random(20)),-dir*Sin(angle-35*dir,40+Random(20)),5);
}

// Feuern im zweiten Feuermodus
public func Fire2()		// ungenaues aber heftiges Minigunfeuer mit Abpralleffekt
{ 
  var user = Contained();
  // Winkel: maximal 5° anvisieren, Streuung von 8°
  var angle = user->AimAngle(5)+RandomX(-4,+4);
  // Schuss an der Mündung der Waffe erstellen...
  var x,y;
  user->WeaponEnd(x,y);
  var ammo = CreateObject(SHT1,x,y,GetController(user));
  ammo->Launch(angle,300,600,3,100,GetFMData(FM_Damage, 2), 0,0,0, 3);
//public func Launch(int iAngle, int iSpeed, int iDist, int iSize, int iTrail, int iDmg, int iDmgPrec, int iGlowSize, int iAPrec, int iReflections)
  // Effekte
  MuzzleFlash(RandomX(20,50),user,x,y,angle);
  
  user->~WeaponBegin(x,y);
  var dir = GetDir(user)*2-1;
  BulletCasing(x,y,-dir*Cos(angle-35*dir,40+Random(20)),-dir*Sin(angle-35*dir,40+Random(20)),5);
}

// Soundeffekte

public func OnReload()
{
  Sound("MiniLoad");
}

public func OnAutoStart()
{
  Sound("MiniGun",0,0,0,0,1);
  Sound("MiniTurn",0,0,0,0,1);
}

public func OnAutoStop()
{
  Sound("MiniGun",0,0,0,0,-1);
  Sound("MiniTurn",0,0,0,0,-1);
}

/* Upgrade */

public func IsUpgradeable(id uid) {
  //wir haben das Upgrade schon, bah.
  if(GetUpgrade(uid))
    return(false);

  if(uid == KSLM) return("$KSLMUpgradeDesc$");
}

public func OnUpgrade(id uid) {
	SetUpgradeGraphic();
  var own = GetOwner(Contained());
  if(uid == KSLM) {
    SetFireMode(2);
    if(Contained()) HelpMessage(own, "$KSLMUpgraded$", Contained());
    return(true);
  }
}

public func OnDowngrade(id uid) {
	SetUpgradeGraphic();
  if(uid == KSLM) {
    SetFireMode(1);
    return(true);
  }
}
