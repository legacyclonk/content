/*-- Pumpgun --*/

#strict

#include WEPN

local rifleupgrade;

public func HandSize() { return(800);  }
public func HandX()    { return(6000); }
public func HandY()    { return(500);  }
public func BarrelYOffset(){return(-1000);}

public func FMData1(int data)
{
  if(data == FM_Name)      return("$Standard$");
  if(data == FM_AmmoID)    return(STAM);
  if(data == FM_AmmoLoad)  return(30);
  if(data == FM_AmmoUsage) return(6);

  if(data == FM_Reload)    return(80);
  if(data == FM_Recharge)  return(30);

  if(data == FM_Damage)    return(32);

  return(Default(data));
}

public func BotData1(int data)
{
  if(data == BOT_Range)    return(120);
  if(data == BOT_Power)    return(BOT_Power_2);

  return(Default(data));
}

public func FMData2(int data)
{
  if(data == FM_Name)      return("$Cluster$");
  if(data == FM_AmmoID)    return(GRAM);
  if(data == FM_AmmoLoad)  return(2);
  if(data == FM_AmmoUsage) return(1);

  if(data == FM_Reload)    return(80);
  if(data == FM_Recharge)  return(70);

  if(data == FM_Condition) return(GetUpgrade(KRFL));

  if(data == FM_Damage)    return(16);

  return(Default(data));
}

public func BotData2(int data)
{
  if(data == BOT_Range)    return(120);
  if(data == BOT_DmgType)  return(DMG_Explosion);
  if(data == BOT_Ballistic)  return(true);
  if(data == BOT_Power)    return(BOT_Power_3);

  return(Default(data));
}

public func Fire1()    // Projektilschuss
{ 
  var user = GetUser();
  var x,y; user->WeaponEnd(x,y);
  var angle = user->AimAngle(20);
  var ammo;

  var dir = GetDir(user)*2-1;
  
  for(var i=0; i<15; i++)
  {
    ammo = CreateObject(SHT1, x, y, GetController(user));
    ammo -> Launch(angle+RandomX(-7, 7), 250+Random(100), 80+Random(150), 3, 400, GetFMData(FM_Damage, 1), 1);

	if(!(i%3))
		BulletCasing(dir*6, 3, -dir*(Random(15)+5), -Random(15)-15, 5);
  }

  MuzzleFlash(RandomX(40,60), user,x,y,angle);

  // Sound
  Sound("PumpgunFire");
}

public func Fire2()    // Clusterschuss
{
  var user = GetUser();
  var x,y; user->WeaponEnd(x,y);
  var angle = user->AimAngle(20);
  var dir = GetDir(user)*2-1;
  var ammo, xdir, ydir;

  // Effekte
  for(var i=0; i<6; i++)
  {
    ammo = CreateObject(CLSH, x, y, GetController(user));
    ammo -> Launch(angle+RandomX(-7, 7), 250+Random(100), 80+Random(150), 3, 400, GetFMData(FM_Damage, 2), 1);

		if(!(i%3))
			BulletCasing(dir*6, 3, -dir*(Random(15)+5), -Random(15)-15, 5);
  }

  MuzzleFlash(RandomX(40,60), user,x,y,angle);

  // Sound
  Sound("PumpgunFire");
}

/* Upgrades */

public func IsUpgradeable(id uid) {
  if(GetUpgrade(uid))
    return(false);
  
  if(uid == KRFL) return("$KRFLUpgradeDesc$");
}

public func OnUpgrade(id uid) {
	SetUpgradeGraphic();
  if(uid == KRFL) {
    SetFireMode(2);
    var own = GetOwner(Contained());
    if(Contained()) HelpMessage(own,"$KRFLUpgraded$",Contained());
    return(true);
  }
}

public func OnDowngrade(id uid) {
	SetUpgradeGraphic();
  if(uid == KRFL) {
    SetFireMode(1);
    return(true);
  }
}


// Soundeffekte

public func OnReload()
{
  Sound("PumpgunLoad");
}
