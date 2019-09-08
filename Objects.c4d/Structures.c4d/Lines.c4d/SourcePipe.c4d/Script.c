/*--- Zuflussrohr ---*/

#strict

protected func Initialize()
{
  SetAction("Connect");  
  SetVertex(0, 0, GetX()); SetVertex(0, 1, GetY());
  SetVertex(1, 0, GetX()); SetVertex(1, 1, GetY());
  SetPosition(0, 0, this());
}

protected func Transfer()
{
  // Zielobjekt suchen
  var pPumpTarget = GetActionTarget(0);
  // Kein Zielobjekt  
  if (!pPumpTarget) return(0);
  // Zielobjekt akzeptiert keinen Transfer
  if (!pPumpTarget->~AcceptTransfer()) return(0);
  // Zielobjekt hat nicht genug Energie (könnte man auch in AcceptTransfer verlegen...)
  if (!EnergyCheck(1000,pPumpTarget)) return(0);
  // Materialtransfer
  for (var i = 0; i < 3; i++)
  {
    var mat = ObjectExtractLiquid(GetActionTarget(1));
    if (mat == -1) mat = ExtractLiquid(GetVertex(50, 0), GetVertex(50, 1));
    ObjectInsertMaterial(mat, pPumpTarget);
  }
  // Blubbern  
  if (!Random(5)) Bubble(GetVertex(50,0), GetVertex(50,1));
}

public func LineBreak(bool fNoMsg)
{
  Sound("LineBreak");
  if (!fNoMsg) BreakMessage();
}

private func BreakMessage()
{
  var pPumpTarget = GetActionTarget(0);
  if (GetID(pPumpTarget) != LNKT)
    pPumpTarget = GetActionTarget(1);
  Message("$TxtLinebroke$", pPumpTarget);
}
