/*-- Flammenwefer --*/

#strict

#include WEPN // Das Objekt ist eine Waffe  

// Anzeige in der Hand
public func HandSize() { return(1000); }
public func HandX()    { return(5000); }
public func HandY()    { return(500);  }

public func FMData1(int data)
{
  if(data == FM_Name)      return("$FlameThrower$");
  if(data == FM_AmmoID)    return(GSAM);
  if(data == FM_AmmoLoad)  return(50);
  if(data == FM_Reload)    return(60);
  if(data == FM_Recharge)  return(5);
  if(data == FM_Auto)      return(true);
  if(data == FM_Condition) return(!GetUpgrade(KRFL));

  if(data == FM_Damage)   return(22);

  return(Default(data));
}

public func BotData1(int data)
{
  if(data == BOT_Range)    return(150);
  if(data == BOT_DmgType)  return(DMG_Fire);
  if(data == BOT_Power)    return(BOT_Power_2);

  return(Default(data));
}

public func FMData2(int data)
{
  if(data == FM_Name)     return("$SlimeThrower$");
  if(data == FM_AmmoID)   return(GSAM);
  if(data == FM_AmmoLoad) return(50);

  if(data == FM_Reload)   return(60);
  if(data == FM_Recharge) return(5);
  if(data == FM_Auto)     return(true);  

  if(data == FM_Condition) return(GetUpgrade(KSLM) && !GetUpgrade(KRFL));

  if(data == FM_Damage)   return(29);

  return(Default(data));
}

public func BotData2(int data)
{
  if(data == BOT_Range)    return(150);
  if(data == BOT_DmgType)  return(DMG_Bio);
  if(data == BOT_Power)    return(BOT_Power_3);

  return(Default(data));
}

public func FMData3(int data)
{
  if(data == FM_Name)      return("$FlameThrower$");
  if(data == FM_Recharge)  return(3);
  if(data == FM_Condition) return(GetUpgrade(KRFL));
  return FMData1(data);
}

public func BotData3(int data)
{
  if(data == BOT_Range)     return(300);
  if(data == BOT_DmgType)   return(DMG_Fire);
  if(data == BOT_Ballistic) return(true);
  if(data == BOT_Power)     return(BOT_Power_3);

  return(Default(data));
}

public func FMData4(int data)
{
  if(data == FM_Name)      return("$SlimeThrower$");
  if(data == FM_Recharge)  return(3);
  if(data == FM_Condition) return(GetUpgrade(KSLM) && GetUpgrade(KRFL));
  return FMData2(data);
}

public func BotData4(int data)
{
  if(data == BOT_Range)     return(300);
  if(data == BOT_DmgType)   return(DMG_Bio);
  if(data == BOT_Ballistic) return(true);
  if(data == BOT_Power)     return(BOT_Power_3);

  return(Default(data));
}

public func FireSpray(int fm, bool bio)
{
  var user = GetUser();
  var x,y; user->WeaponEnd(x,y);
  var angle = user->~AimAngle(20);
  var flame = CreateObject(FFLM, x,y, GetController(user));
  var xdir = +Sin(angle,35)+GetXDir(user);
  var ydir = -Cos(angle,35);

  flame -> Launch(xdir,ydir,bio, GetFMData(FM_Damage, fm));
}

public func Fire1()
{
  FireSpray(1,false);
}

public func Fire2()
{
  FireSpray(2,true);
}

public func FireSuperSoaker(int fm, bool bio)
{
  var user = GetUser();
  var x,y; user->WeaponEnd(x,y);
  var angle = user->AimAngle(5)+RandomX(-6,+6)-15*(GetDir(user)*2-1);
  var flame = CreateObject(FLMB, x,y, GetController(user));
  var xdir = +Sin(angle,75);
  var ydir = -Cos(angle,75);

  flame -> Launch(xdir,ydir, bio, GetFMData(FM_Damage, fm));
}

public func Fire3()
{
  FireSuperSoaker(3,false);
}

public func Fire4()
{
  FireSuperSoaker(4,true);
}

/* Sounds */

public func OnAutoStart(int i)
{
  if(i%2) Sound("Flamer",0,0,0,0,1);
  else    Sound("BioFlamer",0,0,0,0,1);
}

public func OnAutoStop(int i)
{
  if(i%2) Sound("Flamer",0,0,0,0,-1);
  else    Sound("BioFlamer",0,0,0,0,-1);
}

public func OnReload()
{
  Sound("FlamerLoad");
}

/* Upgrade */

public func IsUpgradeable(id uid) {
  if(GetUpgrade(uid))
    return(false);
  
  if(uid == KSLM) return("$KSLMUpgradeDesc$");
  if(uid == KRFL) return("$KRFLUpgradeDesc$");
}

public func OnUpgrade(id uid) {
  SetUpgradeGraphic();
  var own = GetOwner(Contained());
  if(uid == KSLM) {
    if(GetUpgrade(KRFL))
      SetFireMode(4);
    else
      SetFireMode(2);
    if(Contained()) HelpMessage(own,"$KSLMUpgraded$",Contained());
    return(true);
  }
  if(uid = KRFL) {
    if(GetUpgrade(KSLM))
      SetFireMode(4);
    else
      SetFireMode(3);
    if(Contained()) HelpMessage(own,"$KRFLUpgraded$",Contained());
    return(true);
  }
}

public func OnDowngrade(id uid) {
  SetUpgradeGraphic();
  if(uid == KSLM) {
    if(GetUpgrade(KRFL))
      SetFireMode(3);
    else
      SetFireMode(1);
    return(true);
  }
  if(uid == KRFL) {
    SetFireMode(1);
    return(true);
  }
}
