/*-- Goldmine --*/

#strict
#include OREM

public func IsFulfilled()
{
  // Gold noch nicht gezählt?
  var GoldCnt = GetMaterialCount(Material("Gold"));
  if (GoldCnt == -1) return();
  // Noch zu viel?
  if (GoldCnt >= 150) return();
  // Noch Goldstücke abzubauen?
  return(!ObjectCount(GOLD));
}

func SellID () { return (GOLD); }

protected func Activate(iPlayer)
{
  // Alles abgebaut
  if (IsFulfilled()) 
    return(MessageWindow("$MsgGoalFulfilled$", iPlayer));
  var goldTotal = ObjectCount2(Find_ID(GOLD));
  var goldOutside = ObjectCount2(Find_ID(GOLD), Find_NoContainer());
  // Noch nicht alles abgebaut (fest Menge noch unbekannt)
  if (GetMaterialCount(Material("Gold")) == -1)
    return(MessageWindow(Format("$MsgGoalUnfulfilled2$", goldTotal, goldOutside), iPlayer));
  // Noch nicht alles abgebaut ( -50 statt -150 weil beim / 100 abgerundet wird )
  var amountSolid = Max((GetMaterialCount(Material("Gold")) - 50) / 100, 0);
  MessageWindow(Format("$MsgGoalUnfulfilled$", amountSolid, goldTotal, goldOutside), iPlayer);
}
