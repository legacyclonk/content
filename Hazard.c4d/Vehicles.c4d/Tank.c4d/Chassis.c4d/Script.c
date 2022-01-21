/*-- Chassis --*/

#strict

public func AttachTo(attachto) {	// attach to tank
	SetDir(DIR_Left);
	SetAction("Attach",attachto);
}

public func Remove() {			// remove
	if(GetAction() eq "Idle")
		RemoveObject();
}

public func Turn() {
	SetAction("Turn",GetActionTarget());
}

protected func ChangeDir() {
	var a = GetAction(GetActionTarget());
	if(a eq "Turn" || a eq "TurnStalled") return(SetAction("Turn"));

	SetAction("Attach",GetActionTarget());
	if(GetDir() == DIR_Left) SetDir(DIR_Right);
	else  SetDir(DIR_Left);
}

/* Engine effect */

protected func Engine() {
	SetR(GetR(GetActionTarget()));

	// set matrix values
	if(GetActionTarget()->Contents()	&& !(GetActionTarget()->EMPShocked()))
		SetObjDrawTransform (1000, 0, 0, 0, 1000, Sin(GetActTime()*50,300));
}
