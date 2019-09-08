/*-- Seepferdchen (gesattelt) --*/

#strict
#include HICP
local iGait; // Gangart: 0 Stehen, 1 Gehen, 2 Trab, 3 Galopp
// entspricht dem Originalfisch außer ~Edible=1 in der DefCore und 
local energy;

public func ControlDownDouble(pByObject)
{
  [$CtrlGetOffDesc$]
  // Direkte Steuerung löscht Kommandos
  ClearCommands();
  // Reiter: absitzen
  if (pByObject == GetRider())
    { ObjectSetAction(pByObject,"Walk"); }
  SetGait(0);
  SetComDir(COMD_Stop());
  // Keine Steuerung
  return(1);
}

// bis hier

/* TimerCall */

private func Activity() 
{
// Angehalten: Stehen
  if (GetComDir() == COMD_Stop())
    if (!GetXDir())
      if (GetAction() eq "Walk" || GetAction() eq "Trot" || GetAction() eq "Gallop")
        SetAction("Stand");
  // Bewegungskommando: Aktion anpassen
  if (GetCommand() eq "MoveTo")
    MoveToAdjustAction();
  // Ggf. Reiterposition anpassen
  if(GetAction() ne "Jump")
    ResetRiderPos();   
  // Pferd steht?
  if(GetAction() eq "Stand"){ SetGait(iGait); SetYDir(15);}
return();
}

public func OpenSaddleBagMenu(pClonk, iSelection)
{
  if(Local()) CloseMenu(Local());
  Local() = pClonk;
  var sCaption = Format("$MsgEmpty$", GetName());
  CreateMenu(GetID(),pClonk,this(),0,sCaption,0,0, 1);
  // Menü aufbauen
  var loop;
  var obj;
  var oldObj;
  var oldObjCount;
  var MenuItemCount=-1;
  if(Contents(0, pClonk)) AddMenuItem(Format("$MsgPut$", GetName(Contents(0, pClonk))), "Put", PUCN, pClonk, 0, pClonk); 
  while(obj = Contents(loop++))
	{
		if(GetID(obj)==GetID(oldObj))
		{
			oldObjCount++;
			continue;
		}
		if(oldObj)
		{
			AddMenuItem("$MsgFetch$", "Take", GetID(oldObj), pClonk, oldObjCount, pClonk); 
			MenuItemCount++;
		}
		oldObj=obj;
		oldObjCount=1;
	}
  if(oldObj)
  {
	AddMenuItem("$MsgFetch$", "Take", GetID(oldObj), pClonk, oldObjCount, pClonk); 
	MenuItemCount++;
  } 
  if(MenuItemCount>=iSelection) SelectMenuItem(iSelection, pClonk);
  else SelectMenuItem(MenuItemCount, pClonk);
  return(1);
}

public func Take(id, pClonk, bRight)
{
	var iSelection=GetMenuSelection(pClonk);
	if(bRight) { pClonk->~ControlSpecial(); OpenSaddleBagMenu(pClonk, iSelection); return(); }
	var obj = FindContents(id);
	if(!obj) return(0);
	Enter(pClonk, obj);
	if(ContentsCount(0, pClonk)>GetDefCoreVal("CollectionLimit", "DefCore", GetID(pClonk)))
		Enter(this(), Contents(ContentsCount(0, pClonk)-1, pClonk));
	OpenSaddleBagMenu(pClonk, iSelection);
}

public func Put(id, pClonk, bRight)
{
	var iSelection=GetMenuSelection(pClonk);
	if(bRight) { pClonk->~ControlSpecial(); OpenSaddleBagMenu(pClonk, iSelection); return(); }
	var obj = Contents(0, pClonk);
	if(!obj) return(0);
	Enter(this(), obj);
	if(ContentsCount(0, this())>GetDefCoreVal("CollectionLimit", "DefCore", GetID(this())))
		Enter(pClonk, Contents(ContentsCount(0, this())-1, this()));
	OpenSaddleBagMenu(pClonk, iSelection);
}
