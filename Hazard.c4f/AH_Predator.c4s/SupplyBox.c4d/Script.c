/*-- Kiste --*/

#strict
#include CHST

public func Hit() { Sound("ClonkHit*"); }

public func Supply()
{
  if(!FindContents(STAP)) CreateContents(STAP);
  ScheduleCall(this(), "Supply", 400);
  if(down)
  {
    if(!FindContents(ENAP)) CreateContents(ENAP);
    if(!FindContents(GRAP)) CreateContents(GRAP);
  }
}
