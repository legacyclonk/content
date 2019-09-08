/*-- B‰r --*/

#strict
#include ANIM

local iRun;
local iAggressiv;

public func IsPossessible() { return(1); }

/* Initialisierung */

protected func Initialize() { return(Birth()); }

/* TimerCall mit KI-Steuerung */

private func Activity()
{
  var pEnemy;
  // Die KI-Steuerung wird bei Besessenheit nicht gebraucht
  if (GetEffect("PossessionSpell", this())) return();

  // Nichts machen
  if (Random(2) || (GetAction() ne "Walk" && GetAction() ne "Run" && GetAction() ne "Swim")) return(1);

  // Ein Ei legen
//  if (!Random(ReproductionRate())) 
//   Reproduction();	

  // Feind in Sicht: abhauen
  if(pEnemy=EnemyNearby())
  {
//    if(GetEnergy()>60)  return(RunAway(pEnemy));
//    else   return(RunAttack(pEnemy));
  }
  
  // Springen
  if (GetAction() eq "Walk" || GetAction() eq "Run")
   if (!Random(3)) return(DoJump());

  if (!Random(3)) if(GetAction()S="Walk") return(SetAction("Run")); else return(iRun=0, SetAction("Walk"));

  if (!Random(3)) return(SetAction("Attack"));

  // Umsehen
//  if (GetAction() eq "Walk")
//   if (!Random(8)) return(SetAction("HeadDown"));

  // Umdrehen
  if (Random(2)) return(TurnRight());
  return(TurnLeft());
}

private func EnemyNearby() {
  var pEnemy=FindObject(0,0,0,-1,-1, OCF_CrewMember());
  var iDist=ObjectDistance(pEnemy);
  var pContainer=Contained(pEnemy);
  if(pContainer) if(GetID(pContainer)==GetID()) return();
  if(pEnemy)
    if(iDist<350)
      if(!Contained(pEnemy) || iDist<150)
        if(PathFree(GetX(),GetY(),GetX(pEnemy),GetY(pEnemy)) || iDist<100 )
          return(pEnemy);
}

/* Verhaltenssteuerung */
  
private func RunAway(pEnemy) { // C4Object *pEnemy
  // Nahrungssuche einstellen
  if (GetCommand()eq"Follow") SetCommand(this(),"None");
  // In die entgegengesetzte Richtung
  if(GetX(pEnemy)>GetX()) TurnLeft();  
  if(GetX(pEnemy)<GetX()) TurnRight();
  // Bewegungskommando
  SetCommand(this(),"None");
  AddCommand(this(),"MoveTo",0,GetX()+100*(GetDir()*2-1),GetY(),0,20);
  // Rennen
//  SetAction("Run");
}

private func RunAttack(pPrey) { // C4Object *pEnemy
  if(Contained(pPrey)) return();	
  // Nahrungssuche einstellen
  if (GetCommand()eq"Follow") SetCommand(this(),"None");
  // In die passende Richtung
  if(GetX(pPrey)<GetX()) TurnLeft();  
  if(GetX(pPrey)>GetX()) TurnRight();
  // Bewegungskommando
  SetCommand(this(),"None");
  // Immer von hinten mit einem Abstand angreifen
  var iFromSide = +1; if (GetX(pPrey)>GetX()) iFromSide = -1;
  SetCommand(this(), "MoveTo", 0, GetX(pPrey) + 21 * iFromSide, GetY(pPrey));
  // Rennen
  if(!GetEffect("Attack", this())) AddEffect("Attack", this(), 1, 10, this());
  iRun=1;  
}

func FxAttackTimer()
{
  if(!Random(10)) return(-1, SetCommand(this(), "None"));	
  var pEnemy = EnemyNearby();
  if(Attack()) return();
  // Feind in Sicht: abhauen
  if(!Contained(pEnemy)) return(RunAttack(pEnemy));
  else return(-1, SetCommand(this(), "None"));
}

private func Attack()
{
  // W‰hrend des Schwimmens kann alle 2 Sekunden einmal gebissen werden
  if (GetEffect("IntAttackDelay", this()) || (GetAction() ne "Walk" && GetAction() ne "Run")) return();
  
  // Beute suchen
  var pPrey = FindObject(0, -20 + 40 * GetDir(), 0, -1, -1, OCF_Prey(), 0, 0, NoContainer());
  if(Abs(GetX()-20 + 40 * GetDir()-GetX(pPrey))>10) return();
  if(Abs(GetY()-GetY(pPrey))>10) return();
  if (!pPrey || !GetAlive(pPrey)) return();
  
  // Beiﬂen :D
  Punch(pPrey, 8);
  Fling(pPrey, -1 + 2*GetDir(), -2);
  SetAction("Attack");
  Sound("Munch1");
  AddEffect("IntAttackDelay", this(), 1, 70);
  return(1);
}

