/*-- U-Boot --*/

#strict

local turn_end_dir;

/* Initialisierung */

protected func Completion()
{
  SetAction("Walk");
  SetComDir(COMD_Stop());
  turn_end_dir = -1;
}

/* Steuerung */

private func TurnEnd()
{
  if(turn_end_dir != -1)
  {
    SetDirection(turn_end_dir);
    turn_end_dir = -1;
  }
}

public func SetDirection(int comdir)
{
  ClearCommand();

  if(GetAction() S= "Turn" || GetAction() S= "Grapple")
  {
    // Set direction when Turn action has finished
    turn_end_dir = comdir;
    return();
  }

  if(GetAction() ne "Swim") return();

  SetComDir(comdir);

  if(comdir == COMD_Left || comdir == COMD_Right)
    SetYDir(0);
  if(comdir == COMD_Up || comdir == COMD_Down)
    SetXDir(0);

  if(ComDirLike(comdir, COMD_Right))
  {
    if(GetDir() == DIR_Left)
    {
      SetAction("Turn");
      SetDir(DIR_Right);
    }
  }

  if(ComDirLike(comdir, COMD_Left))
  {
    if(GetDir() == DIR_Right)
    {
      SetAction("Turn");
      SetDir(DIR_Left);
    }
  }
}

protected func ContainedUpdate(object clonk, int comdir, bool dig, bool throw)
{
  SetDirection(comdir);
}

protected func ContainedUp(object clonk)
{
  [$TxtComUp$|Image=SSY1:1]
  CloseAirlock();

  if(!GetPlrJumpAndRunControl(clonk->GetController()))
    SetDirection(ComDirTransform(GetComDir(), COMD_Up));

  return(1);
}

protected func ContainedDown(object clonk)
{
  [$TxtComDown$|Image=SSY1:0]
  CloseAirlock();

  if(!GetPlrJumpAndRunControl(clonk->GetController()))
    SetDirection(ComDirTransform(GetComDir(), COMD_Down));

  return(1);
}

protected func ContainedRight(object clonk)
{
  [$TxtComRight$|Image=SSY1:4]
  if(!GetPlrJumpAndRunControl(clonk->GetController()))
    SetDirection(ComDirTransform(GetComDir(), COMD_Right));

  return(1);
}

protected func ContainedLeft(object clonk)
{
  [$TxtComLeft$|Image=SSY1:3]
  if(!GetPlrJumpAndRunControl(clonk->GetController()))
    SetDirection(ComDirTransform(GetComDir(), COMD_Left));

  return(1);
}

protected func ContainedUpDouble(object pByObject)
{
  [$TxtExit$|Image=SSY1:5]
  OpenAirlock();
  Exit(pByObject);

  SetDirection(COMD_Stop);
}

protected func ContainedDig(object clonk)
{
  [$TxtFire$|Image=SSY1:2]
  var i, obj;
  for(var i = 0; obj = Contents(i); ++ i)
    if(obj->~IsTorpedo())
      break;

  if(obj) SetCommand(obj, "Exit");
  else Sound("Error");

  return(1);
}

protected func ControlCommand(string szCommand, object pTarget, int iX,int iY)
{
  // Bewegungskommando
  if (szCommand S= "MoveTo")
  {
    // Reset manual movement
    return(SetCommand(this(),szCommand,pTarget,iX,iY));
  }
}

/* Einsammeln & Eingang */

protected func RejectCollect(id idObject, object pObject)
{
  // Anderes Objekt nicht im Freien
  if (Contained(pObject)) return(0);
  // Torpedotreffer
  if (pObject->~IsTorpedo())
    return(TorpedoHit(pObject));
  // U-Boot sammelt an Land nichts ein
  if (GetAction() ne "Swim") return(1);
  // Objekt befindet sich nicht im Bereich des Greifers
  if (!Inside(GetX(pObject) - GetX() + 30 - GetDir() * 60, -8, +8)) return(1);
  if (!Inside(GetY(pObject) - GetY() - 14, -8, +8)) return(1);
  // Einsammeln okay
  return(0);
}

protected func Ejection(object obj)
{
  if (obj->~IsTorpedo())
    obj->Launch(this());
}

protected func Collection(object pObject, bool fPut)
{
  if (fPut) return(1);
  SetAction("Grapple");
  Sound("Grapple");
}

protected func ActivateEntrance(object pByObject)
{
  return(OpenAirlock());
}

/* Kontakt */

protected func ContactLeft()
{
  if(GetAction() ne "Swim") return(0);

  if (ComDirLike(GetComDir(), COMD_Left))
    SetDirection(COMD_Stop);
}
  
protected func ContactRight()
{
  if(GetAction() ne "Swim") return(0);

  if (ComDirLike(GetComDir(), COMD_Right))
    SetDirection(COMD_Stop);
}

/* Funktionen */

private func ClearCommand()
{
  SetCommand(this(), "None");
}

private func CheckCommandAction()
{
  // Wenden nach rechts
  if(GetCommand())
    if(ComDirLike(GetComDir(), COMD_Right))
      if(GetDir()==DIR_Left)
      {
        SetAction("Turn");
        SetDir(DIR_Right);
      }

  // Wenden nach links
  if (GetCommand())
    if (ComDirLike(GetComDir(), COMD_Left))
      if (GetDir()==DIR_Right())
      {
        SetAction("Turn");
        SetDir(DIR_Left);
      }
}

private func SwimProcess()
{
  Bubbling();
  CheckCommandAction();
}

private func Bubbling()
{
  Bubble(0, Random(4));
  Bubble(0, -5);
  Bubble(0, 5);
}

private func TorpedoHit(object pTorpedo)
{
  if(GetAction(pTorpedo) ne "Swim") return(1);
  if(GetActTime(pTorpedo) < 25) return(1);
  pTorpedo->~Hit();
  return(1);
}

private func OpenAirlock()
{
  if (GetEntrance()) return(0);
  SetEntrance(1);
  Sound("Airlock1");
  return(1);
}

private func CloseAirlock()
{
  if(!GetEntrance()) return(0);
  SetEntrance(0);
  Sound("Airlock2");
  return(1);
}

private func Damage()
{
  var i=0;
  if (GetDamage()<150) return(0);
  ChangeDef(SUBB);
  SetAction("Wrecked");
  DoCon(-5);
  while(i++<14) Bubble(Random(30),Random(20));
}

/* Forschung */

public func IsDeepSeaResearch() { return(1); }
