/*-- Wachturm Anfassobjekt --*/

#strict

protected func ControlLeft()
{
  [$TxtLeftgate$]
  ProtectedCall(GetActionTarget(), "ControlLeft");
}

protected func ControlRight()
{
  [$TxtRightgate$]
  ProtectedCall(GetActionTarget(), "ControlRight");
}

protected func AttachTargetLost() { return(RemoveObject()); }
