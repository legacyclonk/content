/*-- Railgun-Aufsatz --*/

#strict

#include WEPN

local lasupgrade;

public func HandSize() { return(1000); }
public func HandX() { return(7000); }
public func HandY() { return(0); }

public func FMData1(int data)
{
  if(data == FM_AmmoID)    return(ENAM);
  if(data == FM_AmmoUsage) return(1);
  if(data == FM_AmmoRate)  return(100);
  if(data == FM_Auto)      return(false);
  if(data == FM_Aim)       return(0);
  if(data == FM_Condition) return(true);
  if(data == FM_AmmoLoad)  return(100);
  if(data == FM_Reload)    return(60);
  if(data == FM_Recharge)  return(125);
}

func Fire1()    // Peng
{ 
  var user = GetUser();
  var angle = user->AimAngle();
  
  var iX;
  var iY;
  
  user->WeaponEnd(iX,iY);
  
  var laser = CreateObject(LASR, iX, iY, GetController(user));
  laser->SetClrModulation(RGBa(255, 255, 255, 64));
  laser->Set(angle, 13, 500, 30, this(), user);
  laser->SetContinuous(2);
  Sound("RailGunShot");
}

/*public func AimLaser()
{
  var iX;
  var iY;
  WeaponEnd(iX,iY);
  var user = Contained();
  
  var Laser = CreateObject(LASR, iX, iY - 3, GetController(user));
  Laser->SetClrModulation(RGBa(255, 0, 0, 200));
  Laser->Set(GetR(),2,200,0,this(),this());
  Laser->SetContinuous(2);
}*/

func LaserStrike(object pObj) {
  //Schaden machen, natürlich tollen Energiebrazzelschaden!
  DoDmg(6, DMG_Energy, pObj);

  return(0);
}

public func NoWeaponChoice() { return(true); }
