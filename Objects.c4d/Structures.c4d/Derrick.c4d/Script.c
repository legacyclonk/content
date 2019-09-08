/*--- Bohrturm ---*/

#strict

#include CXEC

#include DRBS

/* Lokale Variablen */

local pBarrel;
local pDrillHead;

/* Initialisierung */

protected func Initialize()
{
  // Internes Fass als Zwischenlager erzeugen
  pBarrel = CreateContents(BARL);
  return(1);
}

public func LineConnectType()
{
  // Schon ein Abflussrohr da? Dann machen wir mit Energie weiter.
  if (FindObject(DPIP, 0,0,0,0, 0, "Connect", this()) )
    return(PWRL);
  // Sonst Abfluss.
  return(DPIP);
}

private func PipeHeadCheck()
{
  // Bei Bedarf Bohrkopf und Leitung erzeugen
  if (!pDrillHead) 
  {              
    // Bohrkopf
    pDrillHead = CreateObject(PIPH, 0, 36, GetOwner());
    SetAction("Pump", pDrillHead);
    ObjectSetAction(pDrillHead, "Stop", this());
    CreateObject(DPLI, 0, 0, -1)->DPLI::Init(this(), pDrillHead);
  }
  return(pDrillHead);
}

/* Status */

public func AcceptTransfer()
{
  return (GetAction() S= "Pump");
}

/* Steuerung */

private func StartDrillHead(object pCaller)
{
  PipeHeadCheck();
  Sound("Click");
  if (!EnergyCheck(1000)) return(0);
  SetComDir(COMD_Down(), pDrillHead);
  ObjectSetAction(pDrillHead, "Drill");
  SetPlrView(pCaller->GetController(), pDrillHead);
  DigFreeRect(GetX(pDrillHead)-1, GetY() + GetObjHeight() / 2, 3, GetY(pDrillHead) - GetY() - GetObjHeight() / 2);
}

protected func ControlDownSingle(pCaller)
{
  [$CtrlDrillDesc$|Image=PIPH|Method=Classic]
  if(!GetPlrJumpAndRunControl(pCaller->GetController()))
    StartDrillHead(pCaller);
}

protected func ControlDown(pCaller)
{
  [$CtrlDrillDesc$|Image=PIPH|Method=JumpAndRun]
  SetPlrView(pCaller->GetController(), pDrillHead);
  if(GetPlrJumpAndRunControl(pCaller->GetController()))
    StartDrillHead(pCaller);
}

protected func ControlUp(pCaller)
{
  [$CtrlRetractDesc$|Image=PIPH]
  PipeHeadCheck();
  Sound("Click");
  if (!EnergyCheck(1000)) return(0);
  SetComDir(COMD_Up(), pDrillHead);
  ObjectSetAction(pDrillHead, "Move");
  SetPlrView(pCaller->GetController(),pDrillHead);
}

private func StopDrillHead(object pCaller)
{
  Sound("Click");
  ObjectSetAction(pDrillHead, "Stop");
  SetComDir(COMD_Stop(), pDrillHead);
  SetYDir(0, pDrillHead);
  SetPlrView(pCaller->GetController(), pDrillHead);
}

protected func ControlLeft(pCaller)
{
  [$CtrlStopDesc$|Image=PIPH|Method=Classic]
  if(!GetPlrJumpAndRunControl(pCaller->GetController()))
    StopDrillHead(pCaller);
  else
  {
    AddCommand(pCaller, "Call", this(), 0,0,0,0, "ContMoveLeft");
    AddCommand(pCaller, "UnGrab");
  }
}

protected func ControlRight(pCaller)
{
  [$CtrlStopDesc$|Image=PIPH|Method=Classic]
  if(!GetPlrJumpAndRunControl(pCaller->GetController()))
    StopDrillHead(pCaller);
  else
  {
    AddCommand(pCaller, "Call", this(), 0,0,0,0, "ContMoveRight");
    AddCommand(pCaller, "UnGrab");
  }
}

public func ContMoveLeft(object caller) { caller->SetComDir(COMD_Left); }
public func ContMoveRight(object caller) { caller->SetComDir(COMD_Right); }

protected func ControlUpReleased(pCaller)
{
  return(StopDrillHead(pCaller));
}

protected func ControlDownReleased(pCaller)
{
  return(StopDrillHead(pCaller));
}

/* Pumpkontrolle */

protected func Waiting()
{
  if (ReadyToPump())
    SetAction("Pump", pDrillHead);
}

protected func Pumping()
{
  if (!ReadyToPump())
    SetAction("Wait", pDrillHead);
}

protected func ReadyToPump()
{
  // Abflussrohr suchen
  var pDrainPipe = FindObject(DPIP,0,0,0,0,0,"Connect",this());
  // Kein Abflussrohr da: pumpen
  if (!pDrainPipe) return(1);
  // Abflussrohr ist gar nicht oder an einen Leitungsbausatz angeschlossen: pumpen
  var pPumpTarget = GetActionTarget(1, pDrainPipe);
  if (!pPumpTarget || (GetID(pPumpTarget)==LNKT)) return(1);
  // Kapazität für gepumptes Material im Zielobjekt prüfen
  if (!CheckCapacity(pPumpTarget, 12, GetMaterial(0, GetY(pDrillHead) - GetY())))
    // Keine ausreichende Kapazität: nicht pumpen
    return(0);
  // Ansonsten: pumpen
  return(1);
}

private func CheckCapacity(pTarget, iAmount, iMaterial)
{
  // Teilweise gefüllte Fässer suchen
  var BarrelID = GetBarrelType(iMaterial);
  var objBarrel;
  if (BarrelID)
  {
    while (objBarrel = FindObject(BarrelID,0,0,0,0,0,0,0,pTarget,objBarrel))
      if (!objBarrel->BarrelIsFull())
        if ((iAmount -= objBarrel->BarrelMaxFill() - objBarrel->GetAmount()) <= 0)
          // Noch Kapazität vorhanden
          return(1);
  }
  // Leere Fässer suchen
  BarrelID = EmptyBarrelID();
  objBarrel = 0;
  if (BarrelID)
  {
    while (objBarrel = FindObject(BarrelID,0,0,0,0,0,0,0,pTarget,objBarrel))
      if ((iAmount -= objBarrel->BarrelMaxFill()) <= 0)
        // Noch Kapazität vorhanden
        return(1);
  }
  // Keine Kapazität vorhanden
  return(0);
}

/* Zerstörung */

protected func Incineration() { if (pBarrel) RemoveObject(pBarrel); pBarrel=0; if (pDrillHead) RemoveObject(pDrillHead); pDrillHead=0; }
protected func IncinerationEx() { if (pBarrel) RemoveObject(pBarrel); pBarrel=0; if (pDrillHead) RemoveObject(pDrillHead); pDrillHead=0; }
protected func Destruction() { if (pBarrel) RemoveObject(pBarrel); pBarrel=0; if (pDrillHead) RemoveObject(pDrillHead); pDrillHead=0; return(_inherited() ); }


/* Forschung */

public func GetResearchBase() { return(PUMP); }
