/*-- Glühbirne --*/

#strict
#include BLGH

local base, length, cable;
local phase, speed, dir, minspeed; //phase as in Winkel.

func Initialize() {
  dir=Random(2)*-2+1;
  minspeed = 0;
	_inherited();
}

func CreateLight()
{
	pLight = AddLightAmbience(40);
}

func Init(object pObj, int iLength) {
  base = pObj;
  cable = CreateObject(BLBC);
  cable->Set(base, this());
  length = iLength;
  SetPosition(base->GetX(),base->GetY()+iLength);
  speed = iLength/3*-dir;
  phase = 0;
}

public func Set(int Length) {
  if(Length <= 0)
    return();
  length = Length;
  UpdatePos();
}

private func UpdatePos() {
  var x,y;
  x = Sin(phase/10,length);
  y = Cos(phase/10,length);
  SetPosition(base->GetX()+x,base->GetY()+y);
}

private func Adjust() {
  if(!base)
  {
  	RemoveObject(cable);
    RemoveObject();
  }
  if(!dir)
    return();
  //Position anpassen
  UpdatePos();
  
  phase += speed;
  speed += dir;
  
  if((speed > 0 && dir < 0) || (speed < 0 && dir > 0))
    if(speed%5)
      speed += dir;
    
    
  if(Abs(phase) >= 900) {
    speed = 0;
    phase = phase/Abs(phase)*899;
  }
  //Gegenrechnen, weil sonst überschwanken
  //while(Abs(speed) > maxspeed) {
    //speed -= dir;
  //}
  
  if((phase >= 0 && speed >= 0 && dir > 0) || (phase <= 0 && speed <= 0 && dir < 0)) {
    dir *= -1;
    if(Abs(speed) < 15) {
      speed+=dir;
      phase+=dir;
      if(Abs(speed) < 7 && Abs(phase) < 5) {
        dir = 0;
        speed = 0;
        speed = 0;
      }
    }
  }
  
  SetR(Angle(GetX(),GetY(),base->GetX(),base->GetY()));
  // Kabel anpassen
  cable->Update();
}

func IsBulletTarget(id ID) {
  if(ID != SHT1)
    return(0);
  var shot = FindObject(ID,0,0,-1,-1);
  if(!shot)
    return(0);
  
  speed += shot->GetXDir();
  speed = BoundBy(speed,-55,55);
  dir = BoundBy(-speed,-1,1);
  return(1);
}
