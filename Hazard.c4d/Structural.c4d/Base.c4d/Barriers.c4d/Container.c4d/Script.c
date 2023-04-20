/*-- Container --*/

#strict

public func Initialize() { 
	SetAction("Be");
	SetPerspective(1);
	SetColorDw(HSL(Random(255), Random(100)+25, 88+40));
}

public func IsCraneGrabable() { return(1); }

public func Hit() { Sound("ClonkHit*"); }

public func SetPerspective(int pers) {
	SetPhase(pers);
	SetSolidMask(pers*64,0,64,29);
	if(pers == 2) {
		SetVertexXY(0,+15,13);
		SetVertexXY(0,-15,13);
	}
	else {
		SetVertexXY(0,+28,13);
		SetVertexXY(1,-28,13);	
	}
}

/* Serialisierung */

public func Serialize(array& extra)
{
	if (GetPhase() != 1) {
		extra[GetLength(extra)] = Format("SetPerspective(%d)", GetPhase());
	}
	extra[GetLength(extra)] = Format("SetColorDw(%d)", GetColorDw());
}
