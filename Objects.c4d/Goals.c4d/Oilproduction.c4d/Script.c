/*-- Ölförderung --*/

#strict
#include OREM

public func IsFulfilled () {
  var OilCnt = GetMaterialCount (Material ("Oil"));
  if (OilCnt == -1) return ();
  if (OilCnt >= 100) return ();
  return (!ObjectCount (OBRL));
}

public func PlayerHasSold (plr, obj) {     
  if (GetID (obj) == OBRL) Local (plr) += obj -> OBRL::CalcValue();
} 

protected func Activate (plr) {
  if (IsFulfilled ()) return (MessageWindow ("$done$", plr));
  // Fördermenge noch unbekannt
  if (GetMaterialCount(Material("Oil")) == -1)
    return(MessageWindow(Format("$todo2$", ObjectCount(OBRL)), plr));
  // +100, damit ab 100 eine Zahl ungleich 0 angezeigt wird
  MessageWindow(Format("$todo$", (GetMaterialCount(Material("Oil")) + 100) / 200, ObjectCount(OBRL)), plr);
}
