/*-- Jetpack --*/

#strict

#include GEAR

//wieviel Zeit bis zum nächsten Boost
local timeleft;
local pHazard;

public func GetGearType()	{ return(GEAR_Backpack); }
public func IsEquipment()	{ return(true); }


public func GearEffect(object pH) {
  pHazard = pH;
}

public func GearEffectReset(object pH) {
  pHazard = 0;
}
/*
public func GetFMData(int d) {
  if(d == FM_AmmoLoad) return(5);
  if(d == FM_AmmoID)   return(GSAM);
}
*/
//Commands, die vom Clonk übergeben werden.

public func ControlUpDouble(object pCon)
{
  // nur als Gear
  if(!( pCon->~HasGear(0,JTPK))) return(0);
  
  if(!CheckStart(pCon)) return(0);
  //Clonk wird hochgechubst (Doppelsprung)
  //Schub
  SetJetpackAction(pCon);
  pCon->SetXDir(GetXDir()/3-10 + 20 * pCon->GetDir());
  pCon->SetYDir(GetYDir()/3-60);
//  AddEffect("Jumping", pCon, 1, 2, pCon);
  pCon->JumpStart();
  
  StartToFly(pCon);
  
  return(1);
}

public func ControlRightDouble(object pCon)
{
  // nur als Gear
  if(!( pCon->~HasGear(0,JTPK))) return(0);
  
  if(!CheckStart(pCon)) return(0);
  //Nun noch herausfinden, ob rechts unsere Blickrichtung ist
  if(pCon->GetDir() == DIR_Right)
    FlyForward(pCon);
  else
    return(0);

  return(1);
}

public func ControlLeftDouble(object pCon)
{
  // nur als Gear
  if(!( pCon->~HasGear(0,JTPK))) return(0);
  
  if(!CheckStart(pCon)) return(0);
  //Nun noch herausfinden, ob links unsere Blickrichtung ist
  if(pCon->GetDir() == DIR_Left)
    FlyForward(pCon);
  else
    return(0);

  return(1);	
}

public func FlyForward(object pClonk)
{
  //Aktion setzen
  SetJetpackAction(pClonk);
  pClonk->SetXDir(pClonk->GetXDir()/3-50 + 100 * pClonk->GetDir());
  pClonk->SetYDir(pClonk->GetYDir()/3-15);
  
  pClonk->JumpStart();

  StartToFly(pClonk);
}

private func StartToFly(object pClonk) {
  //Zeugs fürs Fliegen hinzufügen
  AddEffect("JetpackDelay", pClonk, 170, 5, this());
  AddEffect("Thrust", pClonk, 170, 1, this());
  timeleft = 15;
  //DoAmmo(GetFMData(FM_AmmoID),-GetFMData(FM_AmmoLoad),this());
}

public func CheckStart(object pCon) {
  if(!ReadyToFly(pCon)) return(0);
/*  var ammoload = GetFMData(FM_AmmoLoad);
  var ammoid = GetFMData(FM_AmmoID);
  if(!CheckAmmo(ammoid,ammoload))
    if(!Reload())
      return(0);*/
  return(1);
}
/*
public func Reload(bool nomsg) {
  if(!pHazard) return(false);
  if(reload) return(false);
  
  //checken ob wir genug Sprit zum nochmal fliegen haben
  if(!CheckAmmo(GetFMData(FM_AmmoID),GetFMData(FM_AmmoLoad),pHazard)) {
    var ammoid = GetFMData(FM_AmmoID);
    if(!nomsg) PlayerMessage(GetOwner(pHazard),"$NotEnoughAmmo$",pHazard,ammoid);
    return(false);
  }
  //doch, haben wir, also Tank auffüllen:
  DoAmmo(GetFMData(FM_AmmoID),-GetFMData(FM_AmmoLoad),pHazard);
  DoAmmo(GetFMData(FM_AmmoID),+GetFMData(FM_AmmoLoad),this());
  reload=true;
  return(true);
}
*/
public func ReadyToFly(object pClonk)
{
  // So ist der Clonk in der Luft?
  if(pClonk->GetAction() eq "JumpArmed"
  || pClonk->GetAction() eq "Jump"
  || pClonk->GetAction() eq "Dive"
  || pClonk->GetAction() eq "JetpackFlight") {
    // Dürfen wir überhaupt?
    if(GetEffect("JetpackDelay", pClonk)) return(false);
  
    return(true);
  }
  return(false);
}

public func FxThrustStart(object pTarget, int iEffectNumber) {
  EffectVar(0,pTarget,iEffectNumber) = AddLight(75,RGB(255,127,0),pTarget,GLOW);
  EffectVar(0,pTarget,iEffectNumber)->SetVertex(0,0,GetDir(pTarget)*10-5,0,2);
  Sound("Flamer", false, pTarget, 100,0,+1);
}

public func FxThrustTimer(object pTarget, int iEffectNumber, int iTime) {

  /* halb kopiert aus Missile.c4d -- */
  var dist = Distance(0,0,GetXDir(pTarget),GetYDir(pTarget));
  var angle = Angle(0,0,GetXDir(pTarget),GetYDir(pTarget));
  var maxx = +Sin(angle,dist/10);
  var maxy = -Cos(angle,dist/10);
  var ptrdist = 66;

  for(var i=0; i<dist; i+=ptrdist) {

    var x = -maxx*i/dist+GetX(pTarget)-GetDir(pTarget)*20+10;
    var y = -maxy*i/dist+GetY(pTarget);

    var rand = RandomX(-30,30);
    var xdir = +Sin(angle+rand,20);
    var ydir = -Cos(angle+rand,20);

    CreateParticle("Thrust",x,y,GetXDir(pTarget)/2,GetYDir(pTarget)/2,RandomX(80,100),RGBa(255,200,200,60),0,0);
    CreateParticle("Smoke2",x,y,xdir,ydir,RandomX(100,120),RGBa(220,200,180,0),0,0);
  }
  /* -- halb kopiert aus Missile.c4d */
  
  if(iTime > 20)
    return(-1);
}

public func FxThrustStop(object pTarget, int iEffectNumber) {
  Sound("Flamer", false, pTarget, 100,0,-1);
  if(EffectVar(0,pTarget,iEffectNumber))
    RemoveObject(EffectVar(0,pTarget,iEffectNumber));
    
  // reload
  //Reload(true);
}

public func FxJetpackDelayTimer(object pTarget, int iEffectNumber)
{
  timeleft--;
  if(timeleft <= 0) return(-1);
/*
  if(pTarget->GetAction() ne "JetpackFlight"
  && pTarget->GetAction() ne "BackFlip"
  && pTarget->GetAction() ne "JumpArmed"
  && pTarget->GetAction() ne "Jump")
  {
    return(-1);
  }*/
} 

public func FxJetpackDelayStop(object pTarget, int iEffectNumber) {
}

protected func Update() {
  // nicht angelegt
  if(!pHazard) return();
  /*
  if(reload) {
    timeleft -= 15;
    if(timeleft <= 0) {
      timeleft = 0;
      reload = false;
      // echt nervig
      //var ammoload = GetFMData(FM_AmmoLoad);
      //var ammoid = GetFMData(FM_AmmoID);
      //PlayerMessage(GetOwner(pHazard),"$Reloaded$",pHazard,ammoload,ammoid);
    }
  }*/
}

private func SetJetpackAction(object pCon) {
  if(GetActMapVal("Name","JetpackFlight",GetID(pCon)))
	pCon->SetAction("JetpackFlight");
  // ansonsten so lassen wie is.
}