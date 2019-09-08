/*-- Drugstore --*/

#strict

#include BAS9
#include DOOR

// Standardmäßig kaufen alle Spieler aus dem Heimatbasismaterial von Spieler 1.
// Soll jedoch jeder Spieler aus seiner eigenen Materialliste kaufen, muss
// die Spielregel "Individuelle Angebote" (DRSI) aktiviert sein.
public func IndividualOffers() { return(ObjectCount(DRSI)); }
// Sollen die Spieler aus einer anderen Liste als der von Spieler 1 kaufen,
// kann dies mit folgender Funktion im Szenarienscript geregelt werden:
//   public func Drugstore_HomebaseMaterialPlayer() { return([Spielernummer]); }
public func HomebaseMaterialPlayer() 
{
  var iPlr = GameCall("Drugstore_HomebaseMaterialPlayer");
  if(!GetPlayerName(iPlr))
   return GetPlayerByIndex(0, C4PT_User);
  return iPlr;
}

/* Kaufmenü */
protected func OpenBuyMenu(object pClonk)
{  
  var i, iID, iPlr;
  // Clonk soll erst in den Laden laufen
  if(Contained(pClonk) != this()) {
    SetCommand(pClonk,"None");
    AddCommand(pClonk,"Call",this(),pClonk,0,0,0,"OpenBuyMenu");
    AddCommand(pClonk,"Enter",this());
    return(1);
  }
  // Mit Banditen wird nicht gehandelt
  if(pClonk->~IsBandit()) return(NoBandits());
  // Etwaige Menüs schließen
  if(GetMenu()) CloseMenu();
  // Menü erzeugen
  CreateMenu(DRST,pClonk,0,C4MN_Extra_Value,"$TxtNoBuyMaterial$",0,0,1);
  // Individuelle Angebote nutzen?
  if(IndividualOffers()) iPlr = GetOwner(pClonk);
  else iPlr = HomebaseMaterialPlayer();
  // Items hinzufügen
  for(i = 0; iID = GetHomebaseMaterial(iPlr,0,i,C4D_All); i++)
    AddMenuItem("$TxtBuy$","BuyItem",iID,pClonk,GetHomebaseMaterial(iPlr,iID),pClonk,GetDesc(0,iID),128,0,GetValue(0,iID,this(),iPlr));
  return(1);
}

private func BuyItem(id iID, object pClonk, bool fSpecial)
{
  var iPlayer, iBuyPlayer, iCount, iValue, pObj, iSelection;
  // Bank vorhanden
  if(!GetBank(this())) return(0);

  iPlayer = GetOwner(pClonk);
  if(IndividualOffers()) iBuyPlayer = iPlayer;
  else iBuyPlayer = HomebaseMaterialPlayer();

  // Ist von diesem Objekt noch genug vorhanden?
  iCount = GetHomebaseMaterial(iBuyPlayer,iID);
  if(!iCount) {
    // Objekt ist nicht verfügbar!
    Sound("CommandFailure1",0,pClonk);
    return(AddMessage("$TxtNotAvailable$",pClonk,GetName(0,iID)));
//    Sound("CommandFailure1",0,pClonk,0,iPlayer+1);
//    return(PlayerMessage(iPlayer,"$TxtNotAvailable$",pClonk,GetName(0,iID)));
  }

  // Hat der Spieler genug Geld?
  iValue = GetValue(0,iID,this(),iPlayer);
  if(GetWealth(iPlayer) < iValue) {
    // Nicht genug Clunker!
    Sound("CommandFailure1",0,pClonk);
    return(AddMessage("$TxtNoMoney$",pClonk));
//    Sound("CommandFailure1",0,pClonk,0,iPlayer+1);
//    return(PlayerMessage(iPlayer,"$TxtNoMoney$",pClonk));
  }

  // Nun zum Kaufen
  while(iCount--) {
    // Noch genug Geld vorhanden?
    if(GetWealth(iPlayer) < iValue) {
      Sound("CommandFailure1",0,pClonk);
      AddMessage(Format("%s|%s","$TxtNoMoney$","$TxtNotBuyable$"),pClonk);
//      Sound("CommandFailure1",0,pClonk,0,iPlayer+1);
//      PlayerMessage(iPlayer,Format("%s|%s","$TxtNoMoney$","$TxtNotBuyable$"),pClonk);
      break;
    }
    // Geld abziehen
    SetWealth(iPlayer,GetWealth(iPlayer)-iValue);
    // Objekt aus der Kaufliste abziehen
    DoHomebaseMaterial(iBuyPlayer,iID,-1);
    // Objekt erschaffen
    pObj = CreateContents(iID,this());

    // Besitzer wechseln
    SetOwner(iPlayer,pObj);
    // Gekauftes Objekt ist ein Mannschaftsmitglied
    if(GetOCF(pObj) & OCF_CrewMember) {
      // CrewMember setzen
      MakeCrewMember(pObj,iPlayer);
      // Startet hier mit voller Lebensenergie
      DoEnergy(GetPhysical("Energy",0,pObj)/1000, pObj);
    }
    // Ansonsten ins Inventar, wenn möglich
    else if(pClonk && GetDefCollectible(iID) && !GetDefCoreVal("NoGet","DefCore",iID))
      Collect(pObj, pClonk);

    // Soll noch mehr gekauft werden?
    if(!fSpecial) break;
  }
  // Kaufsound abspielen
  if(iValue > 0) Sound("Uncash",1,0,0,iPlayer+1);
  if(iValue < 0) Sound("Cash",1,0,0,iPlayer+1);
  // Menü neu erzeugen
  iSelection = GetMenuSelection(pClonk);
  CloseMenu(pClonk);
  ContainedUp(pClonk);
  SelectMenuItem(iSelection, pClonk);
  // Fertig
  return(1);
}

