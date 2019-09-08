/*-- Eiskrähe --*/

#strict

local pMaster, iTime;

/* Initialisierung */

protected func Initialize()
{
  SetAction("Fly");
  if (Random(2)) return(SetComDir(COMD_Right()));
  return(SetComDir(COMD_Left()));
}

public func SetMaser(pObj) { pMaster = pObj; iTime = 1; Activity(); }

/* TimerCall mit KI-Steuerung */

protected func Activity()
{
  // Verschwinden
  if(iTime) if(iTime++ >= 20) return(Kill());
  
  // Geräusche machen
  if (!Random(25)) Sound("Raven*");

  // Die KI-Steuerung wird beim Reiten nicht gebraucht
  if (GetRider()) return();

  if(!GetCommand() && pMaster) SetCommand(this(), "Follow", pMaster);
  
  if(pMaster) return();

  // Nichts machen
  if (Random(2) || GetAction() ne "Fly") return(1);

  // Nach oben oder unten fliegen
  SetComDir(COMD_Up());
  if (Random(2)) SetComDir(COMD_Down());

  // Nichts machen
  if (!Random(4)) return(1);

  // Umdrehen
  if (Random(2)) return(TurnRight());
  return(TurnLeft());
}

protected func HitCheck()
{
  return();
	
  // Die KI-Steuerung wird bei Besessenheit nicht gebraucht
  if (GetRider()) return();   if(pMaster) return();

  // Einen Ballon oder ein Luftschiff zum attackieren suchen
  return(Attack());
}

public func Attack(object pAttack)
{
  // Nur während des Fliegens kann gebissen werden
  if (GetAction() ne "Fly") return();

  // Einen Ballon oder ein Luftschiff suchen
//  if (!pAttack) pAttack = FindObject(BALN, -20,0,40,30, 0, 0,0, NoContainer());
//  if (!pAttack) pAttack = FindObject(BLMP, -30,0,60,40, 0, 0,0, NoContainer());
//  if (!pAttack) return();

  // Reinbeißen!
//  pAttack->~Poke();
  SetAction("Attack");
  Sound("Raven*");
}

protected func Survive()
{
  if(pMaster) return();
	
  // Die KI-Steuerung wird bei Besessenheit nicht gebraucht
  if (GetRider()) return();

  // Nicht in Flüssigkeiten oder brennende Objekte fliegen
  if (InLiquid() || GBackLiquid(GetXDir()*2, GetYDir()*2) || GBackLiquid(GetXDir()*3, GetYDir()*3) || FindObject(0, -20 + GetXDir() * 2, -20 + GetYDir() * 2, 40, 40, OCF_OnFire()))
   SetComDir(COMD_Up());
}

/* Kontakte */

protected func ContactLeft()
{
  if(pMaster) return();
  
  // Die KI-Steuerung wird bei Besessenheit nicht gebraucht
  if (GetRider()) return();

  return(TurnRight());
}

protected func ContactRight()
{
  if(pMaster) return();
  
  // Die KI-Steuerung wird bei Besessenheit nicht gebraucht
  if (GetRider()) return();

  return(TurnLeft());
}

protected func ContactTop()
{
  if(pMaster) return();
  
  // Die KI-Steuerung wird bei Besessenheit nicht gebraucht
  if (GetRider()) return();

  return(SetComDir(COMD_Down()));
}

protected func ContactBottom()
{
  if(pMaster) return();

  // Die KI-Steuerung wird bei Besessenheit nicht gebraucht
  if (GetRider()) return();

  return(SetComDir(COMD_Up()));
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

public func ControlCommand(szCommand, pTarget, iTx, iTy)
{
 // Bewegungskommando
 if (szCommand eq "MoveTo")
  return(SetCommand(this(),szCommand, pTarget, iTx, iTy));
 return(0);
}

public func ControlLeft()
{
 [$TxtMovement$]
 SetCommand(this(), "None");
 TurnLeft();
 return(1);
}

public func ControlRight()
{
 [$TxtMovement$]
 SetCommand(this(), "None");
 TurnRight();
 return(1);
}

public func ControlUp()
{
 [$TxtMovement$]
 SetCommand(this(), "None");
 SetComDir(COMD_Up());
 return(1);
}

public func ControlDown()
{
 [$TxtMovement$]
 SetCommand(this(), "None");
 SetComDir(COMD_Down());
 return(1);
}

public func ControlDig()
{
 [$TxtAttack$]
 Attack();
 return(1);
}

/* Reiten */

public func ActivateEntrance(pRider)
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
  // Could we perhaps just use FindObject(0, 0,0,0,0, 0, 0,this()) with no Action and just the ActionTarget specified...?
  if (pRider = FindObject(0, 0,0,0,0, 0, "RideStill",this())) return(pRider);
  if (pRider = FindObject(0, 0,0,0,0, 0, "RideLoadBow",this())) return(pRider);
  if (pRider = FindObject(0, 0,0,0,0, 0, "RideAimBow",this())) return(pRider);
  if (pRider = FindObject(0, 0,0,0,0, 0, "RideMagic",this())) return(pRider);
  return(FindObject(0, 0,0,0,0, 0, "Ride",this()));
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

public func ContainedDigDouble()
{
 [$TxtLeave$]
 RemoveEffect("PossessionSpell", this());
 return(1);
}

public func LightningStrike()
{
  Death();	
}
