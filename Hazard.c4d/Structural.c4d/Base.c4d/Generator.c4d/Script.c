/*-- Generator --*/

#strict

public func IsWorking()
{
	return(GetAction() eq "Work");
}

public func TurnOn()
{
	if((GetAction() eq "Work")||
		 (GetAction() eq "Start1")||
		 (GetAction() eq "Start2"))
		return;
  var phase = GetPhase();
  if(GetAction() eq "Offline" || GetAction() eq "End1") SetAction("Start1");
  if(GetAction() eq "End2") SetAction("Start2");
  SetPhase(phase);
	Sound("Energize");
}

public func TurnOff()
{
  if((GetAction() eq "Idle")||
		 (GetAction() eq "End1")||
		 (GetAction() eq "End2"))
		return;
  var phase = GetPhase();
  if(GetAction() eq "Work" || GetAction() eq "Start2") SetAction("End2");
  if(GetAction() eq "Start1") SetAction("End1");
  SetPhase(phase);
  Sound("DeEnergize");
}

protected func Started() { GameCall("PowerSupplyOn"); }
protected func Ended() { GameCall("PowerSupplyOff"); }

protected func Initialize() { SetAction("Offline"); }

protected func Working()
{
  if(Random(100)) return();
	Malfunc();
}

public func Malfunc()
{
  var x,y,dir;
  dir = 1;
  if(Random(2)) dir = -1;
  x = RandomX(-25,100) * -dir;
  y = RandomX(-50,50);
  CreateObject(GNRL, x, y, GetOwner())->Launch(GetX()+x,GetY()+y,BoundBy(1800*dir,0,1800));
  CastObjects(SPRK, Random(3), 20, x,y);
}

/* Serialisierung */

public func Serialize(array& extra)
{
	if (GetAction() eq "Work") {
		extra[GetLength(extra)] = "SetAction(\"Work\")";
		extra[GetLength(extra)] = "GameCall(\"PowerSupplyOn\")";
	}
}