/* Verkaufen */
protected func OpenSellMenu(object pClonk)
{
  // Clonk soll erst in den Laden laufen
  if(Contained(pClonk) != this()) {
    SetCommand(pClonk,"None");
    AddCommand(pClonk,"Call",this(),pClonk,0,0,0,"OpenSellMenu");
    AddCommand(pClonk,"Enter",this());
    return(1);
  }  
  // Mit Banditen wird nicht gehandelt
  if(pClonk->~IsBandit()) return(NoBandits());
  // Etwaige Menüs schließen
  if(GetMenu()) CloseMenu();
  // Menü erzeugen
  CreateMenu(DRST,pClonk,0,C4MN_Extra_Value,Format("$TxtNoSellMaterial$",GetName()),0,0,1);
  // Items aus Drugstore hinzufügen
  var i, iPlr, pObj, iID, iCount;
  for(iPlr = GetOwner(pClonk); pObj = Contents(i,this()); i++) {
    // Kam der Gegenstand schon dran?
    if(GetID(pObj) == iID) continue;
    // ID speichern
    iID = GetID(pObj);
    // Clonks sind nicht zu verkaufen
    if(GetDefCrewMember(iID)) continue;
    // Lässt sich dies überhaupt verkaufen?
    if(GetDefNoSell(iID)) continue;
    iCount = 1;
    while(GetID(Contents(i+iCount,this())) == iID) iCount++;
//    iCount = ContentsCount(iID,this());
    // Ins Verkaufsmenü
    AddMenuItem("$TxtSell$","SellItem",iID,pClonk,iCount,ObjectNumber(pClonk),GetDesc(pObj),132,pObj,GetValue(pObj,0,0,iPlr));
    // Nächste X Objekte überspringen
    i += iCount-1;
/*
    var j, pObjPic, aList, szName, iColor;
    // Haben alle Objekte dieser ID hier dieselben Namen, Besitzerfarbe und Preis?
    iCount = ContentsCount(iID,this());
//    if(ObjectCount2(Find_ID(iID),Find_ColorDw(GetColorDw(pObj)),Find_FuncEqual("GetValue()",GetValue(pObj)),Find_FuncEqual("GetName()",GetName(pObj))) == iCount)
    if(ObjectCount2(Find_Container(this()),Find_ID(iID),Find_ColorDw(GetColorDw(pObj)),Find_FuncEqual("GetName()",GetName(pObj))) == iCount)
      // Gleich ins Verkaufsmenü
      AddMenuItem("$TxtSell$","SellItem",iID,pClonk,iCount,ObjectNumber(pClonk),GetDesc(pObj),132,pObj,GetValue(pObj,0,0,iPlr));
    // Ansonsten nochmal durchchecken
    else {
      aList = FindObjects(Find_Container(this()),Find_ID(iID));
      iCount = 0;
      j = 1;
      while(j) {
        szName = GetName(aList[0]);
        iColor = GetColorDw(aList[0]);
        pObjPic = aList[0];
        for(pObj in aList) {
          if(GetName(pObj) == szName)
            if(GetColorDw(pObj) == iColor) {
              iCount++;
              continue;
            }
          aList[j-1] = pObj;
          j++;
        }
        AddMenuItem("$TxtSell$","SellItem",iID,pClonk,iCount,ObjectNumber(pClonk),GetDesc(pObjPic),132,pObjPic,GetValue(pObjPic,0,0,iPlr));
        if(j == 1) break;
        SetLength(aList,j-1);
        iCount = 0;
        j = 1;
      }
    }
*/
  }
  return(1);
}

