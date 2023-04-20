/*-- Flak-Shell --*/

#strict
#include GREN

func AdjustR()
{
	SetR(Angle(0,0,GetXDir(),GetYDir()));
}

func ExplodeDelay() { return(20); }

func Hit()
{
	HitObject();
}

func HitObject(object target)
{
	RemoveEffect("Grenade",this());

	BlastObjects(x+GetX(),y+GetY(), iDamage); 
	
	var effSize = 20;
	
	AddLightFlash(25*effSize, 0, 0, RGBa(255,200,0,10));
	
	for(var i = 5+Random(3); i; i--)
	{
		var r = Random(3*effSize),
		    a = Random(360);
		
		var x = Sin(a,r),
		    y = Cos(a,r);
		
		//CastParticles("FSpark",6,effSize,x,y,effSize*5,effSize*9, RGBa(80,70,70,100),RGBa(80,70,70,100));
		//CreateParticle("Smoke2", x, y, 0, 0,effSize*7, RGBa(255,255,255,0));
		//if(!Random(3))
		//	Smoke(x,y,effSize/3);
		
		CreateParticle("Smoke3",x,y,RandomX(-2,2),RandomX(-2,4),effSize*7,RGBa(75,65,55,55));
		CastParticles("FSpark",3,effSize,x,y,effSize*10,effSize*12, RGBa(80,70,70,100),RGBa(80,70,70,100));
		if(Random(3))
			CreateSmokeTrail(effSize, a, x,y, this());
	}
	
	Sound("Blast2",0,0,50);
	RemoveObject();
}

func FxGrenadeTimer(object target, int effect, int time)
{
  if(time > ExplodeDelay()) return(HitObject());

}