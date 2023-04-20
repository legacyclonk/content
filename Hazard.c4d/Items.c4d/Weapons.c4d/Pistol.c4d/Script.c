/*-- Pistole --*/

#strict

#include WEPN

// Anzeige in der Hand
public func HandSize() { return(800); }
public func HandX()    { return(5500); }
public func HandY()    { return(-1000); }
public func BarrelYOffset() { return(-2700); }

public func OnReload(int i)
{
  if(i == 1) Sound("PistolLoad");
  if(i == 2) Sound("PistolLoad");
  if(i == 3) Sound("GrenadeLoad");
  if(i == 4) Sound("SlimeLoad");
}

public func FMData1(int data)
{
  if(data == FM_Name)      return("$Standard$");
  if(data == FM_AmmoID)    return(STAM);
  if(data == FM_AmmoLoad)  return(12);

  if(data == FM_Reload)    return(80);
  if(data == FM_Recharge)  return(20);
  if(data == FM_Condition) return(!GetUpgrade(KLAS));

  if(data == FM_Damage)    return(10);

  return(Default(data));
}

public func BotData1(int data)
{
  if(data == BOT_Range)    return(400);
  if(data == BOT_Power)    return(BOT_Power_1);

  return(Default(data));
}

//Laser
public func FMData2(int data)
{
  if(data == FM_Name)     return("$Laser$");
  if(data == FM_AmmoID)   return(ENAM);
  if(data == FM_AmmoLoad) return(10);

  if(data == FM_Reload)   return(100);
  if(data == FM_Recharge) return(16);

  if(data == FM_Condition) return(GetUpgrade(KLAS));

  if(data == FM_Damage)    return(18);

  return(Default(data));
}

public func BotData2(int data)
{
  if(data == BOT_Range)    return(300);
  if(data == BOT_DmgType)  return(DMG_Energy);
  if(data == BOT_Power)    return(BOT_Power_2);

  return(Default(data));
}

//Granate
public func FMData3(int data)
{
  if(data == FM_Name)   return("$GrenadeLauncher$");
  if(data == FM_AmmoID)   return(GRAM);
  if(data == FM_AmmoLoad) return(1);

  if(data == FM_Reload)   return(80);

  if(data == FM_Condition) return(!GetUpgrade(KSLM));

  if(data == FM_Damage)    return(30);

  return(Default(data));
}

public func BotData3(int data)
{
  if(data == BOT_Range)    return(50);
  if(data == BOT_DmgType)  return(DMG_Explosion);
  if(data == BOT_Ballistic)  return(true);
  if(data == BOT_Power)    return(BOT_Power_1);

  return(Default(data));
}

public func FMData4(int data)
{
  if(data == FM_Name)   return("$Slime$");
  if(data == FM_AmmoID)   return(STAM);
  if(data == FM_AmmoLoad) return(10);

  if(data == FM_Reload)   return(80);
  if(data == FM_Recharge)   return(20);

  if(data == FM_Condition) return(GetUpgrade(KSLM));

  if(data == FM_Damage)    return(28);

  return(Default(data));
}

public func BotData4(int data)
{
  if(data == BOT_Range)    return(50);
  if(data == BOT_DmgType)  return(DMG_Bio);
  if(data == BOT_Ballistic)  return(true);
  if(data == BOT_Power)    return(BOT_Power_1);

  return(Default(data));
}

public func Fire1()    // Projektilschuss
{ 
  var user = GetUser();
  var angle = user->AimAngle(20) + RandomX(-1,+1);
  var x,y; user->WeaponEnd(x,y);
  var ammo = CreateObject(SHT1,x,y,GetController(user));
  ammo->Launch(angle,250,450,3,300,GetFMData(FM_Damage, 1));

  // Effekte
  MuzzleFlash(40,user,x,y,angle);
  var dir = GetDir(user)*2-1;
  BulletCasing(dir*6,3,-dir*5,-20,5);

  // Sound
  Sound("Pistol",0,ammo);
}

public func Fire2()    // Laserschuss
{ 
  var user = GetUser();
  var angle = user->AimAngle(5);
  var x,y; user->WeaponEnd(x,y);
  var dir = GetDir(user)*2-1;
  var laser = CreateObject(LASR,x,y,GetController(user));
  laser->Set(angle,8,300,15,this());
  // Effekte
  MuzzleFlash(40,user,x,y,angle,RGB(255,32,0));

  // Sound
  Sound("LaserShot",0,laser);
}

public func Fire3()    // Granatenwerfer
{ 
  var user = GetUser();
  var dir = GetDir(user)*2-1;
  var angle = user->AimAngle(5) + RandomX(-5,5);
  var x,y; user->WeaponEnd(x,y);

  var xdir = Sin(angle,50);
  var ydir = -Cos(angle,50);

  // Create & launch
  var grenade=CreateObject(GREN, x+xdir/10,y+ydir/10, GetController(user));
  grenade->Launch(xdir+GetXDir(user)/2,ydir+GetYDir(user)/2, GetFMData(FM_Damage, 3));

  Sound("GrenadeFire");
}

public func Fire4()    // Schleimschuss
{ 
  var user = GetUser();
  var dir = GetDir(user)*2-1;
  var angle = user->AimAngle(5) + RandomX(-5,5);
  var x,y; user->WeaponEnd(x,y);

  var xdir = Sin(angle,50);
  var ydir = -Cos(angle,50);

  // Create & launch
  var slime=CreateObject(SLST, x+xdir/10,y+ydir/10, GetController(user));
  slime->Launch(xdir+GetXDir(user)/2,ydir+GetYDir(user)/2,GetFMData(FM_Damage, 4));

  // Sound
  Sound("SlimeShot*");
}

/* Treffer-Callbacks */

public func LaserStrike(object pTarget) {	// Laser trifft
  if(pTarget)
    DoDmg(GetFMData(FM_Damage, 2), DMG_Energy, pTarget);
  return(1);
}

/* Upgrade */

public func IsUpgradeable(id uid) {
  //wir haben das Upgrade schon, bah.
  if(GetUpgrade(uid))
    return(false);
  
  if(uid == KLAS) return("$KLASUpgradeDesc$");
  if(uid == KSLM) return("$KSLMUpgradeDesc$");
}

public func OnUpgrade(id uid) {
	SetUpgradeGraphic();
  var own = GetOwner(Contained());
  if(uid == KLAS) {
    SetFireMode(2);
    if(Contained()) HelpMessage(own, "$KLASUpgraded$", Contained());
    return(true);
  }
  if(uid == KSLM) {
    SetFireMode(4);
    if(Contained()) HelpMessage(own, "$KSLMUpgraded$", Contained());
    return(true);
  }
}

public func OnDowngrade(id uid) {
	SetUpgradeGraphic();
  if(uid == KLAS) {
    SetFireMode(1);
    return(true);
  }
  if(uid == KSLM) {
    SetFireMode(3);
    return(true);
  }
}
