/*-- Fadenkreuz --*/

#strict

static const CH_Distance = 60;

local my_r;

//Nein nein, das Fadenkreuz wolln wir nicht warpen :D
public func NoWarp() { return(1); }

func Initialize() {
  my_r = 0;
  SetVertex(0,1,CH_Distance,0,2);
  SetGraphics(0,this(),GetID(),1,GFXOV_MODE_Action,"Base",0);
  SetGraphics(0,this(),GetID(),2,GFXOV_MODE_Action,"Overlay",0);
  return(1);
}

protected func Check() {
  var user = GetActionTarget();
  if(!user)
    return(RemoveObject());
  //if(!(user->GetAlive()))
  //  return(RemoveObject());
  if(!(user->~IsAiming()))
    return(RemoveObject());	
}

public func Init(object pTarget) {
  SetOwner(GetController(pTarget));
  SetAction("Attach",pTarget);
  Schedule(Format("SetVisibility(%d)",VIS_Owner),2,0,this());
  SetVisibility(VIS_None);
}

public func SetAngle(int iAngle) {
  my_r = iAngle;
  UpdateAngle();
}

public func GetAngle() {
  return(Normalize(my_r,-180));
}

public func ChangeDir() {
  my_r = 360-my_r;
  UpdateAngle();
}

public func DoAngle(int iChange) {
  my_r += iChange;
  UpdateAngle();
}

public func UpdateAngle() {
  SetR(my_r+GetR(GetActionTarget()));
  Timer();
}

protected func Timer() {

  var angle = GetR();
  //var angle2 = GetActTime()*Global(1);
  
  //var siz = Sin(angle2,200);
  var w = 1000;
  var l = 1000;
  var fsin = -Sin(angle, 1000), fcos = Cos(angle, 1000);
  var width = +fcos*w/1000, height = +fcos*l/1000;
  var xskew = +fsin*l/1000, yskew = -fsin*w/1000;
  
  // set matrix values
  SetObjDrawTransform (
    width, xskew, 0,
    yskew, height, 0,
	0, 1
  );
}