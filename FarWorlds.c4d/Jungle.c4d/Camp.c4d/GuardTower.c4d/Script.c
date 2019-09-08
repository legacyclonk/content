/*-- Holz-Wachturm --*/

#strict

#include PALS

local pExit,iB,iC, pGrab;

/* Initialisierung */

protected func Initialize()
{
  iC=0;

  // Bei Fertigstellung an anderen Teilen ausrichten
  AdjustPalisadePosition();
  
  SetEntrance(1);
  SetSolidMask(96,0,48,74,0,0);
  // Eingang oben hinzufügen
  pExit = CreateObject(TWRE,0,-16,GetOwner());
  pExit->TWRE::Activate(this(),true);
  // Transferzone setzen
  UpdateTransferZone();
  PalisadeChange();
  // Anfassobjekt
  ObjectSetAction(pGrab = CreateObject(WTWA), "Attach", this());
}

public func PalisadeChange()
{
  iB = 0;
  if(FindPalisade(-GetObjWidth(), 0) ) iB = 1;
  if(FindPalisade(+GetObjWidth(), 0) )
  {
    if(!iB) iB = 2;
    else iB = 3;
  }
  UpdateSolidMask();
}

protected func Incineration() { if(pExit) RemoveObject(pExit); if(pGrab) RemoveObject(pGrab); }
protected func Destruction()  { if(pExit) RemoveObject(pExit); if(pGrab) RemoveObject(pGrab); }

/* TimerCall */

private func CheckContents()
{
  while(Contents()) Exit(Contents(), 0,-16,0, 0);  
  return(1);
}

/* Steuerung */

protected func ControlLeft()
{
  [$TxtLeftgate$]
  Sound("WoodenGate");
  if(GetAction() eq "Idle")        return(SetGraphics("2"), LeftClosed());
  if(GetAction() eq "LeftClosed")  return(SetGraphics(""), BothOpen());
  if(GetAction() eq "RightClosed") return(SetGraphics("2"), BothClosed());
  if(GetAction() eq "BothClosed")  return(SetGraphics(""), RightClosed());
}

protected func ControlRight()
{
  [$TxtRightgate$]
  Sound("WoodenGate");
  if(GetAction() eq "Idle")        return(RightClosed());
  if(GetAction() eq "RightClosed") return(BothOpen());
  if(GetAction() eq "LeftClosed")  return(BothClosed());
  if(GetAction() eq "BothClosed")  return(LeftClosed());
}

/* Status */

private func RightGateOpen() { return(GetAction() ne "RightClosed" && GetAction() ne "BothClosed"); }
private func LeftGateOpen()  { return(GetAction() ne "LeftClosed"  && GetAction() ne "BothClosed"); }

/* Tor-Aktionen */

private func BothOpen()    { iC=0; UpdateSolidMask(); SetAction("Idle"); }
private func LeftClosed()  { iC=0; UpdateSolidMask(); SetAction("LeftClosed");  }
private func RightClosed() { iC=1; UpdateSolidMask(); SetAction("RightClosed"); }
private func BothClosed()  { iC=1; UpdateSolidMask(); SetAction("BothClosed");  }

private func UpdateSolidMask()
{
  SetSolidMask(iB*96 + iC*48 + 96,0,48,74,0,0);
}

/* Wenn der Turm mal runterfällt */

protected func Hit() { UpdateTransferZone(); }

/* Transfer */

protected func UpdateTransferZone() 
{ 
  if(GetAction() eq "BothOpened")  { SetGraphics("");  iC=0; }
  if(GetAction() eq "RightClosed") { SetGraphics("");  iC=1; }
  if(GetAction() eq "LeftClosed")  { SetGraphics("2"); iC=0; }
  if(GetAction() eq "BothClosed")  { SetGraphics("2"); iC=1; }
  UpdateSolidMask();
  if(GetID(pExit)!=TWRE) ChangeDef(TWRE, pExit);
  SetTransferZone(-23,-50,46,88); 
}

