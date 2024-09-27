#strict 2
#include WNDW

public func SetDir(int iDir)
{
	SetSolidMask(70, 47 * iDir, 70, 47, 0,0);
	return _inherited(iDir);
}
