#strict

local alpha;

func Destruct(){ return(alpha+=10);}

func NoAction() //Keine Äktschn!
{
 if(!ActIdle(GetActionTarget())) ObjectSetAction(GetActionTarget(),"Idle");
 if(alpha) Destructing();
 return(1);
}

func Destructing()
{
  SetClrModulation(RGBa(255,255,255,alpha));
  alpha+=5;
  if(alpha>200) {ObjectSetAction(GetActionTarget(),"Walk"); RemoveObject();}
  return(1);
}
