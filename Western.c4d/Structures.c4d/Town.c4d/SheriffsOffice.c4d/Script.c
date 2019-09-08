/*-- Büro des Sheriff --*/

#strict
#include DOOR
#include BAS9

local arms, prison;

protected func Initialize()
{
  // Bewaffnung vorhanden
  arms = true;
  // Gefängnis erstellen
  prison = CreateObject(PRSN, 0,0, GetOwner());
  prison->Office(this());
  return(_inherited());
}

/* Gefangenenliste anzeigen */

protected func ContainedUp(object pClonk)
{
  [$TxtShowPrisoners$|Image=HDCF]
  var szCaption = "$MsgPrisoners$";
  if(!FindObject(0, 0, 0, 0, 0, OCF_CrewMember, 0, 0, prison)) szCaption = "$MsgNoPrisoners$";
	  
  CreateMenu (HDCF, pClonk, this(), 0, szCaption, 0, 1);
  var obj;
  Local()=0;
  while(obj = FindObject(0, 0, 0, 0, 0, OCF_CrewMember, 0, 0, prison, obj))
  {
    AddMenuItem (Format("%s (%s)", GetName(obj), GetPlayerName(GetOwner(obj))), "", GetID(obj), pClonk, GetRank(obj), 0, 0, 1);
    Local()++;
  }
  SelectMenuItem(Local(), pClonk);
  return();
}

protected func ContextShowPrisoners(object pClonk)
{
  [$TxtShowPrisoners$|Image=HDCF]
  pClonk->ContainedCall("ContainedUp", this());
}

/* Sheriff ausbilden und Handschellen bauen */
protected func ContainedDig(object pClonk)
{
  [$TxtRecruitSheriff$|Image=SHRF]
  if(GetID(pClonk)!=COWB && GetID(pClonk)!=SHRF) return(0);
//  if(pClonk->~IsBandit()) return(0);
  CreateMenu(SHRF,pClonk,this(),2,"",0,0,0);
  if(!pClonk->~IsSheriff())
    AddMenuItem("$TxtRecruitSheriff$","DoRecruition",SHRF,pClonk,0,pClonk,0,128,0,35);
  AddMenuItem("$TxtConstructHandcuffs$","DoHandcuffs",HDCF,pClonk,0,pClonk);
  AddMenuItem("$TxtCancel$","Cancel",MCMX,pClonk,0,pClonk);
}

protected func ContextRecruitSheriff(object pClonk)
{
  [$TxtRecruitSheriff$|Image=SHRF]
  return(ContainedDig(pClonk,1));
}

public func DoRecruition(id idClonk, object pClonk) {
  AddCommand(pClonk,"Call",this(),SHRF,0,0,0,"Recruit",0,1);
  AddCommand(pClonk,"Enter",this());
  return(1);
}

public func DoHandcuffs(id idObj, object pClonk) {
  AddCommand(pClonk,"Call",this(),HDCF,0,0,0,"ConstructHandcuffs",0,1);
  AddCommand(pClonk,"Enter",this());
  return(1);
}

public func Cancel() { }

public func ConstructHandcuffs(object pClonk, id idType)
{
  var pMetal = FindContents(METL);
  if(!pMetal) pMetal = FindContents(METL, pClonk); 
  if(!pMetal) {
    PlayerMessage(GetOwner(pClonk),"$MsgNoMetal$",pClonk);
    return(false);
  }
  RemoveObject(pMetal);
  CreateContents(HDCF);
  Sound("Build1");
  Sound("Build2");
  return(1);
}

public func Recruit(object pClonk, id idClonk)
{
  // Weil ich für dieses Gebäude eine Extra-Regel erstellt habe
  if(FindObject(NR4V)) return(PlayerMessage(GetOwner(pClonk),"$NoSheriff$",pClonk));
  if(FindObject2(Find_ID(SHRF),Find_OCF(OCF_Alive),Find_Not(Find_Hostile(GetOwner(pClonk)))))
    return(PlayerMessage(GetOwner(pClonk),"$TxtOnlyOneSheriff$",pClonk));
  if(GetWealth(GetOwner(pClonk)) < 35)
    return(PlayerMessage(GetOwner(pClonk),"$TxtNotEnoughMoney$",pClonk));
  DoWealth(GetOwner(pClonk),-35);
  ChangeDef(SHRF,pClonk);
  ObjectSetAction(pClonk,"Walk");
  pClonk->~AdjustPortrait();
  SetPhysical("Energy",GetPhysical("Energy",0,pClonk)+25000,2,pClonk);
  DoEnergy(100,pClonk);
  AddEffect("CheckImprison",pClonk,1,10,pClonk);
  Message("$TxtSheriffRecruited$",this());
  Sound("Uncash",1,0,0,GetOwner(pClonk));
  return(1);
}

private func ErrorMessage(string Msg, object pClonk)
{
  // Es ist irgendwas schief gelaufen
  PlayerMessage(GetOwner(pClonk), Msg, pClonk);
  Sound("Error", 1, 0,0, GetOwner(pClonk));
}

/* Gefängnis */

public func Imprison(object pBandit, object pSheriff)
{
  // Kein Gefängnis (O_o), erstellen
  if(!prison)
    {
    prison = CreateObject(PRSN, 0,0, GetOwner());
    prison->Office(this());
    }
  // Bandit wird ins Gefängnis versetzt
  prison->Imprison(pBandit);
  // Bank vorhanden
  if(!GetBank(0, 1)) return();
  // Der Sheriff bekommt eine Belohnung
  DoWealth(GetOwner(pSheriff), 40);
  Sound("Cash", 1, 0,0, GetOwner(pSheriff));
}
