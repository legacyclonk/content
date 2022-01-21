/*-- Kartenprojektor --*/

#strict

local iNew; // Nächste Phase

func Initialize() {
  SetAction("Move");
	SetPhase(Random(20));
  return(1);
}

func Move()
{
  if(GetAction() eq "Stand") return();
	var iPhase=GetPhase();
	if( iPhase != iNew) 
		SetPhase(BoundBy(iNew, iPhase-1, iPhase+1)); // Noch nicht da
	else if(!Random(50)) // Hinbewegt: Irgendwann neues Ziel suchen. 
		iNew=Random(20);
	CreateParticle("PSpark", RandomX(-26,22), RandomX(-18,9), 0, 0, Random(16)+15, RGB(200,255,255));
}

public func TurnOff()
{
  var phase = GetPhase();
  SetAction("Stand");
  SetPhase(phase);
}
public func TurnOn()
{
  var phase = GetPhase();
  SetAction("Move");
  SetPhase(phase);
}

public func IsMachine() { return(1); }
public func EMPShock() { return(TurnOff()); }

/* Serialisierung */

public func Serialize(array& extra)
{
	if (GetAction() eq "Stand") {
		extra[GetLength(extra)] = "SetAction(\"Stand\")";
		extra[GetLength(extra)] = Format("SetPhase(%d)", GetPhase());
	}
}
