/*--- Planwagen ---*/

#strict

/* Initialisierung */

protected func Initialize()
{
  SetAction("Drive0");  
  SetEntrance(1);
}

/* Steuerung */

public func RedirectControl(string controlfunc)
{
  var pHorse;
  var fPulled = false;
  while(pHorse = FindPullingHorse(pHorse))
  {
    ProtectedCall(pHorse, controlfunc, Par(1), Par(2), Par(3), Par(4), Par(5), Par(6), Par(7));
    fPulled = true;
  }
  return(fPulled);
}

// Steuerung an ziehende Pferde weiterleiten
public func ControlLeft(pByObject) { return(RedirectControl("ControlLeft", pByObject)); }
public func ControlRight(pByObject) { return(RedirectControl("ControlRight", pByObject)); }
public func ControlDownSingle(pByObject) { [Method=Classic] return(RedirectControl("ControlDownSingle", pByObject)); }
public func ControlLeftReleased(pByObject) { return(RedirectControl("ControlLeftReleased", pByObject)); }
public func ControlRightReleased(pByObject) { return(RedirectControl("ControlRightReleased", pByObject)); }
public func ControlUpdate(pByObject, int comdir, bool dig, bool throw) { return(RedirectControl("ControlUpdate", pByObject, comdir, dig, throw)); }

public func ControlDownDouble(pByObject)
{
  // Reiter: absitzen
  if (pByObject == GetRider())
  {
    ObjectSetAction(pByObject, "Walk");
    return(1);
  }
  // Keine Steuerung
  return(0);
}

protected func ContainedUp(pByObject)
{
  // Nur ein Steuermann zur Zeit
  if (GetRider()) return(0);
  // Reiter aufsitzen
  if (!ObjectSetAction(pByObject,"Ride",this()))
  {
    NoGoodRider(pByObject);
    return(0);
  }
  return(1); 
}

public func ControlCommand(szCommand, pTarget, iTx, iTy)
{
  // Steuerung an ziehende Pferde weiterleiten
  var pHorse;
  while (pHorse = FindPullingHorse(pHorse))
    pHorse->~ControlCommand(szCommand, pTarget, iTx, iTy);
  return(1);
}

/* Bewegung */

public func TurnRight()
{                   
  // Wenden aus Fahren
  if (GetDir() == DIR_Left())
    if (GetAction() eq "Drive0" || GetAction() eq "Drive1" || GetAction() eq "Drive2")
      SetAction("Turn");
  // Richtung
  SetDir(DIR_Right());
  SetComDir(COMD_Right());
  return(1);
}

public func TurnLeft()
{
  // Wenden aus Fahren
  if (GetDir() == DIR_Right())
    if (GetAction() eq "Drive0" || GetAction() eq "Drive1" || GetAction() eq "Drive2")
      SetAction("Turn");
  // Richtung
  SetDir(DIR_Left());
  SetComDir(COMD_Left());
  return(1);
}

/* Aktionen */

private func Driving()
{
  // Aktion der Geschwindigkeit anpassen
  var iDriving = BoundBy(Abs(GetXDir()),0,1);
  if (Abs(GetXDir()) > 20) iDriving = 2;
  if (GetAction() ne Format("Drive%d",iDriving))
    SetAction(Format("Drive%d", iDriving));  
  // Kutschbockposition anpassen
  AdjustSeatVertex();
  return(1);
}

private func Turning()
{
  // Kutschbockposition anpassen
  AdjustSeatVertex();
}


/* Konfiguration */

private func AdjustSeatVertex()
{
  // Während der Fahrt
  var iPos = -24;
  // Während der Wende
  if (GetAction() eq "Turn") iPos = 20 - 2 * GetPhase();
  // Entsprechend der Richtung
  if (GetDir() == DIR_Right()) iPos = -iPos;
  // Kutschbock-Vertex anpassen
  SetVertex(0,0,iPos);
  return(1);
}

/* Status */

public func IsHorseWagon() { return(1); }

public func GetRider()
{
  var pRider;
  if (pRider = FindObject(0,0,0,0,0,0,"RideStill",this())) return(pRider);
  return(FindObject(0,0,0,0,0,0,"Ride",this()));
}
  
private func NoGoodRider(pRider)
{
  if (GetOCF(pRider) & OCF_CrewMember())
    Message("%s kann keinen|Wagen steuern.",pRider,GetName(pRider));
  return(0);
}

private func FindPullingHorse(pHorse)
{
  while (pHorse = FindObject(0, -100,-100,200,200, OCF_Grab(), 0,0, NoContainer(), pHorse))
    if (GetAction(pHorse) eq "Pull" || GetAction(pHorse) eq "Pull2" || GetAction(pHorse) eq "Pull3" || GetAction(pHorse) eq "Turn")
      if (GetActionTarget(0,pHorse) == this())
        return(pHorse); 
  return(0);
}
  
public func IsStill() { return(GetAction() eq "Drive0"); }
  
/* Verbindung */

public func Connect(pHorse)
{
  CreateObject(CHBM,0,0,GetOwner())->CHBM::Connect(pHorse,this());
  Sound("Connect");
  return(1);
}
  
public func Disconnect(pFromHorse)
{         
  var pBeam = FindObject(CHBM,0,0,0,0,0,"Connect",pFromHorse);
  if (pBeam) RemoveObject(pBeam);
  Sound("Connect");
  return(1);
}
  
public func ControlLeft(pByObject) { [$TxtLeft$] return(inherited(pByObject)); }
public func ControlRight(pByObject) { [$TxtRight$] return(inherited(pByObject)); }
public func ControlDownSingle(pByObject) { [$TxtStop$|Method=Classic] return(inherited(pByObject)); }
public func ControlDownDouble(pByObject) { [$TxtGetoff$] return(inherited(pByObject)); }

public func ContainedUp(pByObject) { [$TxtGeton$] return(inherited(pByObject)); }
