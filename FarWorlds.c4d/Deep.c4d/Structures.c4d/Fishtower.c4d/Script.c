/*-- Fischturm --*/

#strict
#include DOOR

local pDoor;

protected func SoundOpenDoor()  { Sound("Airlock1"); }
protected func SoundCloseDoor() { Sound("Airlock2"); }

protected func Bubbling()
{
  if(GetOCF()&OCF_Fullcon())
    if(!Random(5))
      Bubble(-20,-30);

  // Haie vertreiben  
  var shark;
  while(shark = FindObject(SHRK, -250, -250, 500, 500, OCF_Alive(), 0, 0, 0, shark) )
  {
    var angl = Angle(GetX(), GetY(), GetX(shark), GetY(shark) );
    var dist = 350;
    SetCommand(shark, "MoveTo", 0, GetX() + Sin(angl, dist), GetY() - Cos(angl, dist) );
  }
}

protected func Completion() {
  pDoor=CreateObject(ATDR,0,0,GetOwner());
  pDoor->SetAction("Attach",this());
  // Transferzone setzen
  UpdateTransferZone();
}

protected func ContainedUp(pCaller) {
  Enter(pDoor, pCaller);
  AddCommand(pCaller, "Exit");
  return(1);
  }

func ControlCommand(szCommand, pTarget, iTX, iTY) {
  // Message("ich werde ausgelöst");
  // oben raus
  if(szCommand eq "MoveTo")
    if(Distance(iTX,iTY,GetX(),GetY()+84) > Distance(iTX,iTY,GetX(pDoor),GetY(pDoor)))
      ContainedUp(GetCursor(GetController()));
}

/* Wenn der Turm mal runterfällt */

protected func Hit() { UpdateTransferZone(); }

/* Transfer */

protected func UpdateTransferZone() { SetTransferZone(-10,-55,19,150); }

protected func ControlTransfer(object pObj, int iTx, int iTy)
{
  // Aufstieg zur Plattform
  if(Inside(GetX(pObj)-GetX(),-23,+23) && (Contained(pObj) || GetY(pObj)>GetY()-19)
  && Inside(iTx-GetX(),-23,+23)        && Inside(iTy-GetY(),-60,-20))
      return(HandleTransferPlatform(pObj,iTx,iTy));

  // Abstieg von der Plattform
  if(Inside(GetX(pObj)-GetX(),-23,+23) && Inside(GetY(pObj)-GetY(),-60,-20)
  && Inside(iTx-GetX(),-23,+23)        && Inside(iTy-GetY(),-19,+94))
      return(HandleTransferDescend(pObj,iTx,iTy));

  return();
}

private func HandleTransferPlatform(object pObj, int iTx, int iTy)
{
  // Clonk ist im Turm: aufsteigen
  if(Contained(pObj) == this()) return(0,ContainedUp(pObj));

  // Clonk ist in der Tür: warten
  if(Contained(pObj)==pDoor) return(1);

  // Clonk soll den Turm betreten
  return(AddCommand(pObj,"Enter",this()));
}
  
private func HandleTransferDescend(object pObj, int iTx, int iTy)
{
  // Clonk ist in der Tür: warten
  if(Contained(pObj)==pDoor) return(1);

  // Clonk ist im Turm: rausgehen
  if(Contained(pObj)==this()) return(0, AddCommand(pObj,"Exit"));

  // Clonk soll in die Tür gehen
  return(AddCommand(pObj,"Enter",pDoor));
}

/* Callbacks */

public func IsDeepSeaResearch() { return(1); }

public func IsFishTower() { return true; }
