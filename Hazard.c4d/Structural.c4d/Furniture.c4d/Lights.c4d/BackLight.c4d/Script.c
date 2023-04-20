/*-- Hintergrundlicht --*/

#strict

local pLight, bOn;

public func LightSize() { return(100); }
public func Light() 
{
	return pLight; 
}

protected func Initialize() {
	CreateLight();
	TurnOn();
	return(1);
}

protected func CreateLight()
{
	pLight = AddLightAmbience(LightSize());
}

public func TurnOn() {
	if(EMPShocked()) return;
	bOn = true;
	SetAction("On");
	if(Light())
		Light()->TurnOn();
}

public func TurnOff() {
	if(!SetAction("Off"))
		SetAction("Idle");
	bOn = false;
	if(Light())
		Light()->TurnOff();
}

public func Switch() {
	if(GetAction() S= "On")
		TurnOff();
	else
		TurnOn();
}

public func IsLamp() { return(true); }
public func IsMachine() { return(true); }

public func EMPShock()
{
	TurnOff();
	EMPShockEffect(800+Random(200));
}

public func EMPShockEnd() {
	if(bOn) {
		SetAction("On");
		if(Light()) Light()->TurnOn();
	}
}

/* Serialisierung */

public func Serialize(array& extra)
{
	if (GetAction() ne "On")
		extra[GetLength(extra)] = "TurnOff()";
}
