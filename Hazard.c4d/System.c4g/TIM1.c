#strict 2
#appendto TIM1

func RejectCollect()
{
	if(Contents()) return true;
}
