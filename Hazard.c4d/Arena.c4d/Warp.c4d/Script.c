/*-- Warp --*/

#strict

local iX, iY;

func Initialize() {
  SetAction("Warp");
  return(1);
}

public func Set(int iXDest, int iYDest) {
  iX = iXDest;
  iY = iYDest;
}

private func CheckWarp() // Timer um Zeugs zu warpen!
{
  for(var obj in FindObjects(Find_Distance(17), Find_NoContainer(), Find_Not(Find_Func("NoWarp")), Find_Or(Find_Category(C4D_Living),Find_Category(C4D_Object),Find_Category(C4D_Vehicle))))
    Warp(obj);
}

private func Warp(object pObj)
{
  var xv, yv;
  xv = pObj->GetXDir();
  yv = pObj->GetYDir();
  pObj->SetPosition(iX, iY);
  pObj->SetXDir(xv);
  pObj->SetYDir(yv);
  pObj->AddSpawnEffect(0, RGBa(0,200,255));
}

/* Serialisierung */

public func Serialize(array& extra)
{
	extra[GetLength(extra)] = Format("Set(%d,%d)", iX, iY);
}
