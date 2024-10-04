/*-- Kabel --*/

#strict

local base, bulb;

func Set(object target1, object target2)
{
  base = target1;
  bulb = target2;
  
  SetPosition(target1->GetX(), target1->GetY());
  
  Update();
}

func Update()
{
	var r = -Angle(base->GetX(),base->GetY(), bulb->GetX(), bulb->GetY());
	if(r < 0 && r > -180)
		r -= 2;
	//var xoff = -Abs(base->GetX() - bulb->GetX())/2;
	//var yoff = -Abs(base->GetY() - bulb->GetY())/2;
	var xoff = 0;
	var yoff = 0;
	var d = ObjectDistance(base, bulb);
	
  var fsin=Sin(r, 1000), fcos=Cos(r, 1000);
  SetObjDrawTransform (
    +fcos/2, +fsin*d, (1000-fcos)*xoff - fsin*yoff,
    -fsin/2, +fcos*d, (1000-fcos)*yoff + fsin*xoff
  );
}

/* Serialisierung */

public func NoSerialize() { return !false; }
