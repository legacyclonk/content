/*-- Streifen --*/

#strict

local width,dir;

protected func Initialize() {
  Set(100,RGB(200,200,200),Random(2));
}

public func Set(int iWidth, int iRGB, bool bDir)
{
  width = iWidth;
  dir = !!bDir;
  SetShape(-width/2, -4, width, 8);
  var w = 1000*iWidth/8;
  SetObjDrawTransform(w,(1+dir*-2)*1000,0,0,1000,0,0,0);
  // Farbe
  SetColorDw(iRGB);
}

public func UpdateTransferZone()
{
  Set(width, GetClrModulation());
}

/* Serialisierung */

public func Serialize(array& extra)
{
	extra[GetLength(extra)] = Format("Set(%d,%d,%d)", width, GetColorDw(), dir);
}
