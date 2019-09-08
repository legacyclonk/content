/*-- Todeskrähe --*/

#strict 2

#include BIRD

public func SearchRange() { return(300); }

local pTarget;

/* TimerCall mit KI-Steuerung */

protected func Activity()
{
  // Geräusche machen
  if(!Random(25)) Sound("Raven*");

  // Rückwärtsfliegen? umdrehen
  if(!pTarget) {
    if(GetXDir() > 0 && GetDir() == DIR_Left)  return(TurnRight());
    if(GetXDir() < 0 && GetDir() == DIR_Right) return(TurnLeft());
  }

  // Die KI-Steuerung wird bei Besessenheit nicht gebraucht
  if(GetEffect("PossessionSpell",this()) || Contained()) {
    if(pTarget) return(ClearTarget());
    return(0);
  }
//  if(Contained()) return(0);
//  if(GetCommand()) return(0);

  // Gegner suchen
  if(!pTarget || !Random(3)) CheckTarget();

  // Beute ist nah genug bzw. Angriff ist bereit: Beißen
  if(Attack(pTarget)) return(1);

  var iFromSide;

  // Ziel vorhanden?
  if(pTarget) {
    // Ist der Gegner unerreichbar oder tot?
    if(Contained(pTarget) || (!GetAlive(pTarget) && !pTarget->~IsBait() && !pTarget->~IsBaitMeat())
    || ObjectDistance(pTarget) > SearchRange())
      return(ClearTarget());
    // Im Wasser gibt die Krähe früher auf
    if(InLiquid(pTarget) && ObjectDistance(pTarget) > SearchRange()/3) return(ClearTarget());
    // Noch in Reichweite: Zu ihm fliegen
    SetCommand(this(),"MoveTo",0,GetX(pTarget)+13*iFromSide,GetY(pTarget)-GetObjHeight(pTarget)/2+4);
    iFromSide = BoundBy(GetX(pTarget)-GetX(),-1,1);
    if((iFromSide ==  1) && (GetDir() == 0)) return(TurnRight());
    if((iFromSide == -1) && (GetDir() == 1)) return(TurnLeft());
    return(1);
  }

  // Nicht zu hoch fliegen
  if(GetY()<0) SetComDir(COMD_Down);

  // Nichts machen
  if(Random(2) || GetAction() != "Fly") return(1);

  // Nach oben oder unten fliegen
  SetComDir(COMD_Up);
  if(Random(2)) SetComDir(COMD_Down);

  // Nichts machen
  if(!Random(4)) return(1);

  // Umdrehen
  if(Random(2)) return(TurnRight());
  return(TurnLeft());
}

public func ClearTarget()
{
  pTarget = 0;
  SetCommand(this(), "None");
  return(1);
}

public func CheckTarget()
{
  var pObj, iRange;
  iRange = SearchRange();
  // Ist bereits ein Ziel vorhanden?
  if(pTarget)
    // Clonks schmecken besser
    if(GetOCF(pTarget) & OCF_CrewMember)
      iRange = ObjectDistance(pTarget)-50;
  if(iRange < 50) return(0);
  pObj = FindObject2(Sort_Distance(),Find_OCF(OCF_CrewMember),Find_NoContainer(),
	   Find_Or(Find_And(Find_Distance(iRange),Find_Not(Find_OCF(OCF_InLiquid))),Find_Distance(iRange/3)),
	   Find_PathFree(this()));
  // Wurde ein näheres Ziel gefunden? Wechseln!
  if(pObj) return(pTarget = pObj);
  // Kein Ziel? Vllt. aber schmackhaftes Fleisch oder ein Köder?
  pObj = FindObject2(Sort_Distance(),Find_NoContainer(),
	   Find_Or(Find_Func("IsBait"),Find_Func("IsBaitMeat")),
	   Find_Or(Find_And(Find_Distance(iRange),Find_Not(Find_OCF(OCF_InLiquid))),Find_Distance(iRange/3)),
	   Find_PathFree(this()));
  // Wurde ein guter Köder gefunden? Wechseln!
  if(pObj) return(pTarget = pObj);
  // Auch nicht? Gegebenfalls nach Beutelebewesen suchen.
  if(!Random(10))
  pObj = FindObject2(Sort_Distance(),Find_OCF(OCF_Prey),Find_NoContainer(),
	   Find_Or(Find_And(Find_Distance(iRange),Find_Not(Find_OCF(OCF_InLiquid))),Find_Distance(iRange/3)),
	   Find_PathFree(this()));
  // Wurde ein Ziel gefunden? Wechseln!
  if(pObj) return(pTarget = pObj);
  // Leider kein Erfolg...
  return(0);
}

public func Attack(object pAttack)
{
  // Während des Fliegens kann alle 2 Sekunden einmal gebissen werden
  if(GetEffect("IntAttackDelay",this()) || GetAction() != "Fly") return(0);

  var iX = 14*(GetDir()*2-1);

  // Ziel bevorzugen
  if(pAttack) if(!FindObject2(Find_Not(Find_Exclude(pAttack)),Find_AtPoint(iX,0),Find_NoContainer())) pAttack = 0;
  // Kein Ziel Vorhanden?
  if(!pAttack)
    // Beute suchen
    pAttack = FindObject2(Sort_Distance(),Find_AtPoint(iX,0),Find_OCF(OCF_Prey),Find_NoContainer());
  // Keine Beute vorhanden? Dann fliegende Objekte angreifen
  if(!pAttack)
    // Einen Ballon oder Luftschiff suchen
    pAttack = FindObject2(Find_NoContainer(),Find_Or(Find_And(Find_ID(BALN),Find_Distance(35,iX,9)),
		Find_And(Find_ID(BLMP),Find_InRect(-30-iX,12,60,40))));
  // Noch keine Beute vorhanden? Vllt. ein Köder?
  if(!pAttack)
    pAttack = FindObject2(Sort_Distance(),Find_AtPoint(iX,0),Find_Or(Find_Func("IsBait"),Find_Func("IsBaitMeat")),
		Find_NoContainer());
  // Immernoch keine Beute? Nicht beißen
  if(!pAttack) return(0);

  // Ist es ein Lebewesen?
  if(GetAlive(pAttack)) {
    // Schaden machen und schleudern	  
    Punch(pAttack);
    Fling(pAttack,GetDir()*2-1,-1);
    if(!pTarget) pTarget = pAttack;
/*
    if(GetX(pAttack) < GetX(this()))
      Fling(pAttack, -1, -1);
    else
      Fling(pAttack, 1, -1);
*/
  }
  // Vllt. ein Köder
  if(pAttack->~IsBait() || pAttack->~IsBaitMeat()) RemoveObject(pAttack);
  // Ansonsten: Reinbeißen!
  else pAttack->~Poke();
  SetAction("Attack");
  Sound("Raven*");
  // Abklingzeit
  AddEffect("IntAttackDelay",this(),1,70);
  return(1);
}

protected func ContainedDig()
{
  [$TxtAttack$]
  Attack();
  return(1);
}

// Um nicht aufeinmal die Besitznahme abzubrechen
protected func ContainedThrow() {
  [$TxtAttack$]
  Attack();
  return(1);
}

protected func Death()
{
  ChangeDef(DBBR);
  return(1);
}