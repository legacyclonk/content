/*-- Kopfgeld --*/

#strict
#appendto SHRF

public func ContextRelaunch(pCaller) 
{
  [$CtxRelaunch$|Image=COWB|Condition=NoCrew]
  if(Global(GetOwner())==-1) { if(!relleft) return(PlayerMessage(GetOwner(), "$MsgNoMoreRelaunches$", this())); }
  if(Global(GetOwner())== 1) { if(!relright) return(PlayerMessage(GetOwner(), "$MsgNoMoreRelaunches$", this())); }
  var x = GetX(), y = GetY(), obj = Contained();
  GameCall("RelaunchPlayer", GetOwner(), NO_OWNER, 1);
//  SetCrewEnabled(0, GetCursor(GetOwner()));
  SetCrewEnabled(1, this());
  SetCursor(GetOwner(), this());
  Exit();
  SetPosition(x,y);
  if(obj) Enter(obj);
  return(1);
}

public func NoCrew()
{
  if(!GetCrew(GetOwner(),1)) return(1);	
}
