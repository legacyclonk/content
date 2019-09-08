
/* Balloon-Sequenz */

#strict 2

#appendto SQ69

protected func SeqBalloonStart()
  {
  DestroyBalloon(pBalloon);

  var pFlag;
  while(pFlag = FindObject(FLAG)) RemoveObject(pFlag);
  CreateObject(FLAG,2192,389,0);
  // Shift view to the active clonk
  SeqSetView(pStoryClonk);
  }

protected func SeqBalloon20()
  {
  SeqMsg("$MsgCrash$",pBalloonClonk);
  }

protected func SeqBalloon30()
  {
  NewGoal("$MsgGoal4$");
  Sound("Ding");
  SeqStop(true);
  ScriptGo(1);
  }

global func DestroyBalloon(object balloon)
{
  Incinerate(balloon);
  var cnt = 21;
  while(cnt--) Smoke(Random(41)-20+GetX(balloon),Random(41)-20+GetY(balloon),Random(51));
  LaunchLightning(GetX(balloon),0,-10,21,60,21);
  LaunchLightning(GetX(balloon),0,-10,21,30,21);

  var pBalloonClonk = FindObject2(Find_ID(CLNK), Sort_Distance(GetX(balloon)-GetX(), GetY(balloon)-GetY()));
  AddEffect("IntDestroyBalloon", balloon, 1, 10, 0, 0, pBalloonClonk);
}

global func FxIntDestroyBalloonStart(object target, int number, int temp, object pBalloonClonk)
{
  EffectVar(1, target, number) = pBalloonClonk;
}

global func FxIntDestroyBalloonTimer(object target, int number)
{
  if(EffectVar(0, target, number) == 2)
    Sound("TreeDown1");

  if(EffectVar(0, target, number) == 10)
  {
    var pObj;
    while(pObj = FindObject(0,0,0,0,0,OCF_OnFire,0,0,0,pObj)) Extinguish(pObj);
  }

  if(EffectVar(0, target, number) == 20)
  {
    var pObj;
    while(pObj = FindObject(0,0,0,0,0,OCF_OnFire,0,0,0,pObj)) Extinguish(pObj);
    return -1;
  }

  ++EffectVar(0, target, number);
}
