/* Rakete */

#strict 2
#include MISS

local turn;

public func Acceleration() { return 2; }
public func MaxTime() { return 180; }
public func MaxSpeed() { return 75; }

public func TurnSpeed() { return 15; }

/* Timer */

private func Traveling()
{
  // kommt nich weiter
  if(GetActTime() >= MaxTime())
  {
    Hit();
    return;
  }

  // Effekte
  Smoking();

  // beschleunigen
  Accelerate();
  
  // r drag
  if(!(GetActTime()%3))
    SetRDir(GetRDir()*14/15);
}

func ControlLeft()
{
  SetRDir(-TurnSpeed());
}

func ControlRight()
{
  SetRDir(TurnSpeed());
}

func ControlDown()
{
  SetRDir(0);
}

func ControlUp()
{
  SetRDir(0);
}

func ControlThrow()
{
  Hit();
}

func ControlDig()
{
  ReleaseControl();
}

public func ReleaseControl()
{
  SetPlrView(GetOwner(shooter),shooter);
  SetCursor(GetOwner(shooter),shooter,1,0);
}

public func ActivateControl(object user)
{
  shooter = user;
  if(shooter == GetCursor(GetOwner()))
  {
    SetPlrView(GetOwner(shooter),this);
    SetPlrViewRange(100,this);
    SetCursor(GetOwner(shooter),this,1,0);
  }
}

public func NoDecoDamage() { return true; }
