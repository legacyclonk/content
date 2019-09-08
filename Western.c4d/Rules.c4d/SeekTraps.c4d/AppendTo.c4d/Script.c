/*-- AppendTo --*/

#strict
#appendto CLNK

protected func Initialize()
{
  if(!FindObject(ST4V)) return(_inherited());
  if(this()->~IsTrapper()) return(_inherited());
  AddEffect("SeekTraps",this(),1,3,0,ST4V);
  return(_inherited());
}

public func TrapSeeRange()
{
  if(!FindObject(ST4V)) return(_inherited());
  return(Max(_inherited(),50));
}