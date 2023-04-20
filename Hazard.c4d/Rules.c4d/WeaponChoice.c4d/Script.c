/*-- WeaponChoice --*/

#strict
#include WPVM

local WeaponChoice;
local spawntimer;
local spawnclonk;

protected func Activate(iPlr) {
  MessageWindow(GetDesc(),iPlr);
}

func Initialize() {
  SetPosition();
  WeaponChoice = CreateArray(0);
  inherited();
  AddWares("IsWeapon",-1);
  AddWares("IsEquipment",-1);
  AddWares("IsUpgrade",-1);
  SortWare("IsWeapon","IsEquipment","IsUpgrade");
}

func InitializePlayer(int iPlayer) {
  if(!GetLength(WeaponChoice[iPlayer]))
    WeaponChoice[iPlayer] = CreateArray(0);
  var pClonk = GetCursor(iPlayer);
  if(!pClonk || FindObject(CHOS)) {
    ScheduleCall(0,"InitializePlayer",1,0,iPlayer);
  }
  
  //Startgeld! :)
//  SetWealth(iPlayer,GetWealth(iPlayer)+75); Nöö
  // Für KIs aber schon (verzögert, damit die Crew erst beitreten kann):
  if(GetPlayerType(iPlayer) == C4PT_Script)
    Schedule(Format("SetWealth(%d, GetWealth(%d) * GetCrewCount(%d));", iPlayer, iPlayer, iPlayer), 1);
  ScheduleCall(0,"CreateWpnMenu",1,0,pClonk);
}

func RelaunchPlayer(int iPlr, object pClonk) {
  if(!pClonk)
    if(!(pClonk = GetCursor(iPlr)))
      return(ScheduleCall(this(),"RelaunchPlayer",1,0,iPlr,pClonk));
  if(!GetAlive(pClonk))
    return(ScheduleCall(this(),"RelaunchPlayer",1,0,iPlr));

  if(FindObject(GLMS)) {
	spawntimer = 10;
	spawnclonk = pClonk;
  }
	
  CreateWpnMenu(pClonk);
  return();
}

protected func Spawntimer() {
  if(spawntimer) {
  	spawntimer--;
	PlayerMessage(GetOwner(spawnclonk),"@$TimeTillRespawn$",0,spawntimer);
	if(!spawntimer) {
		Finish(0,spawnclonk);
	}
  }
}

//Erzeugt das ganze Waffenmenü
func CreateWpnMenu(object pClonk) {
  //Parameter vergessen?
  if(!pClonk)
    return(0);
  var iPlayer = GetOwner(pClonk);
    
  //Stuff damit man nicht Angreifbar ist in der Zwischenzeit
  if(GetID(Contained(pClonk)) != TIM1) {
    var tmp = CreateObject(TIM1,AbsX(GetX(pClonk)),AbsY(GetY(pClonk)),iPlayer);
    SetCategory(GetCategory(tmp) | C4D_Foreground,tmp);
    SetGraphics(0,tmp,GetID(pClonk),1,5,0,1,pClonk);
    SetVisibility(VIS_Owner,tmp);
    Enter(tmp,pClonk);
  }

  if(GetPlayerType(iPlayer) == C4PT_Script) return;

  //Schon ein Menü offen?
  if(GetMenu(pClonk)) {
    CloseMenu(pClonk); //zumachen.
  }
 
  if(!GetLength(WeaponChoice[iPlayer])) {
    var i,tmp;
    while(tmp = (pClonk->Contents(i))) {
      var cid = GetID(tmp);
	  if(FindIn(WeaponChoice[iPlayer],cid))
		continue;
      if(FindWare(cid))
        AddToArray(WeaponChoice[iPlayer],GetID(tmp));
      i++;
    }
  }
 
  //absicherung!
  SetWare(0,-1);
 
  CreateBuyMenu(pClonk);

  //Ende -> Menü
  AddMenuItem("$End$","Finish",WEPN,pClonk,0,pClonk);  
}

//gewähltes Zeugs markieren
private func AdaptMenuItem(object pObj,int iPlayer) {
  if(FindIn(WeaponChoice[iPlayer],GetID(pObj))) {
    SetGraphics("Chosen", pObj, WPCH, 1, GFXOV_MODE_Picture);
    pObj->SetClrModulation(RGB(200,200,200));
    SetObjDrawTransform(650,0,5000,0,650,5000, pObj, 1);
  }
}

