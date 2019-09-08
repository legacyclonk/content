/*-- Piranha --*/

#include FISH
#strict

/* Initialisierung */

protected func Initialize()
{
  SetAction("Swim");
  SetComDir(COMD_Left());
  if (Random(2)) SetComDir(COMD_Right());
}

/* TimerCall */

private func Activity()
{
  if (GetAction() eq "Walk") WalkDir();

  // Fortpflanzung (sollte eigentlich laichen...)
  if (!Random(ReproductionRate())) 
    Reproduction();

  // Altes Ziel verlieren
  SetActionTargets();
  SetCommand(this(),"None");

  // Beute
  var pPrey;
  if(pPrey = FindObject(0, 0,0,-1,-1, OCF_Prey()))
   // Höchstens 3 Verfolger
   if(ObjectCount2(Find_ID(GetID()), Find_ActionTarget(pPrey))<3)
    if(ObjectDistance(pPrey)<150)
      if(GetOCF(pPrey) & OCF_InLiquid())
      {
        SetActionTargets(pPrey);
        return(SetCommand(this(),"Follow",pPrey));
      }
  
  // Schwimmverhalten
  if (!GBackLiquid(0, -8) && GetAction() ne "Walk") return(SetComDir(COMD_Down()));
  if (Random(2)) return(1);
  if (GetAction()ne"Swim") return(1);
  if (!Random(5)) SetComDir(COMD_Up());
  if (Random(2)) 
    return(TurnRight(),SetComDir(COMD_Right()));
  else
    return(TurnLeft(), SetComDir(COMD_Left()));
}

/* Anknabbern... */

private func CheckAttack()
{
  var pPrey;
  if(GetAction() ne "Swim") return();
  if(!Contained())
    if(pPrey = FindObject(0,+1,+1,0,0,OCF_Prey(), 0, 0, NoContainer()))
      Attack(pPrey);
}

private func Attack(object pPrey)
{
  SetAction("Knabber");
  Sound("Munch*");
  DoEnergy(-4,pPrey);
  if(GetOCF(pPrey)&OCF_CrewMember())
    Sound("Hurt*",0,pPrey);
  if(!Random(3)) SetComDir(COMD_Stop(),pPrey);
  return(1);
}

/* Aktionen von außerhalb */

public func Cook()
{
  ChangeDef(CPIR);
  SetAlive();
  return(1);
}

/* Aktionen */

protected func Death() 
{
  SetDir(0);
  ChangeDef(DPIR);
  SetAction("Dead");
}
