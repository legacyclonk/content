/*-- MG-Aufsatz --*/

#strict

#include WEPN

public func HandSize() { return(1000); }
public func HandX()    { return(7000); }
public func HandY()    { return(0); }

public func FMData1(int data)
{
  if(data == FM_AmmoID)    return(STAM);
  if(data == FM_AmmoLoad)  return(500);

  if(data == FM_Reload)    return(6);
  if(data == FM_Recharge)  return(6);

  if(data == FM_AmmoUsage) return(1);
  if(data == FM_AmmoRate)  return(1);
  if(data == FM_Auto)      return(1);

//  return(Default(data));
}

public func Fire1()    // Projektil-Dauerfeuer
{
  var user = GetUser();
  var angle = user->AimAngle();
  
  var iX;
  var iY;
  
  user->WeaponEnd(iX,iY);
  
  var ammo = CreateObject(SHT1, iX, iY, GetController(user));
  ammo->Sound("MGFire");
  //int iAngle, int iSpeed, int iDist, int iSize, int iTrail, int iDmg
  ammo->Launch(angle, 300, 600, 3, 100, 44, 1);

  // Effekte
  MuzzleFlash(20,user,iX,iY,angle);


}

public func OnAutoStart()
{
  // wegen einem Bug in der SentryGun nicht loopbar
  //Sound("MGFire",0,GetUser(),0,0,1);
}

public func OnAutoStop()
{
  //Sound("MGFire",0,GetUser(),0,0,-1);
}

public func NoWeaponChoice() { return(true); }
