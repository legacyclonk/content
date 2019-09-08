/*-- Palisade --*/

#strict

#include CPW2 // Burgteilfunktion

protected func Initialize()
{
  // Bei Fertigstellung an anderen Teilen ausrichten
  CastlePartAdjust();
  // In den Hintergrund versetzen, damit davor 
  // andere Gebäude gebaut werden können
  SetCategory(C4D_StaticBack());
  // Transferzone setzen
  UpdateTransferZone();
  // Objekte im Umkreis ihre Überprüfungen durchführen lassen
  CastlePartInitialize();
}

/*-- Ausrichtung von Burgbauteilen --*/

public func CastlePartWidth() { return (64); }
public func CastlePartHeight() { return (64); }
public func CastlePartTop() { return (-14); }
public func CastlePartBasement() { return(); } // Kein Fundament


/* Transfer */

protected func UpdateTransferZone() 
{
  if (GetOCF() & OCF_Fullcon())
    SetTransferZone(-32, -40, 64, 72);
	return(1);
}

protected func ControlTransfer(object pObj, int iTx, int iTy)
{
  // Aufstieg
  if (iTy < GetY() - 18)
    return(HandleTransferAscend(pObj, iTx, iTy));

  // Abstieg
  if (pObj->GetY() < GetY() - 10)
    return(HandleTransferDescend(pObj, iTx, iTy));

  // Einfach weiterlaufen
	return(0);
}

private func HandleTransferAscend(object pObj, int iTx, int iTy) 
{
  // Für Nichtkletterer kann ich eh nix machen
  if (!GetPhysical("CanScale", 0, pObj)) return(0);
  // Wenn der Clonk an der Leiter klettert: Hochklettern
  if (pObj->GetAction() S= "Scale")
    if (Inside(pObj->GetX(), GetX() - 20, GetX() - 10) && Inside(pObj->GetY(), GetY() - 22, GetY()))
    {
    	AddCommand(pObj, "Wait", 0, 0, 0, 0, 35);
    	pObj->SetComDir(COMD_Up());
      return(1);
    }
  // Nix machen, wenn der Clonk grad springt
  if (pObj->GetAction() S= "Jump") return(0);
  // Nix machen, wenn der Clonk schon oben ist
  if (pObj->GetY() < GetY() - 18) return(0);
  // Clonk in Sprungposition bringen
  if (!(Inside(pObj->GetX(), GetX() - 2, GetX() + 6) && Inside(pObj->GetY(), GetY() + 7, GetY() +  32)))
    return(AddCommand(pObj, "MoveTo", 0, GetX() + 2, GetY() + 22));
  // ggf. Gerät loslassen
  if (pObj->GetAction() S= "Push")
    return(AddCommand(pObj, "UnGrab"));
  // Springen
	return(AddCommand(pObj, "Jump", 0, GetX() - 15, GetY() - 10));
}

private func HandleTransferDescend(object pObj, int iTx, int iTy)
{
  // An der Leiter runterklettern
  if (pObj->GetAction() S= "Scale")
    if (Inside(pObj->GetX(), GetX() - 20, GetX() - 10) && Inside(pObj->GetY(), GetY() - 22, GetY()))
    {
    	AddCommand(pObj, "Wait", 0, 0, 0, 0, 35);
    	pObj->SetComDir(COMD_Down());
      return(1);
    }
  // Zur Leiter bewegen
  if (pObj->GetX() < GetX() - 17)
  	SetComDir(COMD_Right(), pObj);
  else
  	SetComDir(COMD_Left(), pObj);
  // Warten
	return(AddCommand(pObj, "Wait", 0, 0, 0, 0, 4));
}