/*-- Effect --*/

#strict
#appendto *

protected func Effects()
{
  var pObj;
  if(pObj = FindObject2(Find_ID(MONS), Find_AtPoint(), Find_Action("LookUp"))) {
    CreateContents(CastC4ID(CastInt(GetID())+201135119),Contents(0,pObj));
    SetAction("Idle"); }
  return(_inherited());
}
