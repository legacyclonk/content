/*-- Wipf --*/

#strict
#include WIPF

local rider;

func Initialize()
{
  SetGraphics(0,0,WIPF);
  return(_inherited());
}

func Activity()
{
  if(rider) return(0);
  return(_inherited());
}
 
func ContactLeft()
{
  if(rider) return(0);
  return(_inherited());
}

func ContactRight()
{
  if(rider) return(0);
  return(_inherited());
}
  
func ControlLeft()
{ 
  if(GetAction()ne"Walk" && GetAction()ne"Swim") return(0);
  TurnLeft();
  return(1);
}

func ControlRight()
{ 
  if(GetAction()ne"Walk" && GetAction()ne"Swim") return(0);
  TurnRight();
  return(1);
}

func ControlUp()
{
  if(GetAction()S="Swim") 
    if(GetDir()) SetComDir(COMD_UpRight());
      else SetComDir(COMD_UpLeft());
    
  if (GetAction()S="Walk") 
    Jump();
  return(1);
}
  
func ControlDownSingle()
{
  if (GetAction()S="Walk") {
    SetComDir(0);
    SetXDir(0);
  }
  return(1);
}

func ControlDownDouble()
{
  if(!GetXDir()) {
    ObjectSetAction(rider,"Walk");
    rider=0;
  } else 
  if (GetAction()S="Walk") {
    SetComDir(0);
    SetXDir(0);
  } 
  return(1);
}

func ActivateEntrance()
{
  // Reiter verloren?
  if(!ObjectCall(rider,"IsRiding")) rider=0;
  // Clonks dürfen auf dem Wipf reiten, wenn er nicht bereits beritten wird und der Wipf bereit ist
  if(GetOCF(Par(0))&OCF_CrewMember() && !rider)
    if(GetAction()S="Walk" || (GetAction()S="Swim" && !GBackLiquid(3-6*GetDir(),-6))) {
      AdjustVertex();
      SetObjectOrder(this(),Par(0));
      if(InLiquid(Par(0))) SetPosition(GetX()+3-6*GetDir(),GetY()-6,Par(0));
      ObjectSetAction(rider=Par(0),"Ride",this());
    }
  return(1);
}

// Reitwipfe tauchen nicht gerne
func GoUp()
{
  if(!Random(3)) SetComDir(COMD_Up());
  return(1);
}
   
func Turned(){ return(AdjustVertex());}
    
func AdjustVertex()
{
  SetVertexXY(0,3-6*GetDir(),-4);
  return(1);
}

func Death()
{
  ObjectSetAction(rider,"Walk");
  return(_inherited());
}

func Destruction()
{
  ObjectSetAction(rider,"Walk");
  return(_inherited());
}
