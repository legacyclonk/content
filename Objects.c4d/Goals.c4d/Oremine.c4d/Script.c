/*-- Erzabbau --*/

#strict
#include GOAL

local Sold;

public func IsFulfilled()
{
  // Erz noch nicht gezählt?
  var OreCnt = GetMaterialCount(Material("Ore"));
  if (OreCnt == -1) return();
  // noch zu viel?
  if (OreCnt >= 150) return();
  // noch Erzstücke abzubauen?
  return(!ObjectCount(ORE1));
}

public func IsFulfilledforPlr (plr)
{
  if (!IsFulfilled()) return ();
  var i = GetPlayerCount();
  while (i--)
    if (Local (GetPlayerByIndex (i)) > Local (plr)) return (0);
  return (1);
}

func SellID () { return (ORE1); }

public func PlayerHasSold (plr, obj)
{
  if (GetID (obj) == SellID ())
  {
    ++Local (plr);
    ++Sold;
  }
}

public func PlayerHasBought (plr, obj)
{
  if (GetID (obj) == SellID ())
  {
    --Local (plr);
    --Sold;
  }
}

protected func Activate(iPlayer)
{
  // Alles abgebaut
  if (IsFulfilled()) 
    return(MessageWindow("$MsgGoalFulfilled$", iPlayer));
  // Noch nicht alles abgebaut (fest Menge noch unbekannt)
  if (GetMaterialCount(Material("Ore")) == -1)
    return(MessageWindow(Format("$MsgGoalUnfulfilled2$", ObjectCount(ORE1)), iPlayer));
  // Noch nicht alles abgebaut ( -50 statt -150 weil beim / 100 abgerundet wird )
  MessageWindow(Format("$MsgGoalUnfulfilled$", Max(GetMaterialCount(Material("Ore")) - 50, 0) / 100, ObjectCount(ORE1)), iPlayer);
}
