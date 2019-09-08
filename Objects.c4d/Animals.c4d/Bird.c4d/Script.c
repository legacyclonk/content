/*-- Vogel --*/

#strict

#include ANIM

local Bait; // Verfolgter Köder
local pNest;

private func ReproductionRate()     { return(2000); }  // Die Chance, dass in einem Timerintervall eine Vermehrung stattfindet

public func IsPossessible() { return(1); }

/* Initialisierung */

protected func Initialize()
{
  SetAction("Fly");
  if (Random(2)) return(SetComDir(COMD_Right()));
  return(SetComDir(COMD_Left()));
}

/* TimerCall mit KI-Steuerung */

protected func Activity()
{		
  // Geräusche machen
  if(!Random(25)) Sound("Raven*");
  
  // Rückwärtsfliegen? umdrehen
  if(GetXDir() > 0 && GetDir() == DIR_Left)  return(TurnRight());
  if(GetXDir() < 0 && GetDir() == DIR_Right) return(TurnLeft());

  // Die KI-Steuerung wird bei Besessenheit nicht gebraucht
  if(GetEffect("PossessionSpell",this())) return(0);
  if(Contained()) return(0);
  if(GetCommand()) return(0);

  // Du wolltes dich doch vermehren!
  if(FindContents(BIRD)) Reproduction();
  
  // Köder markiert?
  if(Bait) {
    // Bereits im Inventar?
    if(Contained(Bait)) Bait = 0;
    // Zu weit entfernt?
    if(ObjectDistance(Bait, this()) > 300) Bait = 0;
    // In Reichweite?
    if(ObjectDistance(Bait, this()) <= 25)
      // Aufessen!
      if(GetAction() == "Fly")
        SetAction("Eat");
  }

  var pObj, aList;
  // Nur wenn der Vogel ausgewachsen ist
  if(GetCon() == 100)
    // Soll nicht zu einfach sein...
    if(!Random(5)) {
      // Ein Köder in der Nähe?
      aList = FindObjects(Find_Distance(250),Find_NoContainer(),Find_Func("IsBait"));
      for(pObj in aList) {
        // Köder?
        if(!WildcardMatch(GetAction(pObj),"*MeatBait*")) continue;
        // Je nach Köderqualität...
        if(Random(100) >= pObj->~IsBait()) continue;
        // ... hinfliegen
        SetCommand(this(), "Follow", pObj);
        // Und Köder merken
        Bait = pObj;
      }
    }

  // Nichts machen
  if(Random(2) || GetAction() != "Fly") return(0);

  // Nach oben oder unten fliegen
  SetComDir(COMD_Up);
  if(Random(2)) SetComDir(COMD_Down);

  // Nichts machen
  if(!Random(4)) return(0);
  
  // Ein Ei legen
  if(!Random(ReproductionRate())) 
    Reproduction();
  
  // Umdrehen
  if(Random(2)) return(TurnRight());
  return(TurnLeft());
}

// Nur im Fliegen
private func SpecialReprodCond()
{	
  if(Contained()) return();
  if(GetCommand()) return();
  return(GetAction()S="Fly");
}

private func SpecialRepr()
{
//  if(FindContents(BIRD)) return(1);
	
  // Wenn das Nest nicht mehr am Baum hängt wird es verworfen
  if(pNest) if(GetAction(pNest) ne "Be") pNest = 0;
  // oder wenn das Nest geflutet wurde
  if(pNest) if(GBackSemiSolid(GetX(pNest)-GetX(), GetY(pNest)-GetY())) pNest = 0;

  // Kein Nest da? Dann schnell eines bauenn
  if(!pNest)
  {
    var pTree = FindTree();	  
    if(pTree)
    {
      var id = GetID(pTree);	    
      SetCommand(this(), "Call", this(), pTree, 0, 0, "BuildNest");
      // Schönes Plätzchen in der Baumkrone suchen
      AddCommand(this(), "MoveTo", 0, GetX(pTree)+GetDefOffset(id)+Random(GetDefWidth(id)-20)+10, GetY(pTree)+GetDefOffset(id, 1)+Random(GetDefHeight(id)-GetDefFireTop(id)-20)+10);
    }
    // Damit das Vermehrungssteuerungsobjekt schon den neuen Vogel sieht
    // und nicht ständig neue Nester bauen lässt
    if(!FindContents(BIRD)) CreateContents(BIRD);
    return(1);
  }
  
  // Manchmal im Nest ein Ei legen
  if(pNest && !FindObject(BEGG, 0, 0, 0, 0, 0, 0, pNest))
  {
    // Damit das Vermehrungssteuerungsobjekt schon den neuen Vogel sieht
    // und nicht ständig neue Nester bauen lässt
    if(!FindContents(BIRD)) CreateContents(BIRD);
    FlyToNest();	  
  }
  return(1);
}

