/*--- Dampfmaschine ---*/

#strict

local iWait; // Wartezeit (an einem Signal)

/* Seil beim Ziehen? */

private func DrawBeam() { return(0); }

/* Initialisierung */

protected func Initialize()
{
  SetAction("Walk");
  SetComDir(COMD_Stop());
}

/* Effekte */

protected func Puff()
{
  // Geräusch nur wenn wir aktiv sind
  if (GetAction() S= "Push")
  {
    Sound("Chuff");
    Smoke(-5, -14, 5 + Random(4));
  }
}

/* Signalsteuerung */

protected func CheckSignal()
{
  // Noch in Wartezeit
  if (iWait) return(0);
  // Signal suchen
  var pSignal = FindObject(SGNL, 0, +1);
  if (!pSignal) return(0);
  // Signal ist abgeschaltet
  if (!GetPhase(pSignal)) return(0);
  // Signal ans Signal ;)
  pSignal->SGNL::TrainArrive(this());
  // Wartezeit
  iWait = 9; 
  // Anhalten
  SetComDir(COMD_Stop());
  return(1);
}

protected func SignalDelay()
{
  // Keine Wartezeit
  if (!iWait) return(0);
  // Wartezeit aufzählen
  --iWait;
  // Abfahrtszeit vorm Zählerende
  if (iWait == 2) DepartFromSignal();
  return(1);
}

private func DepartFromSignal()
{
  // Signal suchen
  var pSignal = FindObject(SGNL, 0, +1);
  if (!pSignal) return(1);
  // Signal benachrichtigen
  pSignal->SGNL::TrainDepart(this());
  // Signal deaktiviert: Stehen bleiben
  var iPhase = GetPhase(pSignal);
  if (!iPhase) return(1);
  // Geschwindigkeit anpassen
  AdjustTrainSpeed();
  // Signal rechts
  if (!(--iPhase)) return(SetComDir(COMD_Right()));
  // Signal links
  if (!(--iPhase)) return(SetComDir(COMD_Left()));
  // Signal Weiterfahrt: Nach GetDir
  if (GetDir() == DIR_Left()) 
    return(SetComDir(COMD_Left()));
  else
    return(SetComDir(COMD_Right()));
}

/* Ereignisse */

protected func GrabLost(obj) 
{
  // Geräusch abspielen, wenn wir angekoppelt waren
  var do_sound;
  if (!obj) do_sound = 1;
  if (GetType(obj) == C4V_C4Object())
    if (GetCategory(obj) & C4D_Vehicle())
      do_sound = 1;
  if (do_sound) Sound("LineBreak");
  // Anhalten
  SetComDir(COMD_Stop());
  return(1);
}

private func GrabTarget() 
{
  // Nächstes greifbares Objekt suchen
  var obj = FindObject(0, -1, -1, -1, -1, OCF_Grab(), 0, 0, NoContainer());
  // Außer Reichweite
  if (!obj || ObjectDistance(this(), obj) > 50) return(0);
  // Keine fixierten, nicht verschieb oder ziebare Objekte
  if(GetDefHorizontalFix(GetID(obj)) || GetDefGrab(GetID(obj))==2 || obj->~NoPull()) return 0;
  // Ankuppeln
  SetAction("Push", obj);
  Sound("Connect");
  // Verbindungslinie zeigen
  if (DrawBeam()) CreateObject(STBM, 0, 0, GetOwner())->~Connect(obj, this());
  return(1);
}

private func ReleaseTarget()
{
  var pLine;
  if (DrawBeam()) if(pLine = FindObject(STBM, 0, 0, 0, 0, 0, "Connect", GetActionTarget()))
    RemoveObject(pLine);
  if (DrawBeam()) if(pLine = FindObject(STBM, 0, 0, 0, 0, 0, "Connect", this()))
    RemoveObject(pLine);
  SetAction("Walk");
  Sound("LineBreak");
}

