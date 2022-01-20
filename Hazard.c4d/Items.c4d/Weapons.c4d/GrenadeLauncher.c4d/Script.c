/*-- Granatenwerfer --*/

#strict

#include WEPN // Das Objekt ist eine Waffe  

// Anzeige in der Hand
public func HandSize() { return(1050); }
public func HandX() { return(5000); }
public func HandY() { return(1000); }

public func FMData1(int data)
{
  if(data == FM_Name)     return("$Grenade$");
  if(data == FM_Icon)     return(GREN);
  if(data == FM_AmmoID)   return(GRAM);
  if(data == FM_AmmoLoad) return(6);

  if(data == FM_Reload)   return(160);
  if(data == FM_Recharge) return(90);

  if(data == FM_Damage)   return(30);
  if(data == FM_Condition) return(!GetUpgrade(KRFL));

  return(Default(data));
}

public func BotData1(int data)
{
  if(data == BOT_Range)    return(300);
  if(data == BOT_DmgType)  return(DMG_Explosion);
  if(data == BOT_Ballistic)  return(true);
  if(data == BOT_Power)    return(BOT_Power_3);

  return(Default(data));
}

public func FMData2(int data)
{
  if(data == FM_Name)     return("$FragGrenade$");
  if(data == FM_Icon)     return(CGRN);
  if(data == FM_AmmoID)   return(GRAM);
  if(data == FM_AmmoLoad) return(8);
  if(data == FM_AmmoUsage)return(2);
  if(data == FM_Reload)   return(170);
  if(data == FM_Recharge) return(90);

  if(data == FM_Condition) return(GetUpgrade(KRFL));

  if(data == FM_Damage)   return(14);

  return(Default(data));
}

public func BotData2(int data)
{
  if(data == BOT_Range)    return(300);
  if(data == BOT_DmgType)  return(DMG_Explosion);
  if(data == BOT_Ballistic)  return(true);
  if(data == BOT_Power)    return(BOT_Power_3);

  return(Default(data));
}

public func FMData3(int data)
{
  if(data == FM_Name)     return("$Napalm$");
  if(data == FM_Icon)     return(NGRN);
  if(data == FM_AmmoID)   return(GSAM);
  if(data == FM_AmmoLoad) return(60);
  if(data == FM_AmmoUsage)return(20);
  if(data == FM_Reload)   return(180);
  if(data == FM_Recharge) return(60);

  if(data == FM_Condition) return(GetUpgrade(KRFL));

  if(data == FM_Damage)   return(30);

  return(Default(data));
}

public func BotData3(int data)
{
  if(data == BOT_Range)    return(300);
  if(data == BOT_DmgType)  return(DMG_Fire);
  if(data == BOT_Ballistic)  return(true);
  if(data == BOT_Power)    return(BOT_Power_2);

  return(Default(data));
}

public func FMData4(int data)
{
  if(data == FM_Name)     return("$Slime$");
  if(data == FM_Icon)     return(SGRN);
  if(data == FM_AmmoID)   return(GRAM);
  if(data == FM_AmmoLoad) return(2);

  if(data == FM_Reload)   return(180);
  if(data == FM_Recharge) return(90);

  if(data == FM_Damage)   return(8);

  if(data == FM_Condition) return(GetUpgrade(KSLM));
  return(Default(data));
}

public func BotData4(int data)
{
  if(data == BOT_Range)    return(200);
  if(data == BOT_DmgType)  return(DMG_Bio);
  if(data == BOT_Ballistic)  return(true);
  if(data == BOT_Power)    return(BOT_Power_3);

  return(Default(data));
}

public func FMData5(int data)
{
  if(data == FM_Name)     return("$EMP$");
  if(data == FM_Icon)     return(EGRN);
  if(data == FM_AmmoID)   return(ENAM);
  if(data == FM_AmmoLoad) return(30);
  if(data == FM_AmmoUsage)return(30);
  if(data == FM_Reload)   return(60);

  if(data == FM_Condition) return(GetUpgrade(KLAS));

  if(data == FM_Damage)   return(400);

  return(Default(data));
}

public func BotData5(int data)
{
  if(data == BOT_Range)    return(300);
  if(data == BOT_DmgType)  return(0);
  if(data == BOT_Ballistic)  return(true);
  if(data == BOT_Power)    return(BOT_Power_1);
  if(data == BOT_EMP)      return(true);

  return(Default(data));
}

