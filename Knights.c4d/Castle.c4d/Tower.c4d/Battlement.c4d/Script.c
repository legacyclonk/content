/*-- Brustwehr --*/

#strict

public func AttachTo(object pTarget, int iDir)
{
  SetAction("Attach", pTarget);
  SetDir(iDir);
  SetSolidMask(4, 15 * GetDir(), 3, 14);
  return(1);
}

private func CheckTarget()
{
  // Anderes Bauteil im Hintergrund
  if (FindCastlePart(0, +1))
    return(RemoveObject());
  // Zugbrücke im Hintergrund
  if (FindObject(CPBL, 0, -1) || FindObject(CPBR, 0, -1))
    return(RemoveObject());
  // Wand ist zersört worden
  if (!GetActionTarget())
    return(RemoveObject());
  if (!(GetActionTarget()->GetOCF() & OCF_Fullcon()) || !GetActionTarget()->~CastlePartWidth())
    return(RemoveObject());
  return(1);
}

public func CastleChange()
{
	return(CheckTarget());
}	

private func FindCastlePart(int iOffsetX, int iOffsetY)
{
  // Anderes Bauteil in entsprechender Richtung suchen
  var pPart;
  while (pPart = FindObject(0, iOffsetX, iOffsetY, 0,0, 0,0,0, NoContainer(), pPart))
    if (pPart->~CastlePartWidth())
      return(pPart);
  // Keins gefunden
  return(0);
}
  
private func FindStaircase(int iOffsetX, int iOffsetY)
{
  // Anderes Treppenhaus in entsprechender Richtung suchen
  var pPart;
  while (pPart = FindObject(0, iOffsetX, iOffsetY, 0,0, 0,0,0, NoContainer(), pPart))
    if (pPart->GetOCF() & OCF_Fullcon())
      if (pPart->~IsStaircase())
      	return(pPart);
  // Keins gefunden
  return(0);
}

/* Transfer */

protected func UpdateTransferZone()
{
  SetTransferZone(-18 + 15 * GetDir(), -15, 20, 30);
  // SolidMasks alter Burgen anpassen
  return(SetSolidMask(4, 14 * GetDir(), 3, 14));
}

protected func ControlTransfer(object pClonk, int iTx, int iTy)
{  
  // Hinweis: ControlTransfer der Brustwehr gibt immer 0 zurück,
  //          selbst wenn Bewegungskommandos gesetzt wurden, da
  //          der Clonk sich nie wirklich durch die Transferzone
  //          der Brustwehr bewegen soll.

  var pStairs;
  
  // Clonk ist unterhalb der Brustwehr...
  if (pClonk->GetY() > GetY() + 15)
    // ...und will nach oben hinter die Brustwehr
    if (iTy < GetY() + 15)    
    	if (((GetDir() == 0) && (iTx > GetX())) || ((GetDir() == 1) && (iTx < GetX() - 2)))
        // Soll durch das darunterliegende Treppenhaus aufsteigen
        if (pStairs = FindStaircase(10 - 20 * GetDir(), 10))
        {
        	AddCommand(pClonk, "MoveTo", 0, pStairs->GetX(), iTy);
        	AddCommand(pClonk, "MoveTo", 0, pStairs->GetX(), pStairs->GetY());
        	return(0);
       	}

  // Clonk ist oberhalb der Brustwehr...
  if (pClonk->GetY() < GetY() + 15)
    // ...und will nach unten
    if (iTy > GetY() + 14)
      // Soll durchs Treppenhaus gehen
      if (pStairs = FindStaircase(10 - 20 * GetDir(), 10))
      {
      	AddCommand(pClonk, "MoveTo", 0, pStairs->GetX(), iTy);
      	return(0);
     	}
    
  // Clonk will direkt horizontal durch die Transferzone (ungewöhnlich)
  if (Inside(pClonk->GetY() - GetY(), -14, +14))
    if (Inside(iTy - GetY(), -14, +14))
      // Soll er drübersteigen
      {
      	AddCommand(pClonk, "MoveTo", 0, GetX(), GetY());
      	return(0);
      }

  return(0);
}
