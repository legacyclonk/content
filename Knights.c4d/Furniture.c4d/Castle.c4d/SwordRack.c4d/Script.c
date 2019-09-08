/*-- Schwertständer --*/

#strict

public func RackedItem() { return(SWOR); }
public func RackedItemCount() { return(5); }

protected func Initialize()
  {
  SetAction("ShowItems");
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
  Sound("RackPut");
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
  [$CtrlTake$|Image=SWOR]
  // Gegenstand nehmen
  var pItem = Contents();
  if (!pItem) return(Message("$MsgRackEmpty$", this()));
  if (Collect(pItem, pClonk)) Sound("RackPut");
  return(1);
  }