private func Walking()
{
  // Vorherige Gangart wieder aufnehmen (nach Sprung oder Wendung)
  if (iRun) 
    { SetAction("Run"); return(); }
  // Geschwindigkeit und Ger‰usch
  SetPhysical("Walk",55000,2);
  // Beute am Maul beiﬂen
  if (Attack()) return();
  return(1);
}
  
private func Running()
{
  iRun=1;
  // Geschwindigkeit und Ger‰usch
  SetPhysical("Walk", 90000, 2);
  // Beute am Maul beiﬂen
  if (Attack()) return();
  return(1);     
}

/* Kontakte */

protected func ContactLeft()
{
  // Die KI-Steuerung wird bei Besessenheit nicht gebraucht
  if (GetEffect("PossessionSpell", this())) return();
  
  return(TurnRight());
}

protected func ContactRight()
{
  // Die KI-Steuerung wird bei Besessenheit nicht gebraucht
  if (GetEffect("PossessionSpell", this())) return();
  
  return(TurnLeft());
}

/* Aktionen */

public func TurnRight()
{
  if (Stuck() || (GetAction() ne "Walk" && GetAction() ne "Run" && GetAction() ne "Swim")) return();
  if (GetXDir() < 0) SetXDir(0);
  SetDir(DIR_Right());
  SetComDir(COMD_Right()); 
  return(1);
}

public func TurnLeft()
{
  if (Stuck() || (GetAction() ne "Walk" && GetAction() ne "Run" && GetAction() ne "Swim")) return();
  if (GetXDir() > 0) SetXDir(0);
  SetDir(DIR_Left());
  SetComDir(COMD_Left()); 
  return(1);
}

public func DoJump()
{
  if (GetAction() ne "Walk" && GetAction() ne "Run") return();
  if (Random(2)) Sound("Growl*");
  Jump();
}

/* Einwirkungen */

protected func Death()
{
  Sound("DeathGrowl");
//  SetDir(DIR_Left());
  ChangeDef(DBER);
  SetAction("Dead");
  return(1);
}

/* Vermehrung */
/*
private func ReproductionRate() { return(2000); } // Die Chance, dass in einem Timerintervall eine Vermehrung stattfindet
private func MaxAnimalCount()   { return(4); }    // Maximale Tieranzahl im Umkreis


private func CountMe()
{
  var ReprodSize = ReproductionAreaSize();
  var ReprodSizeHalb = ReprodSize  / -2;
  return(ObjectCount(GetID(), ReprodSizeHalb, ReprodSizeHalb, ReprodSize , ReprodSize , OCF_Alive()) + ObjectCount(MEGG, ReprodSizeHalb, ReprodSizeHalb, ReprodSize , ReprodSize , 0));
}*/

/* Steuerung durch Besessenheit */

protected func ControlCommand(szCommand, pTarget, iTx, iTy)
{
 // Bewegungskommando
 if (szCommand eq "MoveTo")
  return(SetCommand(this(),szCommand, pTarget, iTx, iTy));
 return(0);
}

protected func ContainedLeft()
{
  [$TxtMovement$]
  if(GetDir() == DIR_Right) {
    if(LocalN("iRun") == 0)
    {
      SetCommand(this(), "None");
      TurnLeft();
      return(1);
    }
    LocalN("iRun") = 0;
    if(GetAction() eq "Run")
      SetAction("Walk");
    return(1);
  }
  LocalN("iRun") = 1;
  if(GetAction() eq "Walk")
    SetAction("Run");
  return(1);
}

protected func ContainedRight()
{
  [$TxtMovement$]
  if(GetDir() == DIR_Left) {
    if(LocalN("iRun") == 0)
    {
      SetCommand(this(), "None");
      TurnRight();
      return(1);
    }
    LocalN("iRun") = 0;
    if(GetAction() eq "Run")
      SetAction("Walk");
    return(1);
  }
  LocalN("iRun") = 1;
  if(GetAction() eq "Walk")
    SetAction("Run");
  return(1);
}

// Nur mal zur Gem¸tlichkeit
protected func ContainedLeftDouble()
{
  [$TxtMovement$]
  if(_inherited()) return();
  return(ContainedLeft());
}

protected func ContainedRightDouble()
{
  [$TxtMovement$]
  if(_inherited()) return();
  return(ContainedRight());
}

protected func ContainedUp()
{
 [$TxtMovement$]
 SetCommand(this(), "None");

 if (GetAction() eq "Swim")
  return(SetComDir(COMD_Up()));

 DoJump();
 return(1);
}

protected func ContainedDown()
{
 [$TxtMovement$]
 SetCommand(this(), "None");

 if(Contained()) return SetCommand(this, "Exit");

 if (GetAction() eq "Swim")
  return(SetComDir(COMD_Down()));

 if (GetAction() eq "Walk")
  SetAction("HeadDown");
  
 return(1);
}

protected func ContainedThrow() { return(1); }

protected func ContainedDigDouble()
{
 [$TxtLeave$]
 RemoveEffect("PossessionSpell", this());
 return(1);
}
