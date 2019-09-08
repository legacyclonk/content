/*-- Geisterkrähe --*/

#strict

local pMaster, iTime;

/* Initialisierung */

protected func Initialize()
{
  SetAction("Fly");
  SetClrModulation(RGBa(255,255,255,128));
  SetObjectBlitMode(1);
  if (Random(2)) return(SetComDir(COMD_Right()));
  return(SetComDir(COMD_Left()));
}

public func SetMaster(pObj) { pMaster = pObj; Activity(); }

/* TimerCall mit KI-Steuerung */

protected func Activity()
{
  // Verschwinden
  iTime++;
  if(iTime >= 200) return(Kill());

  // Geräusche machen
  if (!Random(25)) Sound("Raven*");

  // Die KI-Steuerung wird beim Reiten nicht gebraucht
  if (GetRider()) return();

  if(!GetCommand() && pMaster) SetCommand(this(), "Follow", pMaster);

  // Nicht rückwärts fliegen
  if(GetXDir() < 0 && GetDir() == DIR_Right())
    TurnLeft();
  if(GetXDir() > 0 && GetDir() == DIR_Left())
    TurnRight();

  return(1);
}

/* Aktionen */

public func TurnRight()
{
  if (Stuck() || GetAction() ne "Fly") return();
  if (GetXDir() < 0) SetXDir(0);
  SetDir(DIR_Right());
  SetComDir(COMD_Right()); 
  return(1);
}

public func TurnLeft()
{
  if (Stuck() || GetAction() ne "Fly") return();
  if (GetXDir() > 0) SetXDir(0);
  SetDir(DIR_Left());
  SetComDir(COMD_Left()); 
  return(1);
}

/* Einwirkungen */

protected func CatchBlow(iLevel, pObject)
{
  if (pObject->~IsArrow()) SetAction("Tumble");
  if (!Random(3)) Sound("RavenHurt");
  return(1);
}

protected func Death()
{
  CastParticles("PSpark", 10, 20, 0, 0, 50, 70, RGBa(255, 255, 100), RGBa(100, 100, 0));
  Sound("Poff");
  DropRider();
  RemoveObject();
  return(1);
}

/* Steuerung durch Besessenheit */

protected func ControlCommand(szCommand, pTarget, iTx, iTy)
{
  // Bewegungskommando
  if (szCommand eq "MoveTo")
   return(SetCommand(this(),szCommand, pTarget, iTx, iTy));
  return(0);
}

protected func ControlLeft()
{
  [$TxtMovement$]
  SetCommand(this(), "None");
  TurnLeft();
  return(1);
}

protected func ControlRight()
{
  [$TxtMovement$]
  SetCommand(this(), "None");
  TurnRight();
  return(1);
}

protected func ControlUp()
{
  [$TxtMovement$]
  SetCommand(this(), "None");
  SetComDir(COMD_Up());
  return(1);
}

protected func ControlDown()
{
  [$TxtMovement$]
  SetCommand(this(), "None");
  SetComDir(COMD_Down());
  return(1);
}

protected func ControlDownDouble()
{
  [$TxtGetOff$]
  SetCommand(this(), "None");  
  var pRider;
  if(pRider = GetRider())
    ObjectSetAction(pRider, "Walk");
  return(1);
}

protected func ControlDigDouble()
{
  [$TxtKill$]
  Kill();
  return(1);
}

/* Reiten */

protected func ActivateEntrance(pRider)
{
  // Nur ein Reiter zur Zeit
  if (GetRider()) return(0);
  // Nur im Stehen, Gehen, Schwimmen, Ziehen
  if (GetAction() ne "Fly") 
    return(0);
  // Aufsitzen
  if (!ObjectSetAction(pRider,"Ride",this()))
  {
    //NoGoodRider(pRider);
    return(0);
  }
  SetObjectOrder(this(), pRider);
  // Clonkposition anpassen
  SetPosition(GetX(), GetY()-5, pRider);
  // Erfolg
  return(1);
}

public func DropRider()
{
  return(Fling(GetRider(), 0, 1));
}

public func GetRider()
{
  var pRider;
  while(pRider = FindObject(0, 0,0,0,0, OCF_Alive(), 0, this(), 0, pRider))
    if(WildcardMatch(GetAction(pRider), "*Ride*"))
      return(pRider);
}

private func UpdateRideVertex()
{ 
  var strAction = GetAction();
  var iPhase = GetPhase();
  var iDir = GetDir() * 2 - 1;

  if (strAction S= "Fly")
  {
    iPhase-=5;
    if(iPhase<0) iPhase+=20;
    if(iPhase <= 9) SetVertex(0, 1, -4-8*iPhase/10);
    else { iPhase-= 9; SetVertex(0, 1, -12+8*iPhase/10); }
  }
  if (strAction S= "Turn")
  {
    if(iPhase <= 7) SetVertex(0, 1, -8+5*iPhase/8);
    else if(iPhase <= 15) { iPhase-=8; SetVertex(0, 1, -3-11*iPhase/8); }
    else { iPhase-=17; SetVertex(0, 1, -14+6*iPhase/4); }
  }
}
