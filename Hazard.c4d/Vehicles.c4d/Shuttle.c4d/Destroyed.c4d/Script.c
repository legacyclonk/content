#strict

local target;

public func Init(int i) {
	SetAction("Be");
	SetDir(Random(2));
	SetPhase(i);
	SetR(Random(360));
	SetRDir(RandomX(-10,10));
	var r = Random(360);
	var s = RandomX(20,30);
	var xdir = Cos(r,s)-Sin(r,s);
	var ydir = Cos(r,s)+Sin(r,s);
	SetXDir(xdir);
	SetYDir(xdir);
}
