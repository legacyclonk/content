/*-- Kein Wallhack --*/

#strict 2

#appendto WINC
#appendto REVR
#appendto BOW2
#appendto CCAN
// Neue Objekte (Mörser, Gatling)
#appendto MR4V
#appendto GT4V
// Original
#appendto CATA
#appendto CANN
#appendto XBOW
// Wenn vorhanden: Ritter
//#appendto BOW1

protected func Activate(iParameter)
{
  if(GetID() != NW4V) return(_inherited(iParameter));
  MessageWindow(GetDesc(),iParameter);
  return(1);
}

//public func Applications(int iNum) { return([WINC,REVR,BOW2,CCAN,CATA,CANN,XBOW][iNum]); }

public func IsWallhack(object pClonk, id iID)
{
  var i, iDir, iPhase;
  var iX, iY;
  iDir = GetDir(pClonk)*2-1;
  if(iID == WINC) {
    i  = [-11,1,15,28,43,62,76,95,115,135][GetPhase(pClonk)];
    iX =  Sin(i,14)*iDir;
    iY = -Cos(i,14)-2;
    if(PathFree(AbsX(GetX(pClonk)),   AbsY(GetY(pClonk)-4),AbsX(GetX(pClonk)+iX),AbsY(GetY(pClonk)+iY)))
      return(0);
    // Für Türme
    if(PathFree(AbsX(GetX(pClonk)),   AbsY(GetY(pClonk)-4),AbsX(GetX(pClonk)+iX),AbsY(GetY(pClonk)-4)) &&
       PathFree(AbsX(GetX(pClonk))+iX,AbsY(GetY(pClonk)-4),AbsX(GetX(pClonk)+iX),AbsY(GetY(pClonk)+iY)))
      return(0);
    return(1);
  }
  if(iID == REVR) {
    if(PathFree(GetX(pClonk),GetY(pClonk),  GetX(pClonk)+iDir*10,GetY(pClonk)))
      return(0);
    // Für Türme
    if(PathFree(GetX(pClonk),GetY(pClonk)-5,GetX(pClonk)+iDir*10,GetY(pClonk)-5))
      return(0);
    return(1);
  }
  if(iID == BOW2 || iID == BOW1) {
    pClonk->~GetBowOut(this(),pClonk->~GetBowAimAngle(),false,iX,iY,iPhase,iPhase,iPhase,iPhase);
    if(PathFree(GetX(pClonk),   GetY(pClonk)-2,GetX(pClonk)+iX,GetY(pClonk)+iY))
      return(0);
    // Für Türme
//    if(PathFree(GetX(pClonk),   GetY(pClonk)-4,GetX(pClonk)+iX,GetY(pClonk)-4) &&
//       PathFree(GetX(pClonk)+iX,GetY(pClonk)-4,GetX(pClonk)+iX,GetY(pClonk)+iY))
//      return(0);
    if(PathFree(GetX(pClonk),     GetY(pClonk)-4,GetX(pClonk)+iX,GetY(pClonk)-4)) {
      // Um Tiere noch am Boden zu treffen
      if(GetPhase(pClonk) == 7) if(GBackSolid(GetX(pClonk)+iX,GetY(pClonk)+iY))
        return(0);
      if(PathFree(GetX(pClonk)+iX,GetY(pClonk)-4,GetX(pClonk)+iX,GetY(pClonk)+iY))
        return(0);
    }
    return(1);
  }
  if(iID == CCAN) {
    iX = 20*iDir;
    iY = GetPhase()-7;
    if(PathFree(GetX(),   GetY()-4,GetX()+iX,GetY()+iY))
      return(0);
    // Für Türme
    if(PathFree(GetX(),   GetY()-4,GetX()+iX,GetY()-4) &&
       PathFree(GetX()+iX,GetY()-4,GetX()+iX,GetY()+iY))
      return(0);
    return(1);
  }
  if(iID == MR4V) {
    iX = (GetPhase()*2/3+3+GetDir())*iDir;
    iY = GetPhase()-14;
    if(PathFree(GetX(),   GetY(),  GetX()+iX,GetY()+iY))
      return(0);
    return(1);
  }
  if(iID == GT4V) {
    iPhase = LocalN("iRotation",this());
    iX =  Sin(iPhase-5,21)*iDir+GetDir()+6*iDir;
    iY = -Cos(iPhase-5,21)-6;
    if(PathFree(GetX(),   GetY()-6,GetX()+iX,GetY()+iY))
      return(0);
    // Für Türme
    if(PathFree(GetX(),   GetY()-6,GetX()+iX,GetY()-6) &&
       PathFree(GetX()+iX,GetY()-6,GetX()+iX,GetY()+iY))
      return(0);
    return(1);
  }
  if(iID == CATA) {
    if(PathFree(GetX()+8*iDir,GetY()-8,GetX()+12*iDir,GetY()-10))
      return(0);
    return(1);
  }
  if(iID == XBOW) {
    iX = GetPhase()*iDir;
    iY = GetPhase()-15;
    if(PathFree(GetX(),   GetY(),GetX()+iX,GetY()+iY))
      return(0);
    // Für Türme
    if(PathFree(GetX(),   GetY(),GetX()+iX,GetY()) &&
       PathFree(GetX()+iX,GetY(),GetX()+iX,GetY()+iY))
      return(0);
    return(1);
  }
  if(iID == CANN) {
    iX = (GetPhase()+5)*iDir;
    iY = GetPhase()-14;
    if(PathFree(GetX()+2*iDir,GetY(),GetX()+iX,GetY()+iY))
      return(0);
    // Für Türme
    if(PathFree(GetX()+2*iDir,GetY(),GetX()+iX,GetY()) &&
       PathFree(GetX()+iX,    GetY(),GetX()+iX,GetY()+iY))
      return(0);
    return(1);
  }
  return(0);
}

