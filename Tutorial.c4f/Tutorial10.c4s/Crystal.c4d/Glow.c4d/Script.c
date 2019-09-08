/*-- Funke --*/

#strict

public func Attach(pObj)
{
	SetAction("Glow", pObj);
	SetAlive(0);
}

protected func ChangeRotation()
{
	SetRDir(4 + Random(3));	
}