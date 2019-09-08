/*-- Palisade --*/

#strict

#include PALS

/* Initialisierung */

protected func Initialize()
{
  // In den Hintergrund versetzen
  SetCategory(C4D_StaticBack());
  // Bei Fertigstellung an anderen Teilen ausrichten
  AdjustPalisadePosition();
  // Transferzone setzen
  UpdateTransferZone();
  FPalisadeChange();
}

/*-- Ausrichtung von Burgbauteilen --*/

public func PalisadeTop()      { return(-14); }

/* Transfer */

protected func UpdateTransferZone()
{
  if(GetOCF()&OCF_Fullcon())
    SetTransferZone(-32, -40, 64, 72);
}

protected func ControlTransfer(object pObj, int iTx, int iTy)
{
  // Aufstieg & Abstieg
  if(iTy < GetY()-18)        return(HandleTransferAscend(pObj,iTx,iTy));
  if(GetY(pObj) < GetY()-10) return(HandleTransferDescend(pObj,iTx,iTy));
}

private func HandleTransferAscend(object pObj, int iTx, int iTy)
{
  // Für Nichtkletterer kann ich eh nix machen
  if(!GetPhysical("CanScale",0,pObj)) return();
  // Wenn der Clonk an der Leiter klettert: Hochklettern
  if(GetAction(pObj) eq "Scale")
    if( Inside(GetX(pObj),GetX()-20,GetX()-10) && Inside(GetY(pObj),GetY()-22,GetY()) )
    {
      AddCommand(pObj,"Wait",0,0,0,0,35);
      SetComDir(COMD_Up(),pObj);
      return();
    }
  // Nix machen, wenn der Clonk grad springt
  if(GetAction(pObj) eq "Jump") return();
  // Nix machen, wenn der Clonk schon oben ist
  if(GetY(pObj) < GetY()-18) return();
  // Clonk in Sprungposition bringen
  if(!( Inside(GetX(pObj),GetX()-2,GetX()+6) && Inside(GetY(pObj),GetY()+7,GetY()+32) ))
    return(AddCommand(pObj,"MoveTo",0,GetX()+2,GetY()+22));
  // ggf. Gerät loslassen
  if(GetAction(pObj) eq "Push")
    return(AddCommand(pObj, "UnGrab"));
  // Springen
  AddCommand(pObj, "Jump", 0, GetX()-15, GetY()-10);
}

private func HandleTransferDescend(object pObj, int iTx, int iTy)
{
  // An der Leiter runterklettern
  if(GetAction(pObj) eq "Scale")
    if( Inside(GetX(pObj),GetX()-20,GetX()-10) && Inside(GetY(pObj),GetY()-22,GetY()) )
    {
      AddCommand(pObj,"Wait",0,0,0,0,35);
      SetComDir(COMD_Down(), pObj);
      return();
    }  
  // Zur Leiter bewegen
  if(GetX(pObj) < GetX()-17) SetComDir(COMD_Right(), pObj);
  else                       SetComDir(COMD_Left(), pObj);
  AddCommand(pObj,"Wait",0,0,0,0,4);
}

// kann durch das Amulett des Gelehrten erforscht werden
public func IsJungleResearch() { return(1); }

// ohne Turm keine Palisade
public func GetResearchBase() { return(WGTW); }

// durch Palisade erforschbar
public func GetResearchLink() { return(PALS); }