/*-- Handel --*/

#strict

#appendto MTIP

protected func ContainedUp(object pClonk)
{
  if(Local(3)) {
    // Menü mit den Einträgen füllen
    CreateMenu(GetID(),pClonk,this(),C4MN_Extra_None,"$MsgTrade$",0,C4MN_Style_Context);
    AddMenuItem("{{SBTR:8}} $MsgBuyIndian$","BuyIndian",SBTR,pClonk,0,pClonk,0,2,7);
    AddMenuItem("$MsgCancel$","Cancel",MCMX,pClonk,0,pClonk);
    return(1);
  }
  if(!Local(2)) return(0);
  
  // Menü mit den Einträgen füllen
  CreateMenu(GetID(),pClonk,this(),C4MN_Extra_None,"$MsgTrade$",0,C4MN_Style_Context);
  AddMenuItem("{{SBTR:2}} $MsgOre$","Trade(1)",SBTR,pClonk,0,pClonk,0,2,1);
  AddMenuItem("{{SBTR:4}} $MsgSulfur$","Trade(2)",SBTR,pClonk,0,pClonk,0,2,3);
  AddMenuItem("{{SBTR:6}} $MsgCoal$","Trade(3)",SBTR,pClonk,0,pClonk,0,2,5);
  AddMenuItem("$MsgCancel$","Cancel",MCMX,pClonk,0,pClonk);
  return(1);
}

public func ContainedDig(pClonk) { }
protected func ContextJoinClan(pClonk) { [Condition=Never] }
protected func ContextClanInfo(pClonk) { [Condition=Never] }

protected func ContextTrade(pClonk)
{
  [$MsgTrade$|Image=MTIP|Condition=CanTrade]
  return(ContainedUp(pClonk));
}

public func CanTrade() { return(Local(2)); }

protected func ContextIndian(pClonk)
{
  [$MsgBuyIndian$|Image=INDI|Condition=CanBuyIndian]
  return(ContainedUp(pClonk));
}

public func CanBuyIndian() { return Local(3); }

public func Never() { return(0); }

private func GetFoodAmount()
{
  return(ObjectCount2(Find_Container(this()),Find_Func("GetFeedValue")));
}

private func RemoveFood(iAmount)
{
  var pObj;
  for(pObj in FindObjects(Find_Container(this()),Find_Func("GetFeedValue"))) {
    RemoveObject(pObj);
    if(!--iAmount) break;
  }
}

func Trade(iIndex)
{
  var iFood = GetFoodAmount();
  if(iIndex==1)
  {
    if(iFood<10) return(Message("$MsgTooFewFood$", this()));
    RemoveFood(10);
    CreateContents(ORE1,this(),5);
    Sound("Cash");
    return(Message("$MsgFoodOre$", this()));
  }
  if(iIndex==2)
  {
    if(iFood<8) return(Message("$MsgTooFewFood$", this()));
    RemoveFood(8);
    CreateContents(SPHR,this(),5);
    Sound("Cash");
    return(Message("$MsgFoodSulfur$", this()));
  }
  if(iIndex==3)
  {
    if(iFood<6) return(Message("$MsgTooFewFood$", this()));
    RemoveFood(6);
    CreateContents(COAL,this(),5);
    Sound("Cash");
    return(Message("$MsgFoodCoal$", this()));
  }
}

func BuyIndian(idDummy, pClonk)
{
  if(ContentsCount(WINC)>=2 && ContentsCount(AMBO)>=4)
  {
    for(var i=0;i<2;i++)      
      RemoveObject(FindContents(WINC));
    for(var i=0;i<4;i++)      
      RemoveObject(FindContents(AMBO));
    var pIndian = CreateContents(INDI);
    SetOwner(GetOwner(pClonk), pIndian);
    MakeCrewMember(pIndian, GetOwner(pClonk));
    Sound("Cash");
    SetPlrKnowledge(GetOwner(pClonk), LOAM);
    SetPlrKnowledge(GetOwner(pClonk), TOMA);
    SetPlrKnowledge(GetOwner(pClonk), SPRI);
    SetPlrKnowledge(GetOwner(pClonk), TEND);
    SetPlrKnowledge(GetOwner(pClonk), BOW2);
    SetPlrKnowledge(GetOwner(pClonk), ARPI);
    SetPlrKnowledge(GetOwner(pClonk), QUIV);
    return(Message("$MsgIndian$", this()));
  }
  return(Message("$MsgTooFewWeapons$", this()));
}

public func Cancel() { return(1); }