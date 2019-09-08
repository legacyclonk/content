#strict

func Activate()
{
  Sound("Magic1");
  while(Var()=FindObject(0,0,0,0,0,OCF_CrewMember(),0,0,0,Var())) {
    if(GetOwner(Par())!=GetOwner(Var())) ObjectSetAction(Var(1)=CreateObject(ICBL,GetX(Var()),GetY(Var()),-1),"Freeze",Var());
    Extinguish(Var());
    AddVertex(GetVertex(0,0,Var()),
              GetVertex(0,1,Var()),Var(1));
  }

  SetAction("Ticking");
  return(1);  
}

func Remove()
{
  while(Var()=FindObject(ICBL,0,0,0,0,0,0,0,0,Var())) Var()->ICBL::Destruct();
return(RemoveObject());
}

func Tick()
{
 Sound("TickTack");
 return(1);
} 