protected func Entrance(object pNewContainer)
  {
    // Im Gebäude abkoppeln (sonst hat man vor der Tür einen Wagon, der immer noch verbunden ist)
    ReleaseTarget();
  }

private func AdjustTrainSpeed()
{
  // Zug angefasst: langsam fahren
  if (FindObject(0, 0,0,0,0, OCF_Living(), "Push", this()) || (GetActionTarget() && FindObject(0, 0,0,0,0, OCF_Living(), "Push", GetActionTarget())))
    SetPhysical("Walk", 70000, 2);
  // Zug nicht angefasst: schnell fahren
  else
    SetPhysical("Walk", 100000, 2);
}
  
/* Signalsteuerung */

public func SignalCall(object pSignal, bool fLoad)
{
  // An Waggon weiterleiten
  if (GetAction() S= "Push")
    return(GetActionTarget()->~SignalCall(pSignal,fLoad));
}
  
public func SignalDepart(object pSignal)
{
  // An Waggon weiterleiten
  if (GetAction() S= "Push")
    return(GetActionTarget()->~SignalDepart(pSignal));
}

/* Steuerung */

public func SetDirection(int comdir)
{
  // Geschwindigkeit anpassen
  AdjustTrainSpeed();
  // Fahren
  Sound("Click");
  SetComDir(comdir);
}

public func ControlLeft(object pByObject)
{
  [$TxtLeft$|Image=STMS:1]
  // Zähler blockieren
  iWait = 2;
  // Nicht angekoppelt: nur schieben lassen
  if (GetAction() ne "Push") return(0);

  if(!GetPlrJumpAndRunControl(pByObject->GetController()))
    SetDirection(COMD_Left);

  return(1);
}

public func ControlRight(object pByObject)
{
  [$TxtRight$|Image=STMS:2]
  // Zähler blockieren
  iWait = 2;
  // Nicht angekoppelt: nur schieben lassen
  if (GetAction() ne "Push") return(0);

  if(!GetPlrJumpAndRunControl(pByObject->GetController()))
    SetDirection(COMD_Right);

  return(1);
}

public func ControlDownSingle(object pByObject)
{
  [$TxtStop$|Method=None]
  if(!GetPlrJumpAndRunControl(pByObject->GetController()))
  {
    // Anhalten
    SetDirection(COMD_Stop);
    // Zähler resetten
    iWait = 0;
    // Vorm Signal abgestellt?
    CheckSignal();
  }

  return(1);
}

/* JumpAndRun-Steuerung */

public func ControlUpdate(object pByObject, int comdir)
{
  if(comdir == COMD_UpLeft || comdir == COMD_Left || comdir == COMD_DownLeft)
    comdir = COMD_Left;
  else if(comdir == COMD_UpRight || comdir == COMD_Right || comdir == COMD_DownRight)
    comdir = COMD_Right;
  else
    comdir = COMD_Stop;

  SetDirection(comdir);

  if(comdir == COMD_Stop)
  {
    iWait = 0;
    CheckSignal();
  }
}

// Schadet nicht, wenn die auch fuer klassische Steuerung zugänglich sind
public func ControlLeftDouble(object clonk)
{
  [$TxtLeftDouble$|Image=STMS:3]
  // Fahrt links
  SetDirection(COMD_Left);

  // Loslassen
  if(clonk->GetAction() S= "Push")// && clonk->GetActionTarget() == this())
    AddCommand(clonk, "UnGrab");
}

public func ControlRightDouble(object clonk)
{
  [$TxtRightDouble$|Image=STMS:4]
  // Fahrt rechts
  SetDirection(COMD_Right);

  // Loslassen
  if(clonk->GetAction() S= "Push")// && clonk->GetActionTarget() == this())
    AddCommand(clonk, "UnGrab");
}

/* Attach/Detach */

public func ControlDigDouble()
{
  [$TxtAttachOrDetach$|Image=STMS:0]
  if (GetAction() S= "Push") return(ReleaseTarget());
  return(GrabTarget());
}
