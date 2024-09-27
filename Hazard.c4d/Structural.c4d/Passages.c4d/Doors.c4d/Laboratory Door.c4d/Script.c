/*-- Labdoor --*/

#strict

#include GBDR

local Status, bBackground;

//Einstellungem

public func Initialize()
{
   SetDir(DIR_Right);
   SetAction("Close");
   
   UpdateSolidMask();
   _inherited();
   SetMaxDamage(100);
}


public func OnDmg(int iDmg, int iType) 
{
  if(iType == DMG_Projectile)   return(40);
  if(iType == DMG_Melee)    return(20);
  if(iType == DMG_Energy)   return(70);
  if(iType == DMG_Explosion)    return(-10);

  return(100);
}

public func OnOpen()
{
  if( bBackground) return(false);
    
  if( GetAction() eq "Open" || Status) return(0);
  
  SetAction("Open");
  SetSolidMask(0);
  Status = true;
  //Sound
  Sound("LabDoorOpen");
}

public func OnClose()
{
  if( bBackground) return(false);
  
  if( GetAction() eq "Close"|| !Status) return(0);  
  SetAction("Close");
  Status = false;
  UpdateSolidMask();
  Sound("LabDoorClose");
}

public func OnDestroyed(iType)
{
    SetAction("Destroyed");
}

public func ShowFront()
{
   bBackground = true;
   SetAction("FrontSight");
   SetSolidMask();
   
   return(bBackground);
}

public func UpdateSolidMask()
{
  if(GetDir() == DIR_Left)
    return(SetSolidMask(14,0,1,30,14,0) );
  else
    return(SetSolidMask(14,0,1,30,1,0) );
}

public func IsBulletTarget() { 
  if(bBackground) return(false);
  return(_inherited());

}
public func CanBeLaserRecycled() { return(2); }

/* Serialisierung */

public func Serialize(array& extra)
{
	_inherited(extra);
	if (destroyed)
		extra[GetLength(extra)] = "SetAction(\"Destroyed\")";
	if (GetDir() == DIR_Left) {
		extra[GetLength(extra)] = "SetDir(DIR_Left)";
		extra[GetLength(extra)] = "UpdateSolidMask()";
	}
	if (bBackground)
		extra[GetLength(extra)] = "ShowFront()";
}
