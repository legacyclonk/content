#strict

func Completion()
{
  Sound("Fire");
  SetAction("Burn1");
  SetPhase(Random(14));
  SetComDir(COMD_Down());
  return(1);
}

func ChkIncinerate()
{
  while(Var(0)=FindObject(0,-6,-6,12,12,OCF_Prey(),0,0,NoContainer(),Var())) 
    if(Hostile(GetOwner(),GetOwner(Var()))) if(!OnFire(Var())) Incinerate(Var(0));
  return(1);
}

func Damage()
{
  Sound("Fire");
  Explode(15);
  return(1);
}
