/* Context Use drone functionality */

#strict 2

// Enables object to use drones (with context menu/double dig)

protected func ContextSupportDrone()
{
  [$CtxDrone$|Image=DRSU|Condition=GetSupportDrone]
	var pDrone = GetSupportDrone();
  if(pDrone)
	{
		CreateMenu(DRSU,0,0,0,0,0,1);
		AddMenuItem("$CtxDroneTurnOff$", "TurnOffDrone", DRSU);
		AddMenuItem("$CtxDroneGuard$", "DroneGuardHere", DRSU);
		AddMenuItem("$CtxDroneFollow$", "DroneFollowMe", DRSU);
	}
}

public func TurnOffDrone()
{
	var pDrone = GetSupportDrone();
  if(pDrone) pDrone->TurnOff();
}

public func DroneGuardHere()
{
	var pDrone = GetSupportDrone();
  if(pDrone)
	{
		pDrone->SetGuardPosition(GetX(),GetY());
		pDrone->FollowClonk(false);
	}
}

public func DroneFollowMe()
{
	var pDrone = GetSupportDrone();
  if(pDrone) pDrone->FollowClonk(true);
}


public func GetSupportDrone()
{
  for( pDrone in FindObjects(Find_Func("IsDrone"), Find_Action("Fly"), Find_Owner(GetOwner())))
  {
		if(pDrone->GetGuardedClonk() == this)
			return pDrone;
	}
}

// Kann Ausrüstung benutzen
public func CanUse(id idObj)
{
  // Das Objekt kann natürlich Ausnahmen vornehmen....
  if(DefinitionCall(idObj, "IsDrone")) return true;
  return _inherited(idObj);
}