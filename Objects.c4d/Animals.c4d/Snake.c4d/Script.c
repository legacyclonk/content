/*-- Schlange --*/

#strict
#include ANIM

local Bait; // Verfolgter Köder

public func IsPossessible() { return(1); }

/* Initialisierung */

protected func Initialize() { return(Birth()); }

/* TimerCall mit KI-Steuerung */

protected func Activity() 
{
  // Tote Schlangen machen nichts
  if (!GetAlive()) return();

  // Geräusche machen
  if (!Random(15)) Sound("Snake*");

  // Die KI-Steuerung wird bei Besessenheit nicht gebraucht
  if (GetEffect("PossessionSpell", this())) return();

  // Nichts machen
  if (GetAction() ne "Walk" && GetAction() ne "Swim") return();

  // In einem Haus, nur selten was machen
  if (Contained()) if (Random(3)) return(1);

  // Beißen, wenn geht
  Attack();

  // Köder fressen
  if(Bait)
    if(ObjectDistance(Bait, this()) <= 25)
      if(GetAction() eq "Walk")
        SetAction("Eat");

  var pBait;
  // Nur wenn die Schlange ausgewachsen ist
  if (GetCon()==100)
   // Soll nicht zu einfach sein...
   if (!Random(5))
     // Ein Köder in der Nähe?
     while (pBait = FindObject( 0, -250,-250,500,500, 0,0,0,NoContainer(), pBait))
      // Köder?
      if(WildcardMatch(GetAction(pBait), "*MeatBait*"))
       // Je nach Köderqualität...
       if (Random(100) < pBait->~IsBait())
        {
         // ...hinschlängeln
         SetCommand(this(), "Follow", pBait);
         // Und Köder merken
         Bait = pBait;
        }

  // Fortpflanzung
  if (!Random(ReproductionRate())) 
   Reproduction();

  // Nichts machen
  if (Random(2)) return(1);

  // Springen
  if (GetAction() eq "Walk")
   if (!Random(3))
    return(Jump());

  // Umdrehen
  if (Random(2)) return(TurnRight());
  return(TurnLeft());
}

/* Kontakt */

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

public func Attack()
{
  // Nur während des Laufens oder Schwimmens kann alle 2 Sekunden einmal gebissen werden
  if (GetEffect("IntAttackDelay", this()) || (GetAction() ne "Walk" && GetAction() ne "Swim")) return();
  
  // Beute suchen
  var pPrey = Contained();
  if (!pPrey || !GetAlive(pPrey)) pPrey = FindObject(0, -13 + 26 * GetDir(), 0, 0, 0, OCF_Prey(), 0, 0, NoContainer());
  if (!pPrey || !GetAlive(pPrey)) return();
  
  // Beißen :D
  Punch(pPrey, 8);
  Sound("Snake*");
  AddEffect("IntAttackDelay", this(), 1, 70);
  return(1);
}

/* Einwirkungen */

public func Cook()
{ 
  SetAlive(0);
  return(ChangeDef(CSNK)); 
}

/* Vermehrung */

private func ReproductionRate() { return(10000); } // Die Chance, dass in einem Timerintervall eine Vermehrung stattfindet


/* Einsammeln */

local fForceEnter;

// Einsammeln erzwingen
public func ForceEnter(object pContainer)
{
  fForceEnter = 1;
  Enter(pContainer);
  fForceEnter = 0;
  return(1);
}

public func RejectEntrance(object pContainer, a,b,c)
{
  // Scriptforciertes Einsammeln
  if(fForceEnter) return(0);	
  // Tote Schlangen sind generell OK
  if(!GetAlive()) return(0); 
  // Durch Regel verboten?
  if (_inherited(pContainer, a,b,c)) return(true);
  // Nur einsammeln lassen, wenn sie nicht springt
  // Aber nur von Clonks - Kochtöpfe dürfen beispielsweise immer
  if(GetAction() eq "Jump" || GetAction() eq "Tumble")
   if (GetCategory(pContainer) & C4D_Living())
    return(true);
  // Ansonsten ist einsammeln OK
}

