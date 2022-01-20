/*-- Keycard --*/

#strict

local Keynumber;

protected func Initialize()
{
	var i = Random(5)+1;
	if(i != 1)
		SetGraphics(Format("%d", i));
}

public func SetKeyNumber(int iBits)
{
    Keynumber = iBits;
}

public func GetKeyNumber()
{
    return(Keynumber);
}

public func IsKeycard()
{
    return(true);
}
