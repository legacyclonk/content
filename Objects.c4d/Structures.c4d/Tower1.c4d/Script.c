/*-- Kleiner Turm --*/

#strict

#include DOOR

/* Tür */

private func SoundDoorOpen()
{
  Sound("DoorOpen");
}

private func SoundDoorClose()
{
  Sound("DoorClose");
}

/* Aufstieg */

protected func CheckContents()
{
  while (Contents()) Exit(Contents(), 0,-10,0, 0);  
}

/* Initialisierung */

protected func Initialize()
{
  UpdateTransferZone();
}

/* Transfer */

protected func UpdateTransferZone()
{
  SetTransferZone(-19,-28,38,48);
}

protected func ControlTransfer(object pObj, int tx, int ty)
{
  // Aufstieg zur Plattform
  if(Inside(tx - GetX(), -20, 20) && Inside(ty - GetY(), -38, -12) )
    if(GetY(pObj) > GetY() - 12)
      return(HandleTransferAscend(pObj, tx, ty) );

  // Abstieg von der Plattform
  if(Inside(GetX(pObj) - GetX(), -20, 20) && Inside(GetY(pObj) - GetY(), -38, -12) )
    if(Inside(tx - GetX(), -20, 20) && Inside(ty - GetY(), -12, 20) )
      return(HandleTransferDescend(pObj, tx, ty) );

  // Transfer nicht möglich
  return(0);
}

private func HandleTransferAscend(object pObj, int tx, int ty)
{
  if(Inside(GetX(pObj) - GetX(), -20, 20) && Inside(GetY(pObj) - GetY(), -38, -12) )
    return(0);
    
  if(Contained(pObj) == this() )
    return(1);

  // Clonk soll den Turm betreten
  return (AddCommand(pObj,"Enter",this()));
}
  
private func HandleTransferDescend(object pObj, int tx, int ty)
{
  if(Contained(pObj) == this() )
    return(1);
  Enter(this(), pObj);

  return(AddCommand(pObj,"Exit"));
}

// Loren nicht im Turm ausleeren
public func NoLorryEjection(object lorry) { return true; }
