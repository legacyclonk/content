/*-- Funke --*/

#strict

public func Attach(pObj)
{
	SetAction("Glow", pObj);
}

protected func ChangeRotation()
{
	SetRDir(4 + Random(3));	
}