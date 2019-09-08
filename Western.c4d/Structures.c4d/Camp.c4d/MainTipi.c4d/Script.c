/*-- Häuptlingszelt --*/

#strict

// Bauanimation wie beim Tipi
#include TIPI

protected func ContextClanInfo(pClonk)
{
  [$CtrlClanInfo$|Image=MTIP]
  pClonk->ContainedCall("ContainedUp", this());
}

protected func ContainedUp()
{
  [$CtrlClanInfo$]
  if(GetOwner() == NO_OWNER) return();
  var count, obj;
  while(obj = FindObjectClan(TIPI, GetClan(), 0, 0, 0, 0, 0, 0, 0, obj))
   if(GetCon(obj) == 100)
    if(GetAction(obj) ne "Construction")
      count+=2;
  var szMsg="", iClan = GetClan(), i, iPlr;
  if(iClan>=0)
    while(iPlr = EffectVar(++i, 0, iClan)) szMsg = Format("%s|%s", szMsg, GetPlayerName(iPlr-1));
  Message("$TxtClan$", this(), GetPlayerName(GetOwner()), GetClanCount(), count, szMsg);
}

protected func ContextJoinClan(pClonk)
{
  [$CtrlJoinClan$|Image=MTIP]
  pClonk->ContainedCall("ContainedDig", this());
}

// Einem Clan beitreten
public func ContainedDig(pClonk)
{
  [$CtrlJoinClan$]
  if(GetOwner() == NO_OWNER) return();
  if(!Hostile(GetOwner(pClonk), GetOwner()))
    if(GetClan(pClonk)!=GetClan())
    {
      JoinClan(GetClan(), GetOwner(pClonk));
      Message("$TxtJoinClan$", this(), GetPlayerName (GetOwner(pClonk)), GetPlayerName (GetOwner()));
    }
}

private func JoinClan(iClan, iPlr)
{
  // Einem bestehenden Clan beitreten    
  if(iClan>=0)
  {
    var i;  
    while(EffectVar(i++, 0, iClan)) ;
    EffectVar(i-1, 0, iClan) = iPlr+1;
//    Log("%d %d %d", i, iClan, iPlr);
    return();
  }
  // Einen neuen Clan aufmachen
  var ef = AddEffect("Clan", 0, 1, 10, this());
  EffectVar(0, 0, ef) = -iClan;
  EffectVar(1, 0, ef) = iPlr+1;
}

func FxClanTimer(pObj, iNum)
{
  var plr_num, i=0, j, iPlr = EffectVar(0, 0, iNum);
  // Verfeindete Spieler aus dem Bund löschen
  while(plr_num = EffectVar(++i, 0, iNum))
  {
    if(Hostile(iPlr-1, plr_num-1) || !GetPlayerName(plr_num-1)) 
    {//Log("%s %s", GetPlayerName(iPlr-1), GetPlayerName(plr_num-1));
      EffectVar(i, 0, iNum) = 0;
      j = i;
      while(EffectVar(++j, 0, iNum)) EffectVar(j-1, 0, iNum) = EffectVar(j, 0, iNum);
      i--;
    }
  }
  // Gemeinschafts Baupläne 
  i=0;
  while(plr_num = EffectVar(i++, 0, iNum))
    HandlePlayer(plr_num, iNum);
}

private func GetKnowledgeCategory(index) {
  if (!index--) return(C4D_Structure());
  if (!index--) return(C4D_Vehicle());
  if (!index--) return(C4D_Object());
}

private func HandlePlayer(plr_num, iNum) {
  var cat, cat_num = 0;
  while (cat = GetKnowledgeCategory(cat_num++)) {
    var knw_id, knw_num = 0;
    while (knw_id = GetPlrKnowledge(plr_num-1, 0, knw_num++, cat)) {
      TransferToAllies(knw_id, iNum);
    }
  }
}

private func TransferToAllies(knw_id, iNum) {
  var plr_num, i;
  while(plr_num = EffectVar(i++, 0, iNum))
  // Hat den Bauplan noch nicht
   if (!GetPlrKnowledge(plr_num-1, knw_id))
    SetPlrKnowledge(plr_num-1, knw_id);
}

global func FindMainTent(iClan, iIndex) {
  var obj = 0;
  while(obj = FindObjectClan(MTIP,iClan,0,0,-1,-1,OCF_Fullcon,0,0,obj))
    if(GetAction(obj) ne "Construction") return(obj);
  return();
}

global func GetClan(pObj)
{
  var iPlr = GetOwner(pObj);
  if(iPlr<0) return();
  var i, ef, j, iEfVar;
  while(ef = GetEffect("Clan", 0, i++))
  {
    j = 0;
    while(iEfVar = EffectVar(j++, 0, ef)) if(iEfVar == iPlr+1) return(ef);
  }
  return(-iPlr-1);
}

// Gibt einem ganzen Clan einen Bauplan (oder nimmt ihm den Bauplan weg)
global func SetClanKnowledge(iClan, id idDef, bool fRemove)
{
  if(iClan<0) return(SetPlrKnowledge(-iClan-1, idDef, fRemove));    
  var i = 0, iEfVar;      
  while(iEfVar = EffectVar(i++, 0, iClan)) SetPlrKnowledge(iEfVar-1, idDef, fRemove);
}

public func AllowBirth()
{
  var count, obj;
  while(obj = FindObjectClan(TIPI, GetClan(), 0, 0, 0, 0, 0, 0, 0, obj)) 
   if(GetCon(obj) == 100)
    if(GetAction(obj) ne "Construction")
      count+=2;
  if(count <= GetClanCount()) return();
  obj = FindObjectClan(STIP, GetClan());
  if(!obj) return();
  if(obj->~GetFoodAmount()>=100)
  return(count > GetClanCount());
}

global func FindObjectClan(id idDef, int iClan, int iX, int iY, int iWdt, int iHgt, int dwOCF, string szAction, object pActionTarget, object pFindNext)
{
  // Kein Clan
  if(!iClan) return();
  while(pFindNext = FindObject(idDef, iX, iY, iWdt, iHgt, dwOCF, szAction, pActionTarget, 0, pFindNext))
  {
    if(GetClan(pFindNext)==iClan) return(pFindNext);
  }
}

public func GetClanCount()
{
  var count, obj;
  while(obj = FindObject(0, 0, 0, 0, 0, OCF_CrewMember(), 0, 0, 0, obj)) 
    if(obj->~IsIndian())
      if(GetClan()==GetClan(obj))       
        count++;     
  return(count);
}

public func GetResearchBase() { return(TIPI); }

/* Kann vom Indianer hergestellt werden */
public func IsIndianHandcraft() { return(1); }

/* Kann von Indianern erforscht werden */
public func IsIndianResearch() { return(1); }