// Winchester
protected func RifleShot(object pClonk, int iAngle)
{
  if(!FindObject(NW4V)) return(_inherited(pClonk,iAngle));
  if(LocalN("iRifleAmmo", pClonk))
    if(!GetEffect("*ReloadRifle*", pClonk))
      if(IsWallhack(pClonk,WINC)) {
        PlayerMessage(GetOwner(pClonk), "$TxtNoShoot$", pClonk);
        return(0);
      }
  return(_inherited(pClonk,iAngle));
}

// Revolver
public func ControlThrow(object pClonk) {
  if(!FindObject(NW4V) || GetID() != REVR) return(_inherited(pClonk));
  if(!GetEffect("ReloadGun", this()))
    if(LocalN("iAmmoCount",this()))
      if(GetAction(pClonk) == "Walk") if(CheckAction("DrawGun",pClonk))
        if(IsWallhack(pClonk, REVR)) {
          PlayerMessage(GetOwner(pClonk), "$TxtNoShoot$", pClonk);
          return(1);
        }
  return(_inherited(pClonk));
}

// Revolver, Bogen, Kanone, Katapult, Armbrust, Haubitze
public func Fire(object pClonk) {
  if(!FindObject(NW4V)) return(_inherited(pClonk));
  if(GetID() == REVR) {
    pClonk = Contained();
    var iDir = GetDir(pClonk)*2-1;
    if(IsWallhack(pClonk, REVR)) {
      var pAmmo = Contents();
      LocalN("iAmmoCount",this())--;
      Sound("Ricochet*");
      RemoveObject(pAmmo);
      CreateParticle("MuzzleFlash",iDir*10,-2,iDir*10,0,35,RGBa(255,255,255,0),Contained());
      Smoke(iDir*10,-2+Random(5),2);
      Smoke(iDir*10,-2+Random(5),3);
      Sound("RevolverShot");
      AddEffect("ReloadGun",this(),101,20);
      return(1);
    }
    return(_inherited());
  }
  if(GetID() == BOW2 || GetID() == BOW1) {
    if(pClonk->~IsAiming())
      if(!Contained(pClonk))
        if(pClonk->~GetArrowCount()||pClonk->~HasQuiverArrows())
          if(IsWallhack(pClonk, GetID())) {
            PlayerMessage(GetOwner(pClonk), "$TxtNoShoot$", pClonk);
            return(0);
          }
    return(_inherited(pClonk));
  }
  if(GetID() == CCAN) {
    if(FindContents(GUNP))
      if(FindOtherContents(GUNP))
        if(!GetEffect("IntReload", this()))
          if(IsWallhack(this(), CCAN)) {
            Message("$TxtNoShoot$", this());
            return(1);
          }
    return(_inherited(pClonk));
  }
  if(GetID() == MR4V) {
    if(FindContents(GUNP))
      if(FindOtherContents(GUNP))
        if(!GetEffect("IntReload", this()))
          if(IsWallhack(this(), MR4V)) {
            Message("$TxtNoShoot$", this());
            return(1);
          }
    return(_inherited(pClonk));
  }
  if(GetID() == CATA || GetID() == XBOW) {
    if(Contents())
      if(IsWallhack(this(), GetID())) {
        Message("$TxtNoShoot$", this());
        return(1);
      }
    return(_inherited(pClonk));
  }
  if(GetID() == CANN) {
    if(FindContents(GUNP))
      if(FindOtherContents(GUNP))
        if(IsWallhack(this(), CANN)) {
          Message("$TxtNoShoot$", this());
          return(1);
        }
    return(_inherited(pClonk));
  }
  return(_inherited(pClonk));
}

// Gatling
protected func FxIntIsShootingStart(object pTarget, int iEffectNumber, int iTemp)
{
  if(iTemp) return(0);
  if(pTarget->~IsWallhack(pTarget, GT4V)) {
    Message("$TxtNoShoot$", this());
    return(-1);
  }
  Sound("GatlingShot", 0, pTarget, 100, 0, 1);
// Um Schnellfeuer zu vermeiden
//  pTarget->~Shoot();
  return(1);
}

protected func FxIntIsShootingTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  if(!FindObject2(Find_Action("Push"),Find_ActionTarget(pTarget))) return(-1);
  if(pTarget->~IsWallhack(pTarget, GT4V)) {
    Message("$TxtNoShoot$", this());
    return(-1);
  }
  pTarget->~Shoot();
  return(1);
}

// Bogen
public func FireAngle(object pClonk, int iAngle) {
  if(!FindObject(NW4V)) return(_inherited(pClonk,iAngle));
  if(pClonk->~IsAiming())
    if(!Contained(pClonk))
      if(pClonk->~GetArrowCount())
        if(IsWallhack(pClonk, BOW2)) {
          PlayerMessage(GetOwner(pClonk), "$TxtNoShoot$", pClonk);
          return(0);
        }
  return(_inherited(pClonk,iAngle));
}