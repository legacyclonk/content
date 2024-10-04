/*-- Hochenergiegewehr --*/

#strict

#include WEPN

public func HandSize() { return(950); }
public func HandX()    { return(5000); }
public func HandY()    { return(2000); }
public func BarrelXOffset(){return(-3000);}
public func BarrelYOffset(){return(-2000);}

public func FMData1(int data)
{
  if(data == FM_Name)      return("$Standard$");
  if(data == FM_AmmoID)    return(ENAM);
  if(data == FM_AmmoLoad)  return(1);

  if(data == FM_Reload)    return(50);

  if(data == FM_AmmoUsage) return(1);

  return(Default(data));
}

public func BotData1(int data)
{
  if(data == BOT_Range)    return(400);
  if(data == BOT_DmgType)  return(DMG_Energy);

  return(Default(data));
}

public func Fire1()    // Einzelschuss
{
  var user = GetUser();
  var angle = user->~AimAngle(10) + RandomX(-1,1);
  var x,y; user->WeaponEnd(x,y);
  var ammo = CreateObject(SHT3,x,y,GetController(user));
  ammo->Launch(angle,180,400,20,1000);

  // Effekte
  MuzzleFlash(30,user,x,y,angle,RGB(255,64,96));
  // Sound
  Sound("EnergyRifle",0,ammo);
}

public func OnEmpty()
{
  Reload();
}

public func NoWeaponChoice() { return(1); } // Waffe kann nicht gekauft werden
