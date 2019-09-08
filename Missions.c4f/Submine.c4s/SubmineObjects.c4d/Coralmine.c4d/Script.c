/*-- Korallenabbau --*/

#strict 2
#include GOAL

static const _CRM_MatTolerance = 150; // Anzahl Einheiten, die nicht abgebaut werden muessen

local total_corals_sold;
// Local(i): Korallen verkauft durch Spieler i

private func GetCoralMaterialCount()
{
  // Feste Materialmenge in der Landschaft
  return GetMaterialCount(Material("Coral1"))
        + GetMaterialCount(Material("Coral2"))
        + GetMaterialCount(Material("Coral3"));
}

private func GetCoralItemCount()
{
  // Lose Korallenstuecke
  // ObjectCount(C4ID) ist wegen Definitionszaehler hier am schnellsten
  return ObjectCount(GCOR)+ObjectCount(RCOR)+ObjectCount(BCOR);
}

public func IsFulfilled()
{
  // Korallen noch nicht gezählt?
  var coral_mat_count = GetCoralMaterialCount();
  if (coral_mat_count < 0) return false;
  // noch zu viel?
  if (coral_mat_count >= _CRM_MatTolerance) return false;
  // noch lose Stuecke abzubauen?
  return !GetCoralItemCount();
}

public func IsFulfilledforPlr (plr)
{
  // Wettkampf: Mehr Korallen verkaufen als der Gegner!
  if (!IsFulfilled()) return false;
  var i = GetPlayerCount();
  while (i--)
    if (Local (GetPlayerByIndex (i)) > Local (plr)) return false;
  return true;
}

func SellIDs () { return [GCOR, RCOR, BCOR]; }

public func PlayerHasSold (plr, obj)
{
  if (plr>=0 && GetIndexOf(GetID(obj), SellIDs ()))
  {
    ++Local (plr);
    ++total_corals_sold;
  }
}

public func PlayerHasBought (plr, obj)
{
  if (plr>=0 && GetIndexOf(GetID(obj), SellIDs ()))
  {
    --Local (plr);
    --total_corals_sold;
  }
}

protected func Activate(iPlayer)
{
  // Alles abgebaut
  if (IsFulfilled()) 
  {
    MessageWindow("$MsgGoalFulfilled$", iPlayer);
  }
  else
  {
    // Noch nicht alles abgebaut (fest Menge noch unbekannt)
    var coral_mat_count = GetCoralMaterialCount();
    if (coral_mat_count < 0)
    {
      MessageWindow(Format("$MsgGoalUnfulfilled2$", GetCoralItemCount()), iPlayer);
    }
    else
    {
      // Noch nicht alles abgebaut ( +100 weil beim / 100 abgerundet wird )
      MessageWindow(Format("$MsgGoalUnfulfilled$", Max(coral_mat_count - _CRM_MatTolerance + 100, 0) / 100, GetCoralItemCount()), iPlayer);
    }
  }
  return true;
}
