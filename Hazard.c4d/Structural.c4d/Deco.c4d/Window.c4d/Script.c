/*-- Fenster --*/

#strict

func Initialize()
{
	SetAction("Window1");
	SetDir(0);
}

public func SetDir(int iDir)
{
	SetSolidMask(70, 91 * iDir, 70, 91, 0,0);
	return _inherited(iDir);
}

// Backward compatibility
public func Set(int iDir)
{
	SetDir(iDir);
}

/* Serialisierung */

public func Serialize(array& extra)
{
	extra[GetLength(extra)] = Format("Set(%d)", GetDir());
}
