/*-- Zauberenergie-Versorgung --*/

#strict

protected func Activate(iPlr) {
  MessageWindow(GetDesc(),iPlr);
}

func CheckMana()
{
  var pObj;
  while(pObj=FindObject(0,0,0,0,0,OCF_Alive(), 0,0,0,pObj))
    if(GetPhysical("Magic", 0, pObj))
      DoMagicEnergy(+1, pObj);
}
