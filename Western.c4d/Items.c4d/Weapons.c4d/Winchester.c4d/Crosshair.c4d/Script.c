/*-- Fadenkreuz --*/

#strict

protected func Initialize()
{
  SetVisibility(VIS_Owner());
}

protected func Check()
{
  if(!WildcardMatch(GetAction(GetActionTarget()), "*Rifle*")) return(RemoveObject());
}