protected func ControlTransfer(object pObj, int iTx, int iTy)
{
  // Aufstieg zur Plattform
  if( Inside(iTx-GetX(),-24,+24) && Inside(iTy-GetY(),-50,0) )
    if( Contained(pObj) || GetY(pObj)>GetY()+10 )
      return(HandleTransferPlatform(pObj,iTx,iTy));

  // Abstieg von der Plattform
  if(!Contained(pObj))
    if( Inside(GetX(pObj)-GetX(),-27,+27) && Inside(GetY(pObj)-GetY(),-40,0) )
      if( Inside(iTx-GetX(),-24,+24) && Inside(iTy-GetY(),+15,+53) )
        return(HandleTransferDescend(pObj,iTx,iTy));

  if(Inside(iTy-GetY(),-10,+43))
  {
    // Ausgang rechts
    if(Inside(iTx-GetX(),+20,+28))  return(HandleTransferGateRight(pObj,iTx,iTy));
    // Ausgang links
    if(Inside(iTx-GetX(),-28,-20))  return(HandleTransferGateLeft(pObj,iTx,iTy));
  }
  if(Inside(iTy-GetY(),-50,-11))
  {
    // Rausklettern rechts
    if(Inside(iTx-GetX(),+20,+28))  return (0,AddCommand(pObj,"MoveTo",0,GetX()+22,GetY()-15));
    // Rausklettern links
    if(Inside(iTx-GetX(),-28,-20))  return (0,AddCommand(pObj,"MoveTo",0,GetX()-22,GetY()-15));
  }
}

private func HandleTransferPlatform(object pObj, int iTx, int iTy)
{
  // Clonk ist auf der Plattform: erledigt
  if(!Contained(pObj))
    if(Inside(GetX(pObj)-GetX(),-24,+24) && Inside(GetY(pObj)-GetY(),-50,-10))
      return(0);
  // Clonk ist im Turm: aufsteigen
  if(Contained(pObj) == this())
    return(Exit(pObj,0,-18,0,0));
  // Clonk soll den Turm betreten
  return(AddCommand(pObj,"Enter",this()));
}
  
private func HandleTransferDescend(object pObj, int iTx, int iTy)
{
  // Clonk ist im Turm: warten
  if(Contained(pObj)==this()) return(1);
  // Clonk soll noch in die Mitte der Plattform laufen
  if(!(Inside(GetX(pObj)-GetX(),-20,+20) && Inside(GetY(pObj)-GetY(),-40,-10) ))
    return(AddCommand(pObj,"MoveTo",0,GetX(),GetY()-10));
  // Clonk in den Turm versetzen
  Enter(this(),pObj);
  // Clonk soll den Turm verlassen
  return(AddCommand(pObj,"Exit"));
}
  
private func HandleTransferGateRight(object pObj, int iTx, int iTy)
{
  // Tor ist offen: Turm loslassen
  if (RightGateOpen()) 
    if (GetAction(pObj) eq "Push" && GetActionTarget(0,pObj)==pGrab)
      return(AddCommand(pObj,"UnGrab",pGrab));      
  // Tor ist offen: erledigt
  if (RightGateOpen()) return(0);
  // Clonk soll den Turm anfassen
  if (!( GetAction(pObj) eq "Push" && GetActionTarget(0,pObj)==pGrab ))
    return(AddCommand(pObj,"Grab",pGrab));
  // Tor öffnen
  ControlRight(pObj);
}

private func HandleTransferGateLeft(object pObj, int iTx, int iTy)
{
  // Tor ist offen: Turm loslassen
  if (LeftGateOpen())
    if (GetAction(pObj) eq "Push" && GetActionTarget(0,pObj)==pGrab)
      return(AddCommand(pObj,"UnGrab",pGrab));      
  // Tor ist offen: erledigt
  if (LeftGateOpen()) return(0);
  // Clonk soll den Turm anfassen
  if (!( GetAction(pObj) eq "Push" && GetActionTarget(0,pObj)==pGrab ))
    return(AddCommand(pObj,"Grab",pGrab));
  // Tor öffnen
  ControlLeft(pObj);
}

// kann durch das Amulett des Gelehrten erforscht werden
public func IsJungleResearch() { return(1); }

// kann ohne Vorrausetzung erforscht werden
public func GetResearchBase() { return(0); }

/* Kann vom Indianer hergestellt werden */
public func IsIndianHandcraft() { return(1); }

/* Kann vom Trapper hergestellt werden */
public func IsTrapperHandcraft() { return(1); }

/* Kann von Indianern erforscht werden */
public func IsIndianResearch() { return(1); }

/* Erzwingt dass das Gebäude trotz Handcraft mit dem Bausatz gebaut werden kann */ 
public func IsConkitBuilding() { return(1); }

// Loren nicht im Turm ausleeren
public func NoLorryEjection(object lorry) { return true; }
