/*-- Speerständer --*/

#strict

public func RackedItem() { return(SPER); }
public func RackedItemCount() { return(5); }

protected func Initialize()
  {
  SetAction("ShowSpears");
  return(1);
  }

// Zählt vorhandene Bögen
private func CountContents()
  {
  SetPhase(ContentsCount(RackedItem()));
  return(1);
  }

protected func RejectCollect(idObj, pObj)
  {
  // Verhindert Aufnahme aller nichtpassender Objekte
  if( idObj != RackedItem()) return(1);
  // Maximal 5 Stück
  if (ContentsCount(RackedItem()) >= RackedItemCount())
    return(1, Message( "$MsgRackFull$",this() ));
  return(0);
  }
  
protected func Collection2()
  {
  Sound("Clonk");
  CountContents();
  return(1);
  }

protected func Ejection()
  {
  CountContents();
  return(1);
  }

protected func ControlDig(pClonk)
  {
  [$CtrlTake$|Image=SPER]
  // Gegenstand nehmen
  var pItem = Contents();
  if (!pItem) return(Message("$MsgRackEmpty$", this()));
  if (Collect(pItem, pClonk)) Sound("Grab");
  return(1);
  }
