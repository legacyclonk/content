/*-- Nachrichtenroboter --*/

#strict

protected func Initialize()
{
  SetAction("Walk");
  SetComDir(COMD_Left());
}

protected func ContactLeft()
{
  SetComDir(COMD_Right());
}

protected func ContactRight()
{
  SetComDir(COMD_Left());
}

protected func Walking()
{
  CreateParticle("PSpark", RandomX(-2,2), -2, RandomX(-1,1), RandomX(1,3), 10+Random(15), RGB(255,50,50), this(), true);

  if(!Random(30)) Sound("Wheeng");

  if(Random(2))
    SetComDir(COMD_Left());
  else
    SetComDir(COMD_Right());
}

public func IsBulletTarget() { return(1); }

protected func Damage()
{
  Explode(10+Random(5));
}

public func IsMachine() { return(1); }
public func EMPShock() { DoDamage(1); }
public func CanBeLaserRecycled() { return(2); }

protected func Hit3() {  DoDamage(1); }
