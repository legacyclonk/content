/*-- Begrenzte Tränke --*/

#strict
#appendto CLD2

// Folgendes local bestimmt folgendes bei folgenden Objekten:
// Regel: Anzahl der zu nehmenden Tränke
// Kochtopf: Anzahl der genommenen Tränke
local iTaken;

protected func Initialize()
{
  if(GetID(this()) != CP4V) return(_inherited());
  var pOther;

  SetCount(1);
  while(pOther = FindObject(GetID())) {
    DoCount(pOther->~GetCount());
    RemoveObject(pOther);
  }
}

public func Activate(iPlayer)
{
  if(GetID(this()) != CP4V) return(_inherited(iPlayer, ...));
  if(GetCount() == 1)
    MessageWindow("$MsgTodoOne$",iPlayer);
  else
    MessageWindow(Format("$MsgTodo$",GetCount()),iPlayer);
}

public func GetCount() { return(iTaken); }
public func DoCount(iCount) { return(iTaken = Max(GetCount()+iCount,(GetID()==CP4V))); }
public func SetCount(iCount) { return(iTaken = Max(iCount,(GetID()==CP4V))); }

/*-- Kochtopf --*/

public func Fill(pBowl)
{
  _inherited(pBowl);

  DoCount(1);
  if(FindObject(CP4V))
   if(GetCount() >= FindObject(CP4V)->~GetCount()+(LocalN("fWater", this())!=0))
    DestroyContents();
}

protected func DestroyContents()
{
  _inherited();
  SetCount(0);
}

/*private func DestroyMenu(pClonk)
{
  _inherited(pClonk);
  if(FindObject(CP4V) && !(this()->~ExtraInformation(pClonk,1))) {
    var iCount = FindObject(CP4V)->~GetCount()+(LocalN("fWater", this())!=0)-GetCount();
    if(iCount == 1)  AddMenuItem("$YouCanTakeOne$",0,BOWL,pClonk);
    else AddMenuItem(Format("$YouCanTake$",iCount),0,BOWL,pClonk);
  }
  return(1);
}*/

// Da ich in den Western-Bugfixes den Topf schon überarbeitet habe:
private func ExtraInformation(pClonk)
{
  _inherited(pClonk);
  if(FindObject(CP4V)) {
    var iCount = FindObject(CP4V)->~GetCount()+(LocalN("fWater", this())!=0)-GetCount();
    if(iCount == 1)  AddMenuItem("$YouCanTakeOne$",0,BOWL,pClonk);
    else AddMenuItem(Format("$YouCanTake$",iCount),0,BOWL,pClonk);
  }
  return(1);
}