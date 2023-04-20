/*-- Geschoss --*/

#strict

func Initialize() {
   return(1);
}

func ResetRotation() {
    SetR(Angle(GetX(),GetY(),GetX()+GetXDir(),GetY()+GetYDir()),this());
}

func Hit() {
  for(var i=RandomX(10,15); i>0; --i)
  {
    var obj=CreateObject(CGFR, 0, 0, GetController());
    var xdir = RandomX(-70,70);
    var ydir = RandomX(-70,70);
    var angle = Angle(0,0,xdir,ydir);

    obj->SetExplodeSize(10+Random(10));
    SetSpeed(xdir, ydir, obj);
    if(!Random(2))
      CreateSmokeTrail(RandomX(20,30),angle+RandomX(-30,30),0,0,this());
  }

    Explode(45);
}
