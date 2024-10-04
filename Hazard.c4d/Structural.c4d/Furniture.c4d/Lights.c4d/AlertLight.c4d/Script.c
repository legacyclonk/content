/*-- Alarmlicht --*/

#strict
#include BLGH

local pAlert, pAlertWidth;
local dir;

protected func Initialize() {
	dir = 1;
	pLight = AddLightAmbience(60);
	if(pLight)
		pLight->ChangeColor(RGBa(255,50,50,80));

	pAlert = AddLightCone(1000, RGB(255,50,50), this());
	pAlert->ChangeSizeXY(0,100);
	pAlert->ChangeR(GetR()+90);

	// Alarmlicht sind erstmal *nicht* an
	TurnOff();
	
	Schedule(Format("SetR(GetR(Object(%d)), Object(%d))", ObjectNumber(this()), ObjectNumber(pAlert)), 1);
	
	return(1);
}

public func ChangeLightColor(int iNewRGBa)
{
	SetClrModulation(iNewRGBa);
	if(pLight)
		pLight->ChangeColor(iNewRGBa);
	pAlert->ChangeColor(iNewRGBa);
}

public func TurnOn() {
	if(EMPShocked()) return;
	if(GetAction() eq "On") return;
	bOn = true;
	SetAction("On");
	if(pLight) pLight->TurnOn();
	if(pAlert) pAlert->TurnOn();
}

public func TurnOff() {
	if(GetAction() eq "Off") return;
	SetAction("Off");
	bOn = false;
	if(pLight) pLight->TurnOff();
	if(pAlert) pAlert->TurnOff();
}


protected func Alerting() {
	var phase,size;
	phase = GetPhase();
	if(phase == 3 || phase == 11)
		ChangeDir();

	var len = GetActMapVal("Length",GetAction(),GetID());
	var percent = Sin(360*GetPhase()/len,1000);
	var percent2 = Cos(360*GetPhase()/len,1000);
	if(pLight)
		pLight->ChangeSize(140+Max(0,240*percent2/1000));
	pAlert->ChangeSizeXY(1000,percent*3/2);
	var r,g,b,a;
	SplitRGBaValue(GetClrModulation(pAlert),r,g,b,a);
	pAlert->ChangeColor(RGBa(r,g,b,128-Abs(percent)*128/1000));

}

protected func ChangeDir() {
	dir *= -1;
	pAlert->SetObjectOrder(this(),0,dir<0);
}

public func EMPShock()
{
	SetAction("Off");
	if(pLight) pLight->TurnOff();
	if(pAlert) pAlert->TurnOff();
	EMPShockEffect(800+Random(200));
}

public func EMPShockEnd() {
	if(bOn) {
		SetAction("On");
		if(pLight) pLight->TurnOn();
		if(pAlert) pAlert->TurnOn();
	}
}

/* Serialisierung */

public func Serialize(array& extra)
{
	_inherited(extra);
	if (GetClrModulation()) {
		extra[GetLength(extra)] = Format("ChangeLightColor(%d)", GetClrModulation());
	}
}
