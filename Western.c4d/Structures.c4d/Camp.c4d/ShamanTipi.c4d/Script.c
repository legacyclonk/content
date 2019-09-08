/*-- Schamanenzelt --*/

#strict 2

// Amulettproduktion wie bei der Werkstatt
#include WRKS

// Bauanimation wie beim Tipi
#include TIPI

/* Produkteigenschaften */
public func ProductType() { return(C4D_Object); }
public func ProductCondition() { return("IsIndianAmulet"); }
public func NeedsEnergy() { return(0); }

private func MenuProduction(object pCaller) {
  var i, idKnowledge;
  // Menü erzeugen und mit Bauplänen des Spielers füllen
  CreateMenu(MLLT,pCaller,this(),1,"$TxtNoAmulets$");
  for(i = 0; idKnowledge = GetPlrKnowledge(GetOwner(pCaller),0,i,ProductType()); i++)
    if(DefinitionCall(idKnowledge,ProductCondition()))     
      AddMenuItem("$TxtProduction$: %s", "SelectProduction", idKnowledge, pCaller, 0, pCaller);
  return(1);
}

private func Smoking() {
  // Bunter Rauch!
  if(Random(6)) CreateParticle("Smoke", -9, 18, 0,0, 45, RGBa(Random(256),Random(256),Random(256),100));
  if(Random(8)) CreateParticle("Smoke", -9, 18, 0,0, 45, RGBa(Random(256),Random(256),Random(256),100));
  return(1);
}

global func GetClanMystic(int iClan)
{
  var pObj, iMystic;
  while(pObj = FindObjectClan(0,iClan,0,0,0,0,0,"Mystic",0,pObj))
    iMystic += pObj->~GetMystic();
  return(Min(iMystic,100));
}

// Der Häuptling der Indianer soll mal ins Spiel kommen.
protected func ContextRecruitChief(object pClonk)
{
  [$TxtRecruitChief$|Image=INDI|Condition=CheckID]
  return(ContainedDig(pClonk,1));
}

public func CheckID(pClonk, iID) { return(GetID(pClonk) == iID && !(pClonk->~IsChief()) && GetCon() >= 100); }

protected func ContextGetMystic(pClonk)
{
  [$TxtMystic$|Image=MYMS]
  pClonk->ContainedCall("TellClanMystic", this());
}

protected func TellClanMystic(pClonk)
{
  return(Message("%d{{MYMS}}", this(), GetClanMystic(GetClan())));
}

// Damit Produktion an erster Stelle steht
public func ContextConstruction(pCaller) {
  [$ContextProduction$|Image=MLLT|Condition=IsBuilt]
  return(MenuProduction(pCaller));
}

protected func MakeChief(pClonk) { return(ContainedDig(pClonk, 1)); }

protected func ContainedDig(object pClonk, int iMenu)
{
  [$TxtMysticChief$|Image=MYMS]
  if(iMenu != 1) TellClanMystic(pClonk);
  if(GetID(pClonk)!=INDI) return(0);
  CreateMenu(INDI,pClonk,this(),2,"",0,0,0);
  var pIcon = CreateObject(TIM1,0,0,NO_OWNER);
  SetGraphics("Chief",pIcon,INDI);
  SetPicture(GetDefCoreVal("Picture","DefCore",INDI,0),
             GetDefCoreVal("Picture","DefCore",INDI,1),
             GetDefCoreVal("Picture","DefCore",INDI,2),
             GetDefCoreVal("Picture","DefCore",INDI,3),pIcon);
  SetColorDw(GetColorDw(pClonk),pIcon);
  AddMenuItem("$TxtRecruitChief$","DoRecruition",INDI,pClonk,0,pClonk,"$DescChief$",4+128,pIcon,35);
  RemoveObject(pIcon);
  AddMenuItem("$TxtCancel$","Cancel",MCMX,pClonk,0,pClonk);
}

public func Cancel() { return(0); }

public func DoRecruition(id idClonk, object pClonk) {
  AddCommand(pClonk,"Call",this(),INDI,0,0,0,"Recruit",0,1);
  AddCommand(pClonk,"Enter",this());
  return(1);
}

public func Recruit(object pClonk, id idClonk)
{
  // Gibt es bereits einen Häuptling?
  if(FindObject2(Find_ID(idClonk),Find_Func("IsChief"),Find_OCF(OCF_Alive),Find_Not(Find_Hostile(GetOwner(pClonk)))))
    return(PlayerMessage(GetOwner(pClonk),"$TxtOnlyOneChief$",pClonk));
  if(GetWealth(GetOwner(pClonk)) < 35)
    return(PlayerMessage(GetOwner(pClonk),"$TxtNotEnoughMoney$",pClonk));
  DoWealth(GetOwner(pClonk),-35);
  pClonk->~MakeChief();
  Message("$TxtChiefRecruited$",this());
  Sound("Uncash",1,0,0,GetOwner(pClonk));
  return(1);
}

/* Kann vom Indianer hergestellt werden */
public func IsIndianHandcraft() { return(1); }

/* Kann von Indianern erforscht werden */
public func IsIndianResearch() { return(1); }

/* Kein Schamanenzelt ohne Tipi */
public func GetResearchBase() { return(TIPI); }