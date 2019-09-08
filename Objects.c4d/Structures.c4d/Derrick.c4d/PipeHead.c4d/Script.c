/*--- Bohrkopf ---*/

#strict

protected func DestructCheck()
{
  if (!FindLine()) RemoveObject();
}

private func FindDerrick()
{
  var pDerrick = FindObject(DRCK,0,0,0,0,0,"Pump",this());
  if (pDerrick) return(pDerrick);
  return (FindObject(DRCK,0,0,0,0,0,"Wait",this()));
}

private func FindLine()
{
  return(1);
}

private func Processing()
{
  // Normale Geschwindigkeit
  SetPhysical("Float", 150, 2);
  // Materialtransfer
  Transfer();
  // An der Oberkante: anhalten
  if (GetY() < GetY(FindDerrick()) + 36)
  {
    SetPosition(GetX(), GetY(FindDerrick()) + 36);
    SetAction("Stop");
    SetComDir(COMD_Stop());
    SetYDir(0);
    Sound("Click");
  }
}

protected func ContactBottom()
{
  SetAction("Stop");
  SetComDir(COMD_Stop());
  SetYDir(0);
  Sound("Click");
}

private func Transfer()
{
  // Zielobjekt ermitteln
  var pPumpTarget = GetActionTarget(0);
  // Kein Zielobjekt  
  if (!pPumpTarget) return(0);
  // Zielobjekt erlaubt keinen Transfer
  if (!pPumpTarget->~AcceptTransfer()) return(0);
  // Zielobjekt hat nicht genug Energie  
  if ((!EnergyCheck(1000, pPumpTarget)) && ObjectCount(ENRG)) return(0);
  // Materialtransfer
  for (var i = 0; i < 3; i++)  
    ObjectInsertMaterial(ExtractLiquid(0, 0), pPumpTarget);
  // Blubbern
  if (!Random(5)) Bubble(0, 0);
  // Fertig
  return(1);
}

private func Drilling()
{
  // Material prüfen
  var mat = GetMaterial(0,3);
  // Nicht durch SolidMasks bohren
  if (Material("Vehicle") == mat)
  {
    Sound("MetalHit3");
    return(0);
  }
  // Schneller in freien Bereichen
  if ((Material("Tunnel") == mat) || (Material("Sky") == mat))
  {
    SetPhysical("Float", 150, 2);
    return(1);
  }
  // Langsamer in festen Materialien
  if (!GetMaterialVal("DigFree", "Material", mat))
    if (!GetMaterialVal("Instable", "Material", mat))
    {
      var iSpeed = 25;
      // Etwas hardgecodete Erkennung
      if (Material("Granite") == mat)
        iSpeed = 10;
      // Langsamere Geschwindigkeit
      SetPhysical("Float", iSpeed + Random(2 * iSpeed), 2);
      FreeRect(GetX() - 1, GetY(), 3, 4, C4M_Solid);
      return(1);
    }
  // Normale Geschwindigkeit
  SetPhysical("Float", 100, 2);
  return(1);
}

