/*-- Fort-Wachturm --*/

#strict

#include PALC

local pExit, iC, bR, bL, pGrab;

/* Initialisierung */

protected func Initialize()
{
  // Türen sind geöffnet
  iC = 0;

  SetAction("BothOpened");
  SetEntrance(1);
  // Position in der Palisade
  AdjustPalisadePosition();
  FPalisadeChange();
  // SolidMask setzen
  UpdateSolidMask();
  // Eingang oben hinzufügen
  pExit = CreateObject(TWRE,0,-3,GetOwner());
  pExit->TWRE::Activate(this(),true);
  // Brustwehr hinzufügen (nur bei Bedarf)
  if(!FindPalisade(+GetObjWidth(), 0) )
  {
    bR = CreateObject(CBT1, 19, -2, GetOwner());
    bR->AttachTo(this(), DIR_Right());
  }
  if(!FindPalisade(-GetObjWidth(), 0) )
  {
    bL = CreateObject(CBT1,-19, -2, GetOwner());
    bL->AttachTo(this(), DIR_Left());
  }
  // Anfassobjekt
  ObjectSetAction(pGrab = CreateObject(CTWA,0,0,GetOwner()), "Attach", this());
  // Transfer-Zone
  UpdateTransferZone();
}

protected func Incineration() { if(pExit) RemoveObject(pExit); if(pGrab) RemoveObject(pGrab); }
protected func Destruction()  { if(pExit) RemoveObject(pExit); }

// Der Turm steht mit seinem Dach etwas über, deshalb rücken Palisaden näher
public func PalisadeXSub() { return(5); }

/* TimerCall */

private func CheckContents()
{
  while(Contents()) Exit(Contents(), 0,-3,0, 0); 
  return(1);
}

/* Steuerung */

protected func ControlLeft()
{
  [$TxtLeftgate$]
  Sound("WoodenGate");
  if(GetAction() eq "BothOpened")        return(LeftClosed());
  if(GetAction() eq "LeftClosed")  return(BothOpen());
  if(GetAction() eq "RightClosed") return(BothClosed());
  if(GetAction() eq "BothClosed")  return(RightClosed());
}

protected func ControlRight()
{
  [$TxtRightgate$]
  Sound("WoodenGate");
  if(GetAction() eq "BothOpened")        return(SetGraphics("2"), RightClosed());
  if(GetAction() eq "RightClosed") return(SetGraphics(""), BothOpen());
  if(GetAction() eq "LeftClosed")  return(SetGraphics("2"), BothClosed());
  if(GetAction() eq "BothClosed")  return(SetGraphics(""), LeftClosed());
}

/* Status */

private func RightGateOpen() { return(GetAction() ne "RightClosed" && GetAction() ne "BothClosed"); }
private func LeftGateOpen()  { return(GetAction() ne "LeftClosed"  && GetAction() ne "BothClosed"); }

/* Tor-Aktionen */

private func BothOpen()    { iC=0; UpdateSolidMask(); SetAction("BothOpened"); }
private func LeftClosed()  { iC=1; UpdateSolidMask(); SetAction("LeftClosed");  }
private func RightClosed() { iC=0; UpdateSolidMask(); SetAction("RightClosed"); }
private func BothClosed()  { iC=1; UpdateSolidMask(); SetAction("BothClosed");  }

private func UpdateSolidMask()
{
  SetSolidMask(168 + 56*iC,0,56,95);
}

/* Wenn der Turm mal runterfällt */

protected func Hit() { UpdateTransferZone(); }

/* Brustwehrsteuerung */

public func PalisadeChange()
{
  var iWdtH = GetObjWidth()/2, iAdjustRange = PalisadeRange();
  // Links eine Palisade
  if(FindPalisade( -iWdtH - iAdjustRange))
    // Brustwehr noch da
    if(bL)
      // Weg
      RemoveObject(bL);
  // Rechts eine Palisade
  if(FindPalisade( iWdtH + iAdjustRange))
    // Brustwehr noch da
    if(bR)
      // Weg
      RemoveObject(bR);
}

/* Transfer */

protected func UpdateTransferZone() 
{
  if(GetAction() eq "BothOpened")  { SetGraphics("");  iC=0; }
  if(GetAction() eq "RightClosed") { SetGraphics("2"); iC=0; }
  if(GetAction() eq "LeftClosed")  { SetGraphics("");  iC=1; }
  if(GetAction() eq "BothClosed")  { SetGraphics("2"); iC=1; }
  UpdateSolidMask();
  if(!pGrab) ObjectSetAction(pGrab = CreateObject(CTWA), "Attach", this());
  if(GetID(pExit)!=TWRE) ChangeDef(TWRE, pExit);
  SetTransferZone(-27,-25,54,73);
}

protected func ControlTransfer(object pObj, int iTx, int iTy)
{
  // Aufstieg zur Plattform
  if( Inside(iTx-GetX(),-27,+27) && Inside(iTy-GetY(),-50,0) )
    if( Contained(pObj) || GetY(pObj)>GetY()+10 )
      return(HandleTransferPlatform(pObj,iTx,iTy));
  
  // Abstieg von der Plattform
  if(!Contained(pObj))
    if( Inside(GetX(pObj)-GetX(),-30,+30) && Inside(GetY(pObj)-GetY(),-40,0) )
      if( Inside(iTx-GetX(),-29,+29) && Inside(iTy-GetY(),+15,+53) )
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
    if(Inside(iTx-GetX(),+20,+28))  return (0,AddCommand(pObj,"MoveTo",0,GetX()+26,GetY()-15));
    // Rausklettern links
    if(Inside(iTx-GetX(),-28,-20))  return (0,AddCommand(pObj,"MoveTo",0,GetX()-26,GetY()-15));
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
    return(Exit(pObj,0,-3,0,0));
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

// Loren nicht im Turm ausleeren
public func NoLorryEjection(object lorry) { return true; }
