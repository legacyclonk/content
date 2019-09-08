/*-- Zielscheibe --*/

#strict

/* Initialisierung */

protected func Initialize()
{
  SetAction("Stand");
  return(1);
}

/* Kann von Pfeilen getroffen werden */

public func IsArrowTarget() { return(GetCon() >= 100); } 

/* Steuerung */

protected func ControlLeft()
{
  SetDir(DIR_Left());
  return(0);
}

protected func ControlRight()
{
  SetDir(DIR_Right());
  return(0);
}  