private func FindTree()
{
   var obj, pTree;
   // Suche einen Baum, der nicht zu weit weg ist, nicht brennt und nicht gefällt
   for(obj in FindObjects(Find_Func("IsTree"), Find_Distance(500), 
              Find_Not(Find_OCF(OCF_OnFire)), Find_Func("IsStanding")))
   {
      // Außerdem sollte er ausgewachesen sein
      if(GetCon(obj)<100) continue;
      // nicht unter Wasser
      if(GBackSemiSolid(GetX(obj)-GetX(), GetY(obj)-GetY())) continue;
      // und noch kein Nest haben
      if(FindObject2(Find_ID(NEST), Find_ActionTarget(obj))) continue;
      return(obj);
   }
}

public func BuildNest(pThis, pTree)
{
  if(!pTree) return(0);
  if(GetX(pTree)<GetX()) SetDir(0);
  else SetDir(1);
  pNest = CreateConstruction(NEST, -12+24*GetDir(), +5, -1, 50);
  LocalN("pTree", pNest) = pTree;
  ObjectSetAction(pNest, "Be", pTree);
  SetAction("BuildNest");
  SetComDir(COMD_Stop);
  SetXDir(0);
  SetYDir(0);
}

protected func Build()
{
  // Nest bauen	
  if(!pNest || OnFire(pNest)) return(SetAction("Fly"), Activity());	
  DoCon(10, pNest);
  if(GetCon(pNest)>=100) { SetAction("Fly"); FlyToNest(); }
}

private func FlyToNest()
{
  // Ins Nest fliegen und dort das Brüten anfangen	
  SetCommand(this(), "Call", this(), 0, 0, 0, "StartBreeding");
//  AddCommand(this(), "Enter", pNest);
  AddCommand(this(), "MoveTo", pNest); 
}

protected func StartBreeding()
{
  ForceEnter(pNest);
  // Nest benachrichtigen	
  ObjectSetAction(pNest, "Bird");	
}

protected func LayEgg()
{
  // Ein Ei legen
  var pEgg = CreateObject(BEGG);
  pEgg->SetAction("Nest", pNest);
  SetPosition(GetX(pNest), GetY(pNest)-1, pEgg);
  if(FindContents(BIRD))
    Enter(pEgg, FindContents(BIRD));
  Exit();
  SetAction("Fly");
  Activity();
}

/* Geburt */

public func Birth()
{
  SetAction("Fly");
  if (Random(2)) SetComDir(COMD_Left());
  else SetComDir(COMD_Right());
  return(1);
}

protected func HitCheck()
{
  // Die KI-Steuerung wird bei Besessenheit nicht gebraucht
  if (GetEffect("PossessionSpell", this())) return();

  // Einen Ballon oder ein Luftschiff zum attackieren suchen
  return(Attack());
}

public func Attack(object pAttack)
{
  // Nur während des Fliegens kann gebissen werden
  if (GetAction() ne "Fly") return();

  // Einen Ballon oder ein Luftschiff suchen
  if (!pAttack) pAttack = FindObject(BALN, -20,0,40,30, 0, 0,0, NoContainer());
  if (!pAttack) pAttack = FindObject(BLMP, -30,0,60,40, 0, 0,0, NoContainer());
  if (!pAttack) return();

  // Reinbeißen!
  pAttack->~Poke();
  SetAction("Attack");
  Sound("Raven*");
}

