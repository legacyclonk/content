/*-- Rüstung --*/

#strict
#include GEAR


local pNightVision;	// saves the nightvision-light


/* Status */

public func GetGearType()	{ return(GEAR_Armor); }
protected func Hit()		{ Sound("ClonkHit*"); }
public func IsEquipment()	{ return(true); }

/* Callbacks */

public func GearEffect(object pClonk)
{
  // Change graphics and portrait
  var iPortraitCount = 3; 
  var strGraphics = "Armor";
  SetGraphics(strGraphics,pClonk);
  SetPortrait(Format("%s%d", strGraphics, 1 + Random(iPortraitCount)), pClonk, GetID(pClonk), 0, 0);
  Sound("Connect");
}

public func GearEffectReset(object pClonk)
{
  // reset graphics and portrait
  SetGraphics("",pClonk);
  ResetPortrait(pClonk,false);
  if(pNightVision) RemoveObject(pNightVision);
  Sound("Connect");
}


/* Damage reduction */

public func OnClonkDmg(int iDamage, int iType, object pClonk)
{
  if(!pClonk) return();
  //Melee
  if(iType == DMG_Melee)
    return(60);     // 60% Melee
  //Projectile
  if(iType == DMG_Projectile)
    return(40);     // 40% Projektile
  //Flame
  if(iType == DMG_Fire)
    return(0);
  //Explosion
  if(iType == DMG_Explosion)
    return(20);     // 20% Explosion
  //Energy
  if(iType == DMG_Energy)
    return(-20);    // -20% Energy (!)
  //Bio
  if(iType == DMG_Bio)
    return(25);     // 15% Bio (the clonk has -10% sensivity)
}


/* Use per context menu of the hazard clonk */

public func ConDesc() {
  if(pNightVision) return("$CtxNightVisionOff$");
  return("$CtxNightVisionOn$");
}

public func ConUseable() {
  // without darkness, there is no need for nightvision
  if(IsDark())
    return(true);
}

public func ConUse(object pHazard) {

  // turn on
  if(!pNightVision) {
    pNightVision = AddLight(1200,RGBa(50,255,50,65),pHazard);
    if(!pNightVision)
      return();
    pNightVision->SetVisibility(VIS_Owner);
  }

  // turn off
  else {
    RemoveObject(pNightVision);
  }


  // Attentioooon: Here comes the effect!
  // ..
  // .....!
  Sound("Click"); // Ba-bum.
}
