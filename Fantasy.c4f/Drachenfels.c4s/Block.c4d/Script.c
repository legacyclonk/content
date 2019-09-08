/*-- Klotz --*/

#strict

local iXDir,iYDir;

static const _BLK_MoveSound = "Elevator",
             _BLK_HitSound  = "Discharge";

protected func Initialize()
  {
  SetAction("Block");
  }

public func ControlUp(object pController)
  {
  // 2do: sound
  if (iYDir>=0) Sound(_BLK_MoveSound);
  if (GetContact(0, -1, 3)) iXDir=0;
  iYDir = -1;
  return(UpdateComDir());
  }

public func ControlDown(object pController)
  {
  // 2do: sound
  if (iYDir<=0) Sound(_BLK_MoveSound);
  if (GetContact(0, -1, 3)) iXDir=0;
  iYDir = 1;
  return(UpdateComDir());
  }

public func ControlLeft(object pController)
  {
  // 2do: sound
  if (iXDir>=0) Sound(_BLK_MoveSound);
  if (GetContact(0, -1, 12)) iYDir=0;
  iXDir = -1;
  return(UpdateComDir());
  }

public func ControlRight(object pController)
  {
  // 2do: sound
  if (iXDir<=0) Sound(_BLK_MoveSound);
  if (GetContact(0, -1, 12)) iYDir=0;
  iXDir = 1;
  return(UpdateComDir());
  }

protected func ContactLeft() { if (iXDir>=0) return(); Sound(_BLK_HitSound); return(iXDir=0); }
protected func ContactRight() { if (iXDir<=0) return(); Sound(_BLK_HitSound); return(iXDir=0); }
//protected func ContactBottom() { if (iYDir<=0) return(); Sound(_BLK_HitSound); return(iYDir=0); }

protected func Hit()
  {
  Sound(_BLK_HitSound);
  // 2do: sound
  iXDir=iYDir=0; SetComDir();
  }

private func UpdateComDir() // nur für iXDir|iYDir != 0
  {
  // Puzzling, isn't it?
  return(SetComDir(iXDir*(iYDir-2)+!iXDir*(iYDir-1)*2+5));
  }
