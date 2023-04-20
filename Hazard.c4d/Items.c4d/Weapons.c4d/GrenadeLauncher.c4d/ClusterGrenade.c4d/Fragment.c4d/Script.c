/*-- Splittergranaten-Fragment --*/

#strict

local explode_size;

func Construction(object byObj) 
{
  var shooter;
  // Waffe?
  shooter = GetShooter(byObj);
  
  explode_size = 12;
  AddEffect("HitCheck", this(), 1, 1, 0, SHT1,shooter);
  ScheduleCall(0,"Hit",30+RandomX(-20,20));
}

public func SetExplodeSize(int iExplodeSize) { explode_size = iExplodeSize; }

func Hit()
{
  BlastObjects(0,0, explode_size/2, Contained());
  Explode(explode_size,0,0,0,1);
}
func HitObject() { Hit(); }

func GrenadeSmoke()
{
  var iX = -GetXDir()/6;
  var iY = -GetYDir()/6;
  var iSize = 10+RandomX(120,200);

  CreateParticle("Smoke2",iX,iY,RandomX(-4,4),RandomX(-3,5),iSize,RGBa(75,65,55,55));
  if(!Random(4))  
    CreateParticle("Blast",iX,iY,0,0,iSize*2/3,RGBa(250,100+Random(100),100,160));

}
