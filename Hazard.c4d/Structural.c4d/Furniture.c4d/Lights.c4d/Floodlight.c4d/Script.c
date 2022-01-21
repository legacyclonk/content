/*-- Scheinwerfer --*/

#strict

local pLight;

protected func Initialize()
{
  pLight = CreateObject(FLHH, 0,0, -1);
	pLight -> Activate(this());
}

protected func ControlLeft()
{
  pLight->TurnLeft();
}

protected func ControlRight()
{
  pLight->TurnRight();
}

protected func ControlUp()
{
  pLight->Stop();
}

public func ControlDigDouble()
{
  pLight->Switch();
}

// Scriptaufruf

public func SetRotation(int iR, int iOwn)
{
  iR = BoundBy(iR, -90, 90);
  SetR(iOwn);

  pLight->SetR(iR+GetR());
  pLight->RotateLight();
}

// Kein Kran darf mich schnappen!
public func NoCrane() { return(1); }
