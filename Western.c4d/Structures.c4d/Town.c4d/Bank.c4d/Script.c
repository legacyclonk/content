/*-- Bank --*/

#strict
#include BAS9
#include DOOR

/* Geldausgabe */

public func RejectCollect(id idObj, object pObj)
{
  // Objekt ist Gold oder ein Sack voll Geld
  if(idObj != GOLD && idObj != MBAG) return();
  // Objekt wird von einem Clonk getragen
  if(Contained(pObj))
   if(GetOCF(Contained(pObj)) & OCF_CrewMember())
    {
    // Nicht für Banditen
    if(Contained(pObj)->~IsBandit()) return(Sound("Error", 1, 0, 0, GetOwner(pObj)+1));
    DoWealth(GetOwner(Contained(pObj)), GetValue(pObj));
    Sound("Cash", 0, 0, 0, GetOwner(Contained(pObj))+1);
    RemoveObject(pObj);
    return();
    }
}

private func ActivateEntrance(object Lorry)
{
  // Objekt ist eine Lore
  if(! Lorry->~IsLorry()) return(_inherited());
  // Spieler ist ein Bandit
  if(IsBanditPlayer(GetController(Lorry)))
  // Alle Inhaltsobjekte der Lore durchgehen
  for(var i=0 ; Contents(i, Lorry) ; i++)
   // Objekt ist ein Geldsack oder ein Goldklumpen
    if(GetID(Contents(i, Lorry)) == GOLD || GetID(Contents(i, Lorry)) == MBAG)
     {
     // Geld an kontrollierenden Spieler überweisen
     DoWealth(GetController(Lorry), GetValue(Contents(i, Lorry)));
     Sound("Cash", 0, 0,0, GetController(Lorry)+1);
     RemoveObject(Contents(i, Lorry));
     }
  return(_inherited(Lorry));
}

/* Spieler hebt Geld ab */

protected func OpenBankMenu(object pClonk)
{
  // Menü erzeugen
  CreateMenu(GetID(), pClonk, this(), 2, "$WithdrawMoney$");
  AddMenuItem("$WithdrawMoney$", "Withdraw", MBAG, pClonk, 0, pClonk, 0, 128, 0, 10);
  AddMenuItem("$WithdrawMoney$", "Withdraw", MBAG, pClonk, 0, pClonk, 0, 128, 0, 25);
  AddMenuItem("$WithdrawMoney$", "Withdraw", MBAG, pClonk, 0, pClonk, 0, 128, 0, 50);
  AddMenuItem("$WithdrawMoney$", "Withdraw", MBAG, pClonk, 0, pClonk, 0, 128, 0,100);
}

protected func Withdraw(id bagId, object pClonk, bool dummy, int iValue)
{
  // Clonk weg %(
  if(!pClonk) return();
  // Nicht für Banditen
  if(pClonk->~IsBandit()) return(Sound("Error", 1, 0, 0, GetOwner(pClonk)+1));
  // Spieler hat gar nicht so viel Geld
  if(GetWealth(GetOwner(pClonk)) < iValue) return(PlayerMessage(GetOwner(pClonk), "$NotEnoughMoney$", pClonk));
  // Geld weg!
  DoWealth(GetOwner(pClonk), -iValue);
  // Beutel erzeugen
  var cObj = pClonk;
  var bag = CreateContents(bagId, this());
  Collect(bag, pClonk);
  // Besitzer setzen
  SetOwner(GetOwner(pClonk), bag);
  // Wert setzen
  bag->~SetValue(iValue);
  // Geräusch!
  Sound("Uncash", 0, 0,0, GetOwner(pClonk)+1);
}

/* Türgeräusche */

protected func SoundOpenDoor() { Sound("GateOpen"); }
protected func SoundCloseDoor() { Sound("GateClose"); }

/* Globales */

// jegliche Geldgeschäfte sind nur erlaubt, wenn eine Bank vorhanden ist
global func GetBank(object pMessage, bool silent)   // pMessage: Objekt auf ggf. eine Fehler-Nachricht erscheint, silent: keine Fehlernachricht
{
  // Eine Bank vorhanden?
  if(FindObject(BANK, 0, 0, 0, 0, OCF_Fullcon)) return(1);
  // Ansonsten Fehlernachricht, wenn gewünschst
  if(silent) return();
  if(!pMessage) pMessage = this();
  Message(DefinitionCall(BANK, "ErrorMessage"), pMessage);
  return();
}

public func ErrorMessage() { return("$NoBank$"); }

/* Steuerung */

public func ContainedUp(object pClonk)
{
  OpenBankMenu(pClonk); 
}

public func ContextWithdraw(object pClonk)
{
  [$WithdrawMoney$|Image=MBAG]
  MakeContainedCall("OpenBankMenu", pClonk); 
}

/* Hilfsfunktionen */

private func MakeContainedCall(string strFunction, object pClonk)
{
  SetCommand(pClonk, "Call", this(), pClonk, 0, 0, strFunction);
  AddCommand(pClonk, "Enter", this());
}

global func IsBanditPlayer(int iPlr)    // Gibt zurück, ob ein bestimmter Spieler ein Bandit ist
{
  // Kein Spielziel
  if(!ObjectCount(BNKR)) return();
  // Spieler seltsam verschwunden
  if(GetPlayerByIndex() < 0) return();
  // Spielzielanfordern
  return(Local(iPlr, FindObject(BNKR)));
}
