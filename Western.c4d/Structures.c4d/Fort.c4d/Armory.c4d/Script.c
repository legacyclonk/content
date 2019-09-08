/*-- Waffenlager --*/

#strict 2

#include WRKS
#include BAS9

private func Construction()
{
  _inherited();
  if(basement) SetPosition(GetX(basement)+1,GetY(basement),basement);
}

private func Initialize()
{
  SetShape(-50,-16,101,42);
  SetObjDrawTransform(1000,0,0,0,1000,-1000*5,this());
  return(1);
}

/* Produkteigenschaften */

public func ProductType() { return(C4D_Object); }
public func ProductCondition() { return("IsArmoryProduct"); }
public func NeedsEnergy() { return(0); }

/* Produktion */

private func MenuProduction(pCaller) {
  var i, idKnowledge;
  // Menü erzeugen und mit Bauplänen des Spielers füllen
  CreateMenu(CXCN,pCaller,this(),1,"$TxtNoPlans$");
  for(i = 0; idKnowledge = GetPlrKnowledge(GetOwner(pCaller),0,i,ProductType()); i++)
    if(DefinitionCall(idKnowledge, ProductCondition()))
      AddMenuItem("$TxtProduction$: %s", "SelectProduction", idKnowledge, pCaller, 0, pCaller);
  return(1);
}

/* Aktivität */

private func Smoking()
{
  if (Random(2)) Smoke(-29, -25, 8);
  Smoke(-29, -27, 5 + Random(3));
}

/* Captain ausbilden */

protected func ContainedDig(object pClonk)
{
  [$TxtRecruitCaptain$|Image=CVRM]
  if(GetID(pClonk)!=CVRM && GetID(pClonk)!=BNDT) return(0);
  CreateMenu(GetID(pClonk),pClonk,this(),2,"",0,0,0);
  var pIcon = CreateObject(TIM1,0,0,NO_OWNER);
  var idClonk = GetID(pClonk);
  if(idClonk == CVRM) SetGraphics("Captain",pIcon,CVRM);
  if(idClonk == BNDT) SetGraphics("Boss",pIcon,BNDT);
  SetPicture(GetDefCoreVal("Picture","DefCore",idClonk,0),
             GetDefCoreVal("Picture","DefCore",idClonk,1),
             GetDefCoreVal("Picture","DefCore",idClonk,2),
             GetDefCoreVal("Picture","DefCore",idClonk,3),pIcon);
  SetColorDw(GetColorDw(pClonk), pIcon);
  AddMenuItem(Format("$TxtRecruit$",["$Captain$","$Boss$"][idClonk==BNDT]),"DoRecruition",idClonk,pClonk,0,pClonk,
	      ["$DescCaptain$","$DescBoss$"][idClonk==BNDT],4+128,pIcon,35);
  RemoveObject(pIcon);
  AddMenuItem("$TxtCancel$","Cancel",MCMX,pClonk,0,pClonk);
}

public func Cancel() { return(0); }

public func DoRecruition(id idClonk, object pClonk) {
  AddCommand(pClonk,"Call",this(),idClonk,0,0,0,"Recruit",0,1);
  AddCommand(pClonk,"Enter",this());
  return(1);
}
  
public func Recruit(object pClonk, id idClonk)
{
  // Gibt es bereits einen Ausgebildeten?
  if(FindObject2(Find_ID(idClonk),Find_Func("IsCaptain"),Find_OCF(OCF_Alive),Find_Not(Find_Hostile(GetOwner(pClonk)))))
    return(PlayerMessage(GetOwner(pClonk),"$TxtOnlyOne$",pClonk,["$Captain$","$Boss$"][idClonk==BNDT]));
  if(GetWealth(GetOwner(pClonk)) < 35) {
//    Sound("CommandFailure1",0,pClonk,0,GetOwner(pClonk));
    return(PlayerMessage(GetOwner(pClonk),"$TxtNotEnoughMoney$",pClonk));
  }
  DoWealth(GetOwner(pClonk),-35);
  if(idClonk == CVRM) {
    pClonk->~MakeCaptain();
    // Mitspieler sollten erfahren, dass ein Captain ausgebildet worden ist
    //PlayerMessage(GetOwner(pClonk),"$TxtRecruited$",pClonk,"Captain");
    Message("$TxtRecruited$",this(),"Captain");
  }
  if(idClonk == BNDT) {
    pClonk->~MakeBoss();
    Message("$TxtRecruited$",this(),"Boss");
  }
  Sound("Uncash",1,0,0,GetOwner(pClonk));
  return(1);
}

protected func ContextRecruitCaptain(pClonk)
{
  [$TxtRecruitCaptain$|Image=CVRM|Condition=CheckID]
  return(ContainedDig(pClonk));
}

protected func ContextRecruitBandit(pClonk)
{
  [$TxtRecruitBoss$|Image=BNDT|Condition=CheckID]
  return(ContainedDig(pClonk));
}

public func CheckID(pClonk, iID) { return(GetID(pClonk) == iID && !(pClonk->~IsCaptain()) && GetCon() >= 100); }

// Damit Produktion an erster Stelle steht
public func ContextConstruction(object pCaller)
{
  [$ContextProduction$|Image=CXCN|Condition=IsBuilt]
  return(MenuProduction(pCaller));
}