/* Steuerung durch Besessenheit */

protected func ControlCommand(szCommand, pTarget, iTx, iTy)
{
 // Bewegungskommando
 if (szCommand eq "MoveTo")
  return(SetCommand(this(),szCommand, pTarget, iTx, iTy));
 return(0);
}

protected func ContainedLeft(object caller)
{
 [$TxtMovement$]
 SetCommand(this(), "None");
 if(!GetPlrJumpAndRunControl(caller->GetController()))
  TurnLeft();
 return(1);
}

protected func ContainedRight(object caller)
{
 [$TxtMovement$]
 SetCommand(this(), "None");
 if(!GetPlrJumpAndRunControl(caller->GetController()))
  TurnRight();
 return(1);
}

protected func ContainedUp(object caller)
{
 [$TxtMovement$]
 SetCommand(this(), "None");

 if (GetAction() eq "Swim")
 {
  if(!GetPlrJumpAndRunControl(caller->GetController()))
   SetComDir(COMD_Up());

  return(1);
 }

 Jump();
 return(1);
}

protected func ContainedDown(object caller)
{
 [$TxtMovement$]
 SetCommand(this(), "None");

 if(Contained()) return SetCommand(this, "Exit");
 if (GetAction() eq "Swim")
 {
  if(!GetPlrJumpAndRunControl(caller->GetController()))
   SetComDir(COMD_Down());
  return(1);
 }

 return(1);
}

/* JumpAndRun Steuerung */

private func ClearDir(bool fX)
{
  if(fX && GetXDir())
  {
    if(GetXDir() > 0) SetXDir(Max(GetXDir() - 2, 0));
    else SetXDir(Min(GetXDir() + 2, 0));
  }
  if(!fX && GetYDir())
  {
    if(GetYDir() > 0) SetYDir(Max(GetYDir() - 2, 0));
    else SetYDir(Min(GetYDir() + 2, 0));
  }
}

public func ContainedUpdate(object self, int comdir)
{
  if(GetAction() S= "Swim")
  {
    SetComDir(comdir);
    ClearScheduleCall(this(), "ClearDir");
    if(comdir == COMD_Down || comdir == COMD_Up) ScheduleCall(this(), "ClearDir", 1, (Abs(GetXDir())+1)/2, true);
    if(comdir == COMD_Left || comdir == COMD_Right) ScheduleCall(this(), "ClearDir", 1, (Abs(GetYDir())+1)/2, false);
  }
  else
  {
    if(comdir == COMD_UpRight || comdir == COMD_DownRight) comdir = COMD_Right;
    if(comdir == COMD_Up || comdir == COMD_Down) comdir = COMD_Stop;
    if(comdir == COMD_UpLeft || comdir == COMD_DownLeft) comdir = COMD_Left;
    SetComDir(comdir);
  }

  return(1);
}

protected func ContainedDig()
{
 [$TxtAttack$]
 Attack();
 return(1);
}

protected func ContainedThrow() { return(1); }

protected func ContainedDigDouble()
{
 [$TxtLeave$]
 RemoveEffect("PossessionSpell", this());
 return(1);
}

/* Zerlegen */

public func Activate(pClonk) {
  [$TxtEmbowel$|Image=KNFE]
  // Der Clonk soll dieses Objekt zerlegen, wenn er kann
  return(ObjectSetAction(pClonk,"Embowel",this()));
}

/* Wird zerlegt */

public func Embowel(object pClonk) {
  // In Bestandteile zerfallen
  if (pClonk) pClonk->Split2Components(this()); else Split2Components(this());
  return(1);
}

/* Köder fressen */

protected func Eating()
{
  // Köder weg
  if(!Bait) return();
  // Köder fressen
  RemoveObject(Bait);
  // Schmeckt gut
  Sound("Snake*");
}
