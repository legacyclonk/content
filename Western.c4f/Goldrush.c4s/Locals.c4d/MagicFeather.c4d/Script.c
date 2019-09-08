/*-- Feder --*/

#strict

protected func Timer()
{
  var pClonk = GetActionTarget();	
  SetAction(GetAction(pClonk));
  SetPhase(GetPhase(pClonk));
  SetDir(GetDir(pClonk));
}
