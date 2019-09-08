/*-- Zeltpack --*/

#strict

public func Activate(pClonk)
{
  [$TxtCreateConstructions$]
  // Clonk anhalten
  SetComDir(COMD_Stop(), pClonk);
  // Menü erzeugen und mit Zelt-Bauplänen des Spielers füllen
  CreateMenu(CXCN, pClonk, this(), 1, "$TxtNoconstructionplansa$");
  var iBuild = [];
  var i, i2, idType;

  // Das wird irgendwann mal bearbeitet...
//  var iClonkTypes = ["Indian","Trapper"];
//  for(i = 0; i < GetLength(iClonkTypes); i++)
//    if(eval(Format("Object(%d)->~Is%s()",ObjectNumber(pClonk),iClonkTypes[i]))) iBuild[GetLength(iBuild)] = iClonkTypes[i];

  while(idType = GetPlrKnowledge(GetOwner(pClonk),0,i++,C4D_Structure())) {
    if(!idType->~IsTent()) continue;
//    if(!GetLength(iBuild)) {
      AddMenuItem("$TxtConstructions$", "CreateConstructionSite", idType, pClonk);   
      continue;
//    }
//    for(i2 = 0; i2 < GetLength(iBuild); i2++) {
//      if(!eval(Format("%i->~Is%sHandcraft()",idType,iBuild[i2]))) continue;
//      AddMenuItem("$TxtConstructions$", "CreateConstructionSite", idType, pClonk);   
//      break;
//    }
  }

  if(FindObject(CNMT))
    AddMenuItem("$TxtFill$", Format("FillPacket(%i,Object(%d),%d)", GetID(this()), ObjectNumber(pClonk), 0), GetID(this()), pClonk);
  return(1);
}

protected func FillPacket(idType, pClonk)
{
  CloseMenu(pClonk);
  CreateMenu(idType, pClonk, this(), 0, "Keine Objekte gefunden");
  AddMenuItem("$TxtBackMenu$",Format("Activate(Object(%d))",ObjectNumber(pClonk)),CXCN,pClonk,0,0,"$DescBackMenu$");
  AddMenuItem(GetName(0,WOOD),Format("FillIn(%i,Object(%d),%d)",WOOD,ObjectNumber(pClonk),0),WOOD,pClonk,ContentsCount(WOOD,this())+1,0,GetDesc(0,WOOD));
  AddMenuItem(GetName(0,LTHR),Format("FillIn(%i,Object(%d),%d)",LTHR,ObjectNumber(pClonk),0),LTHR,pClonk,ContentsCount(LTHR,this())+1,0,GetDesc(0,LTHR));
  AddMenuItem(GetName(0,ROPE),Format("FillIn(%i,Object(%d),%d)",ROPE,ObjectNumber(pClonk),0),ROPE,pClonk,ContentsCount(ROPE,this()),0,GetDesc(0,ROPE));
}

protected func FillIn(idType, pClonk, iPut)
{
  if(iPut==1) {
    var iItem = FindContents(idType, this());
    if(iItem)
      if(!pClonk->~RejectCollect(idType, iItem)) {
      Exit(iItem);
      Enter(pClonk, iItem);
      }
    else PlayerMessage(GetOwner(pClonk), "$TxtCantTake$", this());
    if(!iItem) { if(idType!=ROPE) PlayerMessage(GetOwner(pClonk), "$TxtCantTake$", this());
    else PlayerMessage(GetOwner(pClonk), "$TxtCantFind$", this()); }
  }
  else {
    var iItem = FindContents(idType, pClonk);
    if(iItem)
      if(!RejectCollect(idType)) {
      Exit(iItem);
      Enter(this(), iItem);
      }
    else PlayerMessage(GetOwner(pClonk), "$TxtCantTake$", this());
    if(!iItem) PlayerMessage(GetOwner(pClonk), "$TxtCantFind$", this());
  }
  FillPacket(GetID(this()), pClonk, iPut);
  if(idType==WOOD) return(SelectMenuItem(1, pClonk));
  if(idType!=ROPE) return(SelectMenuItem(2, pClonk));
  else             return(SelectMenuItem(3, pClonk));
}

public func MenuQueryCancel(MenuItem, pClonk)
{
  // Falsches Menü
  if(GetMenu(pClonk) != GetID()) return();

  // ID herausfinden
  if(MenuItem == 1) return(FillIn(WOOD, pClonk, 1));
  if(MenuItem == 2) return(FillIn(LTHR, pClonk, 1));
  if(MenuItem == 3) return(FillIn(ROPE, pClonk, 1));
}

protected func CreateConstructionSite(idType)
{
  // Nur wenn der Clonk steht und sich im Freien befindet
  if(!Contained()) return();
  if(GetAction(Contained()) ne "Walk") return(0);
  if(Contained(Contained())) return(0);
  // Pruefen, ob das Gebaeude hier gebaut werden kann
  if(idType->~RejectConstruction(0,10,Contained())) return(0);
  // Baustelle erschaffen
  var pSite;
  if(!(pSite = CreateConstruction(idType,0,GetObjHeight(Contained())/2,GetOwner(Contained()),1,1,1))) return(0);
  // Nur wenn Baumaterial aktiviert ist lässt sich das Zeltpacket füllen
  var i, iDef, iCount;
  if(FindObject(CNMT)) {
    CreateContents(LTHR);
    CreateContents(WOOD);
    for(i = 0; i < 3; i++) {
      iDef = [WOOD,LTHR,ROPE][i];
      iCount = Min(ContentsCount(iDef), GetComponent(iDef, 0, 0, idType));
      SetComponent(iDef, iCount, pSite);
      for(iCount; iCount > 0; iCount--)
        RemoveObject(FindContents(iDef,this()));
    }
  }
  // Meldung
  Message("$TxtConstructions$", Contained(), GetName(pSite));
  // Bausatz verbrauchen
  RemoveObject(0, 1);
  return(1);
}

protected func Hit()
{
  // Effekt
  Sound("TentPackHit");
  return(1);
}

protected func RejectCollect(idObject) {
  if(idObject==WOOD) if(ContentsCount(WOOD)<4) return(0);
  if(idObject==LTHR) if(ContentsCount(LTHR)<3) return(0);
  if(idObject==ROPE) if(ContentsCount(ROPE)<2) return(0);
  return(1);
}

/* Objekt darf nur einmal im Inventar sein */
public func CarryLimit() { if(FindObject(CNMT)) return(1); }

/* Kann von Indianern erforscht werden */
public func IsIndianResearch() { return(1); }
public func GetResearchBase() { return(ROPE); }

/* Kann vom Indianer in Handarbeit hergestellt werden */
public func IsIndianHandcraft() { return(1); }

///* Kann vom Trapper in Handarbeit hergestellt werden */
//public func IsTrapperHandcraft() { return(1); }

/* Kann in der Waffenschmiede hergestellt werden */
public func IsArmoryProduct() { return(1); }