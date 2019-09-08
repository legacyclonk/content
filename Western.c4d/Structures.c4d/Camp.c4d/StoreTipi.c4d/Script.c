/*-- Lager --*/

#strict 2

// Bauanimation wie beim Tipi
#include TIPI

local iFood;

public func GetFoodAmount()
{
  var iCount, pObj, aList;
  iCount = iFood;
  aList = FindObjects(Find_Container(this()),Find_Func("GetFeedValue"));
  for(pObj in aList)
    iCount += pObj->~GetFeedValue();
  return(iCount);
}

public func DoFoodAmount(int iAmount)
{
  var i, iCount, pObj, aList;
  if(iFood >= iAmount) {
    iFood -= iAmount;
    return(1);
  }
  iAmount -= iFood;
  iFood = 0;
  aList = FindObjects(Sort_Reverse(Sort_Func("GetFeedValue")),Find_Func("GetFeedValue"),Find_Container(this()));
  for(pObj in aList) {
    iAmount -= pObj->~GetFeedValue();
    RemoveObject(pObj);
    if(iAmount > 0) continue;
    iFood -= iAmount;
    return(1);
  }
  return(0);
}

protected func ContainedUp(object pClonk)
{
  [$TxtMenu$|Image=RMET]
  return(EatMenu(pClonk));
}

protected func ContextMenu(object pClonk)
{
  [$TxtMenu$|Image=RMET]
  pClonk->ContainedCall("ContainedUp",this());
}

public func EatMenu(object pClonk)
{
  var iAmount;
  CreateMenu(RMET,pClonk,this(),3,"",GetFoodAmount());
//  iAmount = (GetPhysical("Energy",0,pClonk)+999-GetEnergy2(pClonk))/1000;
  iAmount = GetPhysical("Energy",0,pClonk)/1000-GetEnergy(pClonk);
  AddMenuItem("$TxtHeal$","Eat",MGHL,pClonk,0,pClonk,"$DescHeal$",128,0,iAmount);
  if(FindObject(NDFO)) {
    iAmount = (LocalN("iHunger",pClonk)-1)*5/3/(200-GetCon(pClonk))+1;
    if(LocalN("iHunger",pClonk) <= 33) iAmount = 0;
    AddMenuItem("$TxtEat$","Eat",WNHN,pClonk,0,pClonk,"$DescEat$",128,0,iAmount);
  }
}

protected func Eat(id idItem, object pClonk)
{
  var iAmount, iFoodAmount;
  iFoodAmount = GetFoodAmount();
  if(!iFoodAmount) {
    EatMenu(pClonk);
    SelectMenuItem(idItem == WNHN,pClonk);
    return(1);
  }
  if(idItem == MGHL) iAmount = GetPhysical("Energy",0,pClonk)/1000-GetEnergy(pClonk);
  if(idItem == WNHN)
    if(LocalN("iHunger",pClonk) > 33)
      iAmount = (LocalN("iHunger",pClonk)-1)*5/3/(200-GetCon(pClonk))+1;

  if(iFoodAmount < iAmount) iAmount = iFoodAmount;
  if(iAmount) {
    DoFoodAmount(iAmount);
    pClonk->~Feed(iAmount);
  }
  EatMenu(pClonk);
  SelectMenuItem(idItem == WNHN,pClonk);
  return(1);
}

/* Kann vom Indianer hergestellt werden */
public func IsIndianHandcraft() { return(1); }

/* Kann von Indianern erforscht werden */
public func IsIndianResearch() { return(1); }

/* Dafür wird richtiges Essen benötigt */
public func GetResearchBase() { return(CLD2); }