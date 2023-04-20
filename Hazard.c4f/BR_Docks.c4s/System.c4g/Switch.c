/*-- Schalter --*/

#strict
#appendto SWTH

protected func ControlThrow()
{
	SetPhase((GetPhase()+1)%2);
  if(target)
  {
    target->~Access();
    ClearScheduleCall(target, "Access");
    ScheduleCall(target, "Close", 70);
  }
  return(1);
}