public func FMData6(int data)
{
  if(data == FM_Name)     return("$Laser$");
  if(data == FM_Icon)     return(LGRN);
  if(data == FM_AmmoID)   return(ENAM);
  if(data == FM_AmmoLoad) return(45);
  if(data == FM_AmmoUsage)return(15);
  if(data == FM_Reload)   return(180);
  if(data == FM_Recharge) return(100);

  if(data == FM_Condition) return(GetUpgrade(KLAS));

  if(data == FM_Damage)   return(50);

  return(Default(data));
}

public func BotData6(int data)
{
  if(data == BOT_Range)    return(300);
  if(data == BOT_DmgType)  return(DMG_Energy);
  if(data == BOT_Ballistic)  return(true);
  if(data == BOT_Power)    return(BOT_Power_2);

  return(Default(data));
}

public func Fire1() { LaunchGrenade(GREN, 90,Contained()->~AimAngle( 5)+RandomX( -5, 5),1); }
public func Fire2() { LaunchGrenade(CGRN, 70,Contained()->~AimAngle(10)+RandomX(-10,15),2); }
public func Fire3() { LaunchGrenade(NGRN, 80,Contained()->~AimAngle( 5)+RandomX( -5, 5),3); }
public func Fire4() { LaunchGrenade(SGRN,100,Contained()->~AimAngle( 5)+RandomX( -5, 5),4); }
public func Fire5() { LaunchGrenade(EGRN, 80,Contained()->~AimAngle( 5)+RandomX( -5, 5),5); }
public func Fire6() { LaunchGrenade(LGRN, 80,Contained()->~AimAngle( 5)+RandomX( -5, 5),6); }

// verschießt eine Granate
// idg:   ID der Granate
// speed: Geschwindigkeit
// angle: Winkel
public func LaunchGrenade(id idg, int speed, int angle, int mode) {
  var user = Contained();
  var dir = GetDir(user)*2-1;

  // Adjust angle
  // special on jetpack...
  if(user->GetAction() ne "JetpackFlight")
	angle = BoundBy(angle+GetYDir(user)*dir,-360,360);
  // calculate speed
  var xdir = Sin(angle,speed);
  var ydir = -Cos(angle,speed);

  var x,y;
  user->WeaponEnd(x,y);

  // create and launch
  var grenade=CreateObject(idg, x+xdir/10, y+ydir/10, GetController(user));
  grenade->Launch(xdir+GetXDir(user)/2, ydir+GetYDir(user)/2, GetFMData(FM_Damage, mode));

  // effect
  Sound("GrenadeFire");

  CreateParticle("Thrust",x,y,GetXDir(user),GetYDir(user),80,RGBa(255,200,200,0),0,0);

  for(var i=0; i<20; ++i) {
    CreateParticle("Smoke2",x+RandomX(-5,+5),y+RandomX(-5,+5),
                   GetXDir(user)+RandomX(0,xdir/4),GetYDir(user)+RandomX(0,ydir/4),
                   RandomX(80,140),RGBa(200,200,200,0),0,0);
  }
}

/* Upgrade */

public func IsUpgradeable(id uid) {
  if(GetUpgrade(uid))
    return(false);
  
  if(uid == KSLM) return("$KSLMUpgradeDesc$");
  if(uid == KLAS) return("$KLASUpgradeDesc$");
  if(uid == KRFL) return("$KRFLUpgradeDesc$");
}

public func OnUpgrade(id uid) {
	SetUpgradeGraphic();
  var own = GetOwner(Contained());
  if(uid == KSLM) {
    // Feuermodus wechseln
    SetFireMode(4);
    if(Contained()) HelpMessage(own,"$KSLMUpgraded$",Contained());
    return(true);
  }
  if(uid == KLAS) {
    // Feuermodus wechseln
    SetFireMode(6);
    if(Contained()) HelpMessage(own,"$KLASUpgraded$",Contained());
    return(true);
  }
  if(uid == KRFL) {
    // Feuermodus wechseln
    SetFireMode(2);
    if(Contained()) HelpMessage(own,"$KRFLUpgraded$",Contained());
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
    SetFireMode(1);
    return(true);
  }
  if(uid == KRFL) {
    SetFireMode(1);
    return(true);
  }
}


// Soundeffekte

public func OnReload()
{
  Sound("GrenadeLoad");
}
