/*-- Piranha --*/

#strict
#include FISH

/* Initialisierung */

protected func Initialize() { return(Birth()); }

/* TimerCall mit KI-Steuerung */

private func Activity()
{
  // Die KI-Steuerung wird bei Besessenheit nicht gebraucht
  if (GetEffect("PossessionSpell", this())) return();
  
  if (GetAction() eq "Walk") WalkDir();

  // Fortpflanzung (sollte eigentlich laichen...)
  if (!Random(ReproductionRate())) 
    Reproduction();

  // Altes Ziel verlieren
  SetCommand(this(),"None");

  // Beute
  var pPrey;
  if(pPrey = FindObject(0, 0,0,-1,-1, OCF_Prey()))
    if(ObjectDistance(pPrey)<150)
      if(GetOCF(pPrey) & OCF_InLiquid())
        return(SetCommand(this(),"Follow",pPrey));
  
  // Schwimmverhalten
  if (!GBackLiquid(0, -8) && GetAction() ne "Walk") return(SetComDir(COMD_Down()));
  if (Random(2)) return(1);
  if (GetAction() ne "Swim") return(1);
  if (!Random(5)) SetComDir(COMD_Up());

  // Umdrehen
  if (Random(2)) return(TurnRight());
  return(TurnLeft());
}

/* Anknabbern... */

private func CheckAttack()
{
  // Die KI-Steuerung wird bei Besessenheit nicht gebraucht
  if (GetEffect("PossessionSpell", this())) return();

  var pPrey;
  if(GetAction() ne "Swim") return();
  if(!Contained())
    if(pPrey = FindObject(0,+1,+1,0,0,OCF_Prey(),0,0,NoContainer()))
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

/* Einwirkungen */

public func Cook()
{
  ChangeDef(CPIR);
  SetAlive();
  return(1);
}

protected func Death() 
{
  SetDir(0);
  ChangeDef(DPIR);
  SetAction("Dead");
}

/* Vermehrung */

public func Birth()
{
  SetAction("Swim");
  SetComDir(COMD_Left());
  if(Random(2)) SetComDir(COMD_Right());
}

/* Steuerung durch Besessenheit */

protected func ContainedThrow() { return(1); }

protected func ContainedDig()
{
 [$TxtAttack$]
  var pPrey;
  if(GetAction() ne "Swim") return();
  if(!Contained())
    if(pPrey = FindObject(0,+1,+1,0,0,OCF_Prey(),0,0,NoContainer()))
      Attack(pPrey);
 return(1);
}