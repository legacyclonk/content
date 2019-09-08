/*-- Schlange --*/

#strict
#include ANIM

local iTime;

public func IsPossessible() { return(0); }

/* Initialisierung */

protected func Initialize() { AddEffect("IntFire", this(), 1, 1, this()); return(Birth()); }

/* TimerCall mit KI-Steuerung */

local pTarget;

// Ziel setzten
func SetTarget(pNewTarget) { pTarget = pNewTarget; SetCommand(this(), "Follow", pTarget); iTime = 1;}

// Feuereffekt
func FxIntFireTimer() 
{ 
	if(!GBackLiquid())
	CreateParticle("Fire2",RandomX(-14, 14),RandomX(4, 8),0,RandomX(-2, -4),RandomX(40,60),RGB(210,120,110), this()); 
}

func FxIntFireEffect (szNewEffectName, pCaster, iEffectNumber, iNewEffectNumber)
{
   if(WildcardMatch(szNewEffectName,"*Freeze*"))  
	   return(RemoveEffect("IntFire", this()));
}

func FxIntFireStop() { RemoveObject(); }

protected func Activity() 
{
  // Verschwinden
  if(iTime) if(iTime++ >= 40)
  {
    CastParticles("PSpark", 10, 20, 0, 0, 50, 70, RGBa(255, 0, 100), RGBa(100, 0, 0));
    Sound("Poff");
    return(Kill());
  }
  
  // Geräusche machen
  if (!Random(15)) Sound("Snake*");

  // Die KI-Steuerung wird bei Besessenheit nicht gebraucht
  if (GetEffect("PossessionSpell", this())) return();

  // Nichts machen
  if (GetAction() ne "Walk" && GetAction() ne "Swim") return(0);

  // In einem Haus, nur selten was machen
  if (Contained()) if (Random(3)) return(1);

  // Beißen, wenn geht
  Attack();

  // Fortpflanzung
  if (!Random(ReproductionRate())) 
   Reproduction();

  // neues Zeil suchen
  if(!pTarget || !GetAlive(pTarget))
  {
    pTarget=0;	  
    SetCommand(this(), "");
    while(pTarget = FindObject(0, 0, 0, -1, -1, OCF_Prey(), 0, 0, NoContainer(), pTarget))
    {
      if(ObjectDistance(pTarget)>300) { pTarget=0; break; }
      // neutrale Schlange greift alles an
      if(GetOwner()==-1) if(GetOwner(pTarget)!=-1) break;
      if(Hostile(GetOwner(),GetOwner(pTarget)) || ObjectCount(NTMG)) break;
    }	    
    if(pTarget) SetCommand(this(), "Follow", pTarget);
  }
  
  // Nichts machen
  if (Random(2)) return(1);
  
  // Habe schon was zu tun: Beenden
  if(pTarget) if(ObjectDistance(pTarget)>15)
  if(GetCommand() && Random(4)) return();
  
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
  
  if(GetOwner()!=-1)
  if(!Hostile(GetOwner(),GetOwner(pPrey)) && !ObjectCount(NTMG)) return();
  
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

/* Anti-Sofort-Wieder-Einsammeln */

public func RejectEntrance(pContainer)
{
  // Nur einsammeln lassen, wenn er nicht springt
  // Aber nur von Clonks - Kochtöpfe dürfen beispielsweise immer
  if(GetAction() eq "Jump" || GetAction() eq "Tumble")
   if (GetCategory(pContainer) & C4D_Living())
    return(1);
}

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

 Jump();
 return(1);
}

protected func ContainedDown()
{
 [$TxtMovement$]
 SetCommand(this(), "None");

 if(Contained()) return SetCommand(this, "Exit");

 if (GetAction() eq "Swim")
  return(SetComDir(COMD_Down()));
  
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
