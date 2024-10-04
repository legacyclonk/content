/*-- Schwärmer --*/

#strict
#include MINE

local xdir, ydir;

func Activate() {
    DoDamage(1);

	var speed = RandomX(0,50);
	var angle = Random(360);
    xdir = Sin(angle, speed);
    ydir = -Cos(angle, speed);

    SetSpeed(xdir, ydir);
    SetAction("Fly");
    
    var tmp = CreateObject(MFLG,0,0,GetController());
    tmp->Set(this());
    tmp->SetR(Random(360));
    tmp->SetCon(70);
}

protected func DecreaseSpeed() {
    if(xdir > 0)
      xdir--;
    if(xdir < 0)
      xdir++;
    if(ydir > 0)
      ydir--;
    if(ydir < 0)
      ydir++;

    SetSpeed(xdir, ydir);
    if(!(GetActTime()%RandomX(20,22))) Warning();
    Check();
}

protected func Warning() {
    CreateParticle("PSpark",0,0,0,0,40,RGBa(0,64,255,120),this());
}

protected func Triggered() {                // ausgelöst (nach delay)
    Explode(15);
}

private func MaxDamage() { return(6); }
private func ExploDist() { return(16); }
private func ExploDelay() { return(5); }

public func IsEquipment() { return(0); }
