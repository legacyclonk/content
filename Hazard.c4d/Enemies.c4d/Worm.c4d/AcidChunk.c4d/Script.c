/*-- Säurebrocken --*/

#strict 2

protected func AdjustR()
{
	SetR(Angle(0,0,GetXDir(),GetYDir()));
}

public func Launch(int xdir, int ydir, object byObj)
{
	SetXDir(xdir);
	SetYDir(ydir);
	AddEffect("HitCheck", this(), 1,1, 0, SHT1,byObj);
	AddAlienFlareEffect(this,500);
}

protected func Hit(int xdir, int ydir)
{
	var x,y;
	x = Sin(GetR(),14);
	y =-Cos(GetR(),14);

	CreateObject(WACM, x,y+32, GetOwner());
	
  for(var i = 5, glob ; i ; i--)
    {
    glob = CreateObject(SLST, 0, 0, GetOwner());
    glob ->~ Launch(RandomX(-40,40), RandomX(-40,40), 5);
    }
	
	RemoveObject();
}
