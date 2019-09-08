/*-- Flagge --*/

#strict

private func Flying()
{
  SetDir(BoundBy(GetWind()/10+4,0,8));
  return(1);
}

protected func Timer()
{
	if(Contained())
		Contained()->~CheckFlag(this());
}

protected func BaseTimer()
{
	if(GetActionTarget())
		GetActionTarget()->~BaseTimer(this());
}