/*-- Scheinwerfer --*/

#strict
#include BLGH

local turn;

func Activate(object pFloodLight)
{
	SetAction("Attach", pFloodLight);
}

func CreateLight()
{
	pLight = AddLightCone(1000, RGBa(255,255,220,70), this());
	pLight->ChangeSizeXY(3000,6000);
	RotateLight();
}

protected func Turning()
{
	if(EMPShocked()) return;
	var anglediff = Normalize(GetR()-GetR(GetActionTarget()),-180);

	if(turn == -1 && anglediff < -90 || turn == +1 && anglediff > 90)
		return(SetAction("Attach",GetActionTarget()));

	SetR(GetR()+turn);
	RotateLight();
}

public func RotateLight() {
	pLight->ChangeR(GetR());
}

public func TurnLeft()
{
	if(EMPShocked()) return;
	turn = -1;
	SetAction("Turn");
}

public func TurnRight()
{
	if(EMPShocked()) return;
	turn = +1;
	SetAction("Turn");
}

public func Stop()
{
	turn = 0;
	SetAction("Attach");
	Sound("CannonStop");
}

public func IsLamp() { return(true); }
