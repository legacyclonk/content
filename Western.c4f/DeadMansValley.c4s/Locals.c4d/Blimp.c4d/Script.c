/*-- Luftschiff --*/

#strict

protected func Initialize()
{
  SetGraphics(0, this(), BLMP);
  SetSolidMask(16,57,36,3,16,57);
}

/* Steuerung */

protected func ControlUp()      // hoch
{
  [$TxtAscend$]
  ClearCommand();
  if (GetAction()S="WaterLand") SetAction("WaterLaunch");
  SetComDir(COMD_Up());
}

protected func ControlDownSingle()  // runter
{
  [$TxtDescend$]
  ClearCommand();
  SetComDir(COMD_Down());
}

protected func ControlRight()       // rechts
{
  [$TxtTurnright$]
  ClearCommand();
  if (GetAction()S="DropOff") return(0);
  if (GetAction()S="Turn") return(0);
  SetAction("FloatPropel");
  if (GetDir()!=DIR_Right()) SetAction("Turn");
  SetComDir(COMD_Right()); 
  SetDir(DIR_Right());
}

protected func ControlLeft()        // links
{
  [$TxtTurnleft$]
  ClearCommand();
  if (GetAction()S="DropOff") return(0);
  if (GetAction()S="Turn") return(0);
  SetAction("FloatPropel");
  if (GetDir()!=DIR_Left()) SetAction("Turn");
  SetComDir(COMD_Left());
  SetDir(DIR_Left());
}

protected func ControlCommand(string szCommand,object pTarget,int iX,int iY)
{
  // Pilot läßt los
  if (szCommand S= "UnGrab")
    return(0,ClearCommand());
  // Bewegungskommando vom Piloten
  if (szCommand S= "MoveTo")
    return(SetCommand(this(),szCommand,pTarget,iX,iY));
  return(0);
}

private func ClearCommand() { SetCommand(this(),"None"); }

/* Initialisierung */

protected func Initialize() { FloatIdleDown(); }

/* Aktivität */

private func FloatProcess()
{
  SetXDir( BoundBy( GetWind()/4, -10, +10 ) );
//  NoPilotCheck();
  CheckCommandAction();
  return(1);
}

private func FlightProcess()
{
//  NoPilotCheck();
  CheckCommandAction();
  return(1);
}

private func CheckCommandAction()
{
  // Propeller an für Kommando
  if (GetCommand()) 
    if (GetAction()S="FloatIdle") 
      SetAction("FloatPropel");
  // Wenden nach rechts
  if (GetCommand())
    if (Inside(GetComDir(),COMD_UpRight(),COMD_DownRight())) 
      if (GetDir()==DIR_Left())
        return(SetAction("Turn"), SetDir(DIR_Right()));
  // Wenden nach links
  if (GetCommand())
    if (Inside(GetComDir(),COMD_DownLeft(),COMD_UpLeft())) 
      if (GetDir()==DIR_Right())
        return(SetAction("Turn"), SetDir(DIR_Left()));
  return(0);  
}   

private func NoPilotCheck()
{
  if (!FindObject(0,0,0,0,0,0,"Push",this())) 
    return(FloatIdleDown());
  return(0);
}

private func FloatIdleDown()
{
  if (GetAction()ne"FloatIdle")
    SetAction("FloatIdle");
  SetComDir(COMD_Down());
  ClearCommand();
  return(1);
}

/* Trefferkontrolle */

protected func RejectCollect(id idObject)
{
  // Pfeile vertragen sich nicht mit einem Ballon
  if (DefinitionCall(idObject,"IsArrow") )
    return(0);
  return(1);
}

protected func Collection(object pObj)
  {
  var idObj = GetID(pObj);
  // bei brennendem Pfeil anzünden
  if(OnFire(pObj)) Incinerate();
  // ansonsten abstürzen
  else SetAction("DropOff");
  // in jedem Fall im Pfeil Hit auslösen (Sprengpfeil explodiert zB)
  ProtectedCall(pObj, "Hit");
  return(1);
  }


/* Angriff durch Vögel */

public func Poke()
{
  Sound("Rip");
  SetAction("DropOff");
  return(1);
}
  
/* Forschung */

public func GetResearchBase() { return(BALN); }
