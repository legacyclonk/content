/*-- Drachenstatue --*/

#strict 2

protected func Initialize()
{
	SetAction("Waiting");	
}

protected func VisitorCheck()
{
  if (FindObject2(Find_InRect(-40, -100, 80, 200), Find_OCF(OCF_CrewMember)))
    Notify();
}

public func Notify()
{
  Sound("DragonGrowl");
	SetAction("Rise");
}	

protected func Passing()
{
  return true;
}
