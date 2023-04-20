/*-- Splittergranate --*/

#strict

// Ist eine Granate
#include GREN

func Hit()
{
	Bounce();
	Explode(30);
	CastClusters();
}

func HitObject(object clonk)
{
	// direkter Treffer -> normal explodieren weil ja noch nicht geclustert
	Explode(30);
	RemoveEffect("Grenade", this,0,1);
}

func Destruct()
{
	HitObject();
}

func TimeIsUp()
{
	Hit();
}

func CastClusters()
{
	for(var i=RandomX(8,11); i>0; --i)
	{
		var obj=CreateObject(CGFR, 0, 0, GetController());
		var xdir = GetXDir()+RandomX(-20,20);
		var ydir = GetYDir()+RandomX(-20,20);
		var angle = Angle(0,0,xdir,ydir);

		obj->SetExplodeSize(iDamage);
		SetSpeed(xdir, ydir, obj);
	}
}

func ExplodeDelay() { return(50); }