//Die Kauffunktion überladen
private func BuyItem(id id, object pClonk) {
  var iPlayer = GetController(pClonk);
  var done;
  
  for(var i in WeaponChoice[iPlayer])
    if(i == id)
      if(FindIn(WeaponChoice[iPlayer],id)) {
        Uncheck(id,iPlayer,pClonk);
        done = true;
      }
  if(!done)
    Check(id,iPlayer,pClonk);
  
  Sound("Click",1,0,0,iPlayer+1);
  
  //Menü updaten
  var sel = GetMenuSelection(pClonk);
  CloseMenu(pClonk);
  CreateWpnMenu(pClonk);
  SelectMenuItem(sel,pClonk);
}

private func Uncheck(id ID, int iPlayer, object pClonk) {
  if(!ID)
    return();
  
  if(FindContents(ID,pClonk))
    RemoveObject(FindContents(ID,pClonk));
  
  for(var x; x < GetLength(WeaponChoice[iPlayer]); x++)
    if(WeaponChoice[iPlayer][x] == ID)
      WeaponChoice[iPlayer][x] = 0;
  SetWealth(iPlayer,GetWealth(iPlayer)+GetDefValue(ID));
}

private func Check(id ID, int iPlayer, object pClonk) {
  if(!ID)
    return();
  //genug Geld?
  if(GetDefValue(ID) > GetWealth(iPlayer))
    return(Sound("Error",1,0,0,iPlayer+1));
  //testen ob wir das Objekt überhaupt noch aufnehmen dürfen
  var tmp = CreateObject(ID,0,0,iPlayer);
  if(pClonk->~RejectCollect(ID,tmp)) {
    RemoveObject(tmp);  
    return(Sound("Error",1,0,0,iPlayer+1));
  }
  else
    Enter(pClonk,tmp);
  AddToArray(WeaponChoice[iPlayer],ID);
  SetWealth(iPlayer,GetWealth(iPlayer)-GetDefValue(ID));
}

func FindIn(array a, b) {
  for(var c in a)
    if(b == c)
      return(true);
  return(false);
}

func Finish(id unused, object pClonk, bool bRight) {
  if(bRight)
    return(MenuQueryCancel(0,pClonk));

  PlayerMessage(GetOwner(spawnclonk),"");
  spawnclonk = 0;
  spawntimer = 0;
  
  var iPlayer = GetOwner(pClonk);
  
  //Auswahl löschen
  WeaponChoice[iPlayer] = CreateArray();
  //Menü schließen
  CloseMenu(GetCursor(iPlayer));
  //Sound! :D
  Sound("Cash",1,0,0,iPlayer+1);
  
  //Alle Waffen auffüllen
  for(var wpn in FindObjects(Find_Container(pClonk), Find_Func("IsWeapon"))) {
    var ammo = wpn->GetFMData(FM_AmmoID);
    var load = wpn->GetFMData(FM_AmmoLoad);
    //erst entladen
    DoAmmo(ammo,-load, wpn);
    //dann neu reinladen
    DoAmmo(ammo, load, wpn);
    //und noch zusätzliche Muni für den Clonk
    DoAmmo(ammo, load, pClonk);
  }
  
  //Wieder angreifbar machen
  RemoveObject(Contained(pClonk),1);
}

func MenuQueryCancel(int iSelection, object pMenuObj) {
  var owner = GetOwner(pMenuObj);
  //Auswahl löschen
  for(var x in WeaponChoice[owner]) {
    Uncheck(x,owner,pMenuObj);
  }
  WeaponChoice[owner] = CreateArray(0);
  
  var sel = GetMenuSelection(pMenuObj);
  CloseMenu(pMenuObj);
  CreateWpnMenu(pMenuObj);
  SelectMenuItem(sel,pMenuObj);
  
  return(true);
}

public func GetWareCount(id ID) {
  for(var x; x < GetLength(aWare); x++)
    if(aWare[x] == ID)
      return -1;
}

// Kann mittels des Spielzielauswählers ausgewählt werden
public func IsChooseable() { return(1); }
