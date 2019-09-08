/*-- Stalaktit --*/

#strict

local phase;

func Initialize() {
  SetAction("Hanging");
  SetPhase(phase=Random(4));
}

protected func Drop()
{
	if(Random(2) && FindObject(_CSN))
	{
		CastPXS("Water", 1, 0, RandomX(-7,7), RandomX(-19,0));
		ScheduleCall(this, "DropSound", 20, 1);
	}
}

func DropSound()
{
	Sound("CaveDrip*");
}