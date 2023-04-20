/*-- Splittergranaten-Fragment --*/

#strict
#include CGFR


func Construction(object byObj) 
{
  var shooter;
  // Waffe?
  shooter = GetShooter(byObj);
  
  explode_size = 12;
  AddEffect("HitCheck", this(), 1, 1, 0, SHT1,shooter);
  ScheduleCall(0,"Hit",70+RandomX(-20,20));
}

func Hit()
{
  var flames = 8;
  var a = 180;
  
  for(var i = 0; i < flames ; ++i) {
    a += 360/flames + RandomX(-30,30);
	var strength = RandomX(30,40);
	var x = +Cos(a,strength);
	var y = -Sin(a,strength);
	var flame = CreateObject(FFLM,x/10,y/10);
	flame->SetMaxLifetime(40);
	flame->Launch(x,y,0,explode_size);
  }

  Sound("Inflame");
  //Explode(explode_size,0,0,0,1);
  RemoveObject();
  //return(1);
}