protected func Survive()
{
  // Die KI-Steuerung wird bei Besessenheit nicht gebraucht
  if (GetEffect("PossessionSpell", this())) return();

  // Nicht in Flüssigkeiten oder brennende Objekte fliegen
  if (InLiquid() || GBackLiquid(GetXDir()*2, GetYDir()*2) || GBackLiquid(GetXDir()*3, GetYDir()*3) || FindObject(0, -20 + GetXDir() * 2, -20 + GetYDir() * 2, 40, 40, OCF_OnFire()))
   SetComDir(COMD_Up());
}

/* Kontakte */

protected func ContactLeft()
{
  // Die KI-Steuerung wird bei Besessenheit nicht gebraucht
  if (GetEffect("PossessionSpell", this())) return();
  TurnRight();
  if(!Random(5)) SetComDir(COMD_Right+Random(2)*2-1);
  return(1);
}

protected func ContactRight()
{
  // Die KI-Steuerung wird bei Besessenheit nicht gebraucht
  if (GetEffect("PossessionSpell", this())) return();
  TurnLeft();
  if(!Random(5)) SetComDir(COMD_Right+Random(2)*2-1);
  return(1);
}

protected func ContactTop()
{
  // Die KI-Steuerung wird bei Besessenheit nicht gebraucht
  if (GetEffect("PossessionSpell", this())) return();

  return(SetComDir(COMD_Down()));
}

protected func ContactBottom()
{
  // Die KI-Steuerung wird bei Besessenheit nicht gebraucht
  if (GetEffect("PossessionSpell", this())) return();

  return(SetComDir(COMD_Up()));
}

/* Aktionen */

public func TurnRight()
{
  if (Stuck() || GetAction() ne "Fly") return();
  if (GetXDir() < 0) SetXDir(0);
  SetDir(DIR_Right());
  SetComDir(COMD_Right()); 
  return(1);
}

public func TurnLeft()
{
  if (Stuck() || GetAction() ne "Fly") return();
  if (GetXDir() > 0) SetXDir(0);
  SetDir(DIR_Left());
  SetComDir(COMD_Left()); 
  return(1);
}

/* Einwirkungen */

protected func CatchBlow(iLevel, pObject)
{
  if (pObject->~IsArrow()) SetAction("Tumble");
  if (!Random(3)) Sound("RavenHurt");
  return(1);
}

protected func Death()
{
  if(FindContents(BIRD)) RemoveObject(FindContents(BIRD));	
  ChangeDef(DBRD);
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
  if(!GetPlrJumpAndRunControl(caller->GetController()))
    SetComDir(COMD_Up());
  return(1);
}

protected func ContainedDown(object caller)
{
  [$TxtMovement$]
  SetCommand(this(), "None");
  if(Contained()) SetCommand(this, "Exit");
  if(!GetPlrJumpAndRunControl(caller->GetController()))
    SetComDir(COMD_Down());
  return(1);
}

/* JumpAndRun-Steuerung */

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

protected func ContainedUpdate(object controller, int comdir)
{
  SetComDir(comdir);
  ClearScheduleCall(this(), "ClearDir");
  if(comdir == COMD_Up || comdir == COMD_Down || comdir == COMD_Stop)
    ScheduleCall(this(), "ClearDir", 1, (Abs(GetXDir())+1)/2, true);
  if(comdir == COMD_Left || comdir == COMD_Right || comdir == COMD_Stop)
    ScheduleCall(this(), "ClearDir", 1, (Abs(GetYDir())+1)/2, false);

  if(comdir == COMD_UpRight || comdir == COMD_Right || comdir == COMD_DownRight)
    SetDir(DIR_Right);
  if(comdir == COMD_UpLeft || comdir == COMD_Left || comdir == COMD_DownLeft)
    SetDir(DIR_Left);

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

/* Köder fressen */

protected func Eating()
{
  // Köder weg
  if(!Bait) return();
  // Köder fressen
  RemoveObject(Bait);
  // Schmeckt gut
  Sound("Raven*");
}
