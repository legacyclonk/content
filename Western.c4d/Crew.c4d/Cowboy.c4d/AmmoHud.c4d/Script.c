/*-- Ammo HUD --*/

#strict 2

local fSecond;
local pOld;

protected func Initialize()
{
  // Damit das HUD bei Atem-/Zauberenergieanzeige komplett zu sehen ist
  SetPosition(56,86);
//  SetPosition(38,86);
  Local(0) = Local(1) = 0;
  SetCategory(C4D_StaticBack|C4D_Parallax|C4D_Foreground|C4D_MouseIgnore|C4D_IgnoreFoW);
  SetVisibility(VIS_Owner);
  SetAction("AmmoHud");
  if(!(fSecond=HudCount()))
    CreateObject(GetID(),0,0,GetOwner());
  return(true);
}

protected func HudCount() { return(ObjectCount(GetID(),0,0,0,0,0,0,0,0,GetOwner())); }

protected func Timer()
{
  if(GetOwner() == -1) return(RemoveObject());
  var pClonk = GetCursor(GetOwner()),
      pObj = Contents(0,pClonk),
      pPush, iID;
  if(!pClonk) return(false);
  if(GetAction(pClonk) == "Push") pPush = GetActionTarget(0,pClonk);
  if(fSecond) {
    CustomMessage(pClonk->~GetHungerString(),this(),GetOwner(),100,0);
    iID = GetID(pPush);
    if(iID == CCAN || iID == MR4V) {
      SetDir(2);
      SetPhase(Min(ContentsCount(CABL,pPush)+1,6));
      return(true);
    }
    SetPhase(0);
    return(true);
  }
  if(pPush) {
    if(CheckPush(pPush,pClonk)) {
      if(pOld != pPush) {
        Message("", this());
        pOld = 0;
      }
      return(true);
    }
  }
  else if(LocalN("idWeapon",pClonk) == WINC) {
    if(pOld) {
      Message("", this());
      pOld = 0;
    }
    SetDir(0);
    SetPhase(LocalN("iRifleAmmo",pClonk)+1);
    return(true);
  }
  if(pObj) {
    if(pOld) {
      if(pOld == pObj) return(true);
      Message("", this());
      pOld = 0;
    }
    if(CheckObject(pObj,pClonk))
      return(true);
  }
  SetDir(0);
  SetPhase(0);
  Message("", this());
  pOld = 0;
  return(true);
}

protected func CheckPush(object pPush, object pClonk)
{
  var iID = GetID(pPush);
  if(iID == CCAN || iID == MR4V) {
    SetDir(3);
    SetPhase(Min(ContentsCount(GUNP,pPush)+1,6));
    return(true);
  }
  if(iID == GT4V) {
    SetDir(4);
    SetPhase(4);
    if(pPush->~IsReloading())
      Message("<c ff0000>%3d   ",this(),pPush->~GetAmmoCount());
    else
      Message("<c ffffff>%3d   ",this(),pPush->~GetAmmoCount());
    pOld = pPush;
    return(true);
  }
  return(false);
}

protected func CheckObject(object pObj, object pClonk)
{
  var iID = GetID(pObj);
  if(iID == REVR) {
    SetDir(0);
    SetPhase(ContentsCount(0,pObj)+1);
    return(true);
  }
  if(iID == WINC) {
    SetDir(0);
    SetPhase(ContentsCount(0,pObj)+1);
    return(true);
  }
  if(iID == DYNB) {
    SetDir(1);
    SetPhase(pObj->~PackCount()+1);
    return(true);
  }
  if(iID == MBAG) {
    SetDir(4);
    SetPhase(1);
    Message("@<c ffff00>%3d   ",this(),LocalN("m_val",pObj));
    pOld = pObj;
    return(true);
  }
  if(pObj->~ManaProcessTime()) {
    if(FindObject(0,0,0,0,0,0,"WearAmulet",pClonk) || pClonk->~IsChief()) {
      SetDir(4);
      SetPhase(3);
      if(pClonk->~IsChief())
        Message("@<c 00ffff>%3d   ",this(),(GetClanMystic(GetClan())+200)*(pObj->~ManaProcessTime())*GetCon(pObj)/120000);
      else
        Message("@<c 00ffff>%3d   ",this(),(GetClanMystic(GetClan())+200)*(pObj->~ManaProcessTime())*GetCon(pObj)/160000);
      pOld = pObj;
      return(true);
    }
  }
  if(iID->~IsValuable()) {
    SetDir(4);
    SetPhase(2);
    Message("@<c ffff00>%3d   ",this(),GetValue(pObj));
    pOld = pObj;
    return(true);
  }
  return(false);
}