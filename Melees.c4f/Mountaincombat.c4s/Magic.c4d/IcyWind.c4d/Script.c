#strict

func Activate()
{
  Sound("Wind2");
  if (GetDir(Par(0)) ==DIR_Left()) {
   while(Var()=FindObject(0,-150,-80,150,160,OCF_CrewMember(),0,0,NoContainer(),Var()))
     if(Hostile(GetOwner(Par()),GetOwner(Var())))
      if(!FindObject(CLDN,0,0,0,0,0,"WarnCold",Var()) && !FindObject(CLDN,0,0,0,0,0,"WarnColdX",Var()))
        ObjectSetAction(CreateObject(CLDN,0,0,-1),"WarnCold",Var(),Var()); 
  }
  if (GetDir(Par(0)) ==DIR_Right()) {
   while(Var()=FindObject(0,0,-80,150,160,OCF_CrewMember(),0,0,NoContainer(),Var()))
     if(Hostile(GetOwner(Par()),GetOwner(Var()))) 
       if(!FindObject(CLDN,0,0,0,0,0,"WarnCold",Var()) && !FindObject(CLDN,0,0,0,0,0,"WarnColdX",Var()))
         ObjectSetAction(CreateObject(CLDN,0,0,-1),"WarnCold",Var(),Var());
  }
  RemoveObject();
  return(1);
}
