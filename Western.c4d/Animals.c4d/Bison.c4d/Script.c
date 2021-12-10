/*-- Bison --*/

#strict
#include ANIM

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
  if (Random(2) || (GetAction() ne "Walk" && GetAction() ne "Swim")) return(1);

  // Ein Ei legen
//  if (!Random(ReproductionRate())) 
//   Reproduction();	

  // Feind in Sicht: abhauen
  if(pEnemy=EnemyNearby())
    return(RunAway(pEnemy));
  
  // Springen
  if (GetAction() eq "Walk")
   if (!Random(3)) return(DoJump());

  // Umsehen
  if (GetAction() eq "Walk")
   if (!Random(8)) return(SetAction("HeadDown"));

  // Umdrehen
  if (Random(2)) return(TurnRight());
  return(TurnLeft());
}

// Mir gefällt es nicht, dass Tiere auch dann wegrennen, sollte sich der Clonk in einem
// anderem Tierkörper oder getarnt in einem Stein befinden, zudem sollte er nicht nur den
// nächsten Clonk abchecken sondern alle, die in der Nähe sind. Damit die Unsichtbarkeit
// nicht so einfach enttarnt wird, reagieren sie auch nicht auf Unsichtbare.
private func EnemyNearby() {
  var pEnemy, iDist, pContainer;
  var iList = FindObjects(Sort_Distance(), Find_OCF(OCF_CrewMember), Find_Distance(300));
  for(pEnemy in iList) {
    iDist = ObjectDistance(pEnemy);
    pContainer = Contained(pEnemy);
    //if(pContainer) if(GetID(pContainer)==GetID()) return();
    if(pContainer) if(GetCategory(pContainer) & 24) continue;
    if(GetAction(pEnemy) eq "Crawl") continue;
    if(GetVisibility(pEnemy)) continue;
    if(iDist > 150 && Contained(pEnemy)) continue;
    if(iDist > 100 && !PathFree(GetX(),GetY(),GetX(pEnemy),GetY(pEnemy))) continue;
    return(pEnemy);
  }
  return(0);
}

/* Verhaltenssteuerung */
  
private func RunAway(pEnemy) { // C4Object *pEnemy
  // Aufstehen
  if(GetAction()eq"Eat") SetAction("HeadUp");
  // Nahrungssuche einstellen
  if (GetCommand()eq"Follow") SetCommand(this(),"None");
  // In die entgegengesetzte Richtung
  if(GetX(pEnemy)>GetX()) TurnLeft();  
  if(GetX(pEnemy)<GetX()) TurnRight();
  // Bewegungskommando
  SetCommand(this(),"None");
  AddCommand(this(),"MoveTo",0,GetX()+100*(GetDir()*2-1),GetY(),0,20);
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
  if (Stuck() || (GetAction() ne "Walk" && GetAction() ne "Swim")) return();
  if (GetXDir() < 0) SetXDir(0);
  SetDir(DIR_Right());
  SetComDir(COMD_Right()); 
  return(1);
}

public func TurnLeft()
{
  if (Stuck() || (GetAction() ne "Walk" && GetAction() ne "Swim")) return();
  if (GetXDir() > 0) SetXDir(0);
  SetDir(DIR_Left());
  SetComDir(COMD_Left()); 
  return(1);
}

public func DoJump()
{
  if (GetAction() ne "Walk") return();
  if (Random(2)) Sound("Growl*");
  Jump();
}

/* Einwirkungen */

protected func Death()
{
  Sound("DeathGrowl");
//  SetDir(DIR_Left());
  ChangeDef(DBIS);
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
 SetCommand(this(), "None");
 TurnLeft();
 return(1);
}

protected func ContainedRight()
{
 [$TxtMovement$]
 SetCommand(this(), "None");
 TurnRight();
 return(1);
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
