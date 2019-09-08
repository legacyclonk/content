/* Steinfalle */
#strict

local delay;

func Initialize()
{
  if(Stuck()) Var(0)=1;
  while(Stuck()) SetPosition(GetX(),GetY()+1);
  if(Var(0)) SetPosition(GetX(),GetY()+5);
  delay=Random(40);
  SetAction("Delaydings");
  return(1);
}

func RockFall()
{
  Var(0)=CreateObject(ROCK,0,0,-1);
  Local(0,Var(0))=1;
  return(1);
}

func DelayMe()
{
  if(GetActTime()>delay) SetAction("RockFall");
  return(1);
}
