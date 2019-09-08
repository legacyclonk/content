/*-- Tür --*/

#strict
#include DOOR

protected func SoundOpenDoor()  { Sound("Airlock1"); }
protected func CloseDoor() { Sound("Airlock2"); }

func ActivateEntrance(pObj)
{
  if (GetAction() eq "Attach") SetAction("OpenDoor");
  return(1);
}

func Collection2(object pObj)
{
  // Objekt kommt nicht grad von anderswo
  if(GetAction() ne "Attach")
    Enter(GetActionTarget(), pObj);
}