private func SellItem(id iID, int iClonk, bool fSpecial, int iVal)
{
  var pClonk, pObj, i, iSellPlayer, iCashPlayer, iValue, idSellTo;
  // Bank vorhanden
  if(!GetBank(this())) return(0);

  pClonk = Object(iClonk);

  // Objekt noch vorhanden?
  if(FindContents(iID)) {
    iCashPlayer = GetOwner(pClonk);
    if(IndividualOffers()) iSellPlayer = iCashPlayer;
    else iSellPlayer = HomebaseMaterialPlayer();

    while(pObj = FindContents(iID)) {
/*
      iValue = GetValue(pObj,0,this(),iCashPlayer);
      i += iValue;
      DoWealth(iCashPlayer,iValue);
      if(GetDefRebuy(iID)) {
        if(idSellTo = (pObj->~SellTo(iSellPlayer)))
          DoHomebaseMaterial(iSellPlayer,idSellTo,+1);
        else
          DoHomebaseMaterial(iSellPlayer,iID,+1);
      }
      RemoveObject(pObj,1);
      if(!fSpecial) break;
    }
    if(i) Sound("Cash",1,0,0,iCashPlayer+1);
*/
      iValue = SellObject(pObj,iCashPlayer);
      DoWealth(iCashPlayer,iValue);
      if(!fSpecial) break;
    }
    if(iValue > 0) Sound("Cash",1,0,0,iCashPlayer+1);
    if(iValue < 0) Sound("UnCash",1,0,0,iCashPlayer+1);
  }
  // Menü neu erzeugen
  if(pClonk && Contained(pClonk) == this()) {
    i = GetMenuSelection(pClonk);
    CloseMenu(pClonk);
    ContainedDig(pClonk);
    SelectMenuItem(i,pClonk);
    // Vllt. letztes Objekt verkauft?
    if(GetMenuSelection(pClonk) != i)
      SelectMenuItem(i-1,pClonk);
  }
  return(1);
}

private func SellObject(object pObject, int iPlayer)
{
  var i, iValue, idSellTo, pObj, idObj;
  idObj = GetID(pObject);
  iValue = GetValue(pObject,0,this(),iPlayer);
  if(GetDefRebuy(idObj)) {
    if(idSellTo = (pObject->~SellTo(iPlayer)))
      DoHomebaseMaterial(iPlayer,idSellTo,+1);
    else
      DoHomebaseMaterial(iPlayer,idObj,+1);
  }
  while(pObj = Contents(0,pObject)) {
    idObj = GetID(pObj);
    // Auf diese Art und Weise sollen sich keine Clonks/unverkaufbare Objekte verkaufen lassen
    if(GetDefCrewMember(idObj) || GetDefNoSell(idObj)) {
      if(!Contained(pObject)) Exit(pObj);
      else Enter(Contained(pObject),pObj);
    }
    else iValue += SellObject(pObj,iPlayer);
  }
  RemoveObject(pObject);
  return(iValue);
}


/* Automatisches Kaufen */

public func ObjectAvailable(id def, int iPlr)
{
  var sellPlayer = HomebaseMaterialPlayer();   // Spieler, bei dem gekauft wird
  if(IndividualOffers()) sellPlayer = iPlr;
  return(GetHomebaseMaterial(sellPlayer, def));
}

public func AutoBuy(object pClonk, id def)
{
  // Clonk soll Kaufen
  AddCommand(pClonk, "Call", this(), def, 0,0,0, "AutoBuyProcess");
  // Clonk soll aber erst hier reinkommen
  AddCommand(pClonk, "Enter", this(), 0,0,0,0,0,0, 1);
}

public func AutoBuyProcess(object pClonk, id def)
{
  // Clonk ist jetzt hier im Gebäude?
  if(Contained(pClonk) != this()) return();
  // Dann Kaufen
  return(BuyItem(def, ObjectNumber(pClonk), false, GetValue(0,def,0,GetOwner(pClonk))));
}

/* Banditen */

private func NoBandits()
{
  Message("$NoBandits$", this());
}

/* Steuerung */

protected func ContainedUp(object pClonk)
{
  [$BuyMenu$]
  OpenBuyMenu(pClonk);  
}

protected func ContainedDig(object pClonk)
{
  [$SellMenu$]
  OpenSellMenu(pClonk);  
}

protected func ContextSell(object pClonk)
{
  [$SellMenu$|Image=DRST]
  OpenSellMenu(pClonk);  
}

protected func ContextBuy(object pClonk)
{
  [$BuyMenu$|Image=DRST]
  OpenBuyMenu(pClonk);  
}

