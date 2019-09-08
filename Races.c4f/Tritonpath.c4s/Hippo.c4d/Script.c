/*-- Seepferdchen --*/

#strict
#include ANIM

local iGait; // Gangart: 0 Stehen, 1 Gehen, 2 Trab, 3 Galopp
// entspricht dem Originalfisch außer ~Edible=1 in der DefCore und 
local idSaddle;

private func ID_Unsaddled() { return(HORS); }
private func ID_Dead()      { return(DHRS); }

public func IsSaddled() { return(idSaddle); }

public func Unsaddle()
{
  // Nicht gesattelt
  if (!IsSaddled()) return(0);
  // Nur ohne Reiter
  if (GetRider()) return(0);
  // Erst Satteltasche leeren
  if (Contents()) { Sound("Error"); Message("$MsgEmptySaddle$",this()); return(0); }
  // Sattel abnehmen
  CreateObject(idSaddle,0,+8,GetOwner()); idSaddle=0;
  Sound("Connect");
  Redefine(ID_Unsaddled());  
  return(1);
}

public func Redefine(idDef)
{
  ChangeDef(idDef);
  SetAction("Swim");
  SetGait(0);
  return(1);
}

public func Saddle(idSaddledHorse, idnSaddle)
{
  // Bereits gesattelt
  if (IsSaddled()) return(0);
  // Satteln: nur im Stehen, Gehen, Ziehen
  if (GetAction() ne "Swim" && GetAction() ne "Stand" && GetAction() ne "Pull") 
    return(0);
  // Nur ohne Reiter
  if (GetRider()) return(0);
  // Satteltyp speichern
  idSaddle = idnSaddle;
  // Sound
  Sound("Connect");
  // Satteln
  Redefine(idSaddledHorse);
  return(1);
}

/* Reiten */

protected func ActivateEntrance(pRider)
{
  // Nur ein Reiter zur Zeit
  if (GetRider()) return(0);
  // Nur im Stehen, Gehen, Schwimmen, Ziehen
//  if (GetAction() ne "Walk" && GetAction() ne "Stand" && GetAction() ne "Swim" && GetAction() ne "Pull") 
//    return(0);
  // Reiter kann nur mit Sattel reiten
  if (pRider->~NeedSaddle())
    if (!IsSaddled())
    {
      Message("$MsgNeedSaddle$",pRider,GetName(pRider));
      return(0);
    }
  // Aufsitzen
  if (!ObjectSetAction(pRider,"Ride",this()))
  {
    //NoGoodRider(pRider);
    return(0);
  }
  // Clonkposition anpassen
  SetPosition(GetX()+5-10*GetDir(), GetY()-13, pRider);
  ResetRiderPos();
  SetObjectOrder(pRider, 0, 1);
  // Erfolg
  return(1);
}

public func GetRider()
{
  var pRider;
  // Could we perhaps just use FindObject(0, 0,0,0,0, 0, 0,this()) with no Action and just the ActionTarget specified...?
  if (pRider = FindObject(0, 0,0,0,0, 0, "RideStill",this())) return(pRider);
  if (pRider = FindObject(0, 0,0,0,0, 0, "RideThrow",this())) return(pRider);
  return(FindObject(0, 0,0,0,0, 0, "Ride",this()));
}

/* Steuerung */

public func ControlLeft(pByObject)
{ 
  [$CtrlLeadDesc$]
  // Direkte Steuerung löscht Kommandos
  ClearCommands();
  if(!iGait) SetGait(1);
  // Gehen/Stehen/Ziehen: Wenden
//  if (iGait <= 1)
    if (GetDir() == DIR_Right())
      { TurnLeft(); return(1); }
  // Beschleunigen   
  if(iGait < 3)
    if(GetDir() == DIR_Left() && (pByObject == GetRider() || iGait == 0 || GetActionTarget()))
      { SetGait(BoundBy(iGait + 1, 0, 3)); return(1); }
  // Abbremsen
//  if(GetDir() == DIR_Right())
//    { SetGait(BoundBy(iGait - 1, 0, 3)); return(1); }
  // Keine Steuerung 
  return(0);
}

public func ControlDig(pByObject)
{

	// Abbremsen
    SetGait(BoundBy(iGait - 1, 0, 3)); 
    return(1);
}

public func ControlDigDouble(pByObject)
{
    // Anhalten
    SetGait(0); 
    return(1);
}

public func ControlRight(pByObject)
{
  [$CtrlLeadDesc$]
  // Direkte Steuerung löscht Kommandos
  ClearCommands();
  if(!iGait) SetGait(1);
  // Gehen/Stehen/Ziehen: Wenden
//  if (iGait <= 1)
    if (GetDir() == DIR_Left())
      { TurnRight(); return(1); }
  // Beschleunigen
  if(iGait < 3)
    if(GetDir() == DIR_Right() && (pByObject == GetRider() || iGait == 0 || GetActionTarget()))
      { SetGait(BoundBy(iGait + 1, 0, 3)); return(1); }
/*  // Abbremsen
  if(GetDir() == DIR_Left())
    { SetGait(BoundBy(iGait - 1, 0, 3)); return(1); } */
  // Keine Steuerung 
  return(0);
}

public func ControlUp(pByObject)
{
  [$CtrlJumpDesc$]
// Direkte Steuerung löscht Kommandos
  ClearCommands();
  if(!iGait) SetGait(1);
  SetComDir(COMD_Up());
  // Kein Sprung
  return(0);              
}

public func ControlDown(pByObject)
{
  [$CtrlStopDesc$]
  // Direkte Steuerung löscht Kommandos
  ClearCommands(); 
  if(!iGait) SetGait(1);
  SetComDir(COMD_Down());
  // Keine Steuerung
  return(0);
}

public func ControlDownDouble(pByObject)
{
  [$CtrlGetOffDesc$]
  // Direkte Steuerung löscht Kommandos
  ClearCommands();
  // Reiter: absitzen
  if (pByObject == GetRider())
    { ObjectSetAction(pByObject,"Walk"); }
  SetGait(1);
  // Keine Steuerung
  return(1);
}


public func ControlCommand(szCommand, pTarget, iTx, iTy)
{
  // Kommando MoveTo: gegebenenfalls wenden
  if (szCommand eq "MoveTo")
    if (iTx > GetX())
      TurnRight();
  if (szCommand eq "MoveTo")
    if (iTx < GetX())
      TurnLeft();
  // Kommando setzen
  return(SetCommand(this(), szCommand, pTarget, iTx, iTy));
}

// Reiter-Position zurücksetzen
private func ResetRiderPos()
{
	if(!GetRider()) { SetVertex(0, 0, 0); SetVertex(0, 1, 0); return();}
// Während der Fahrt
  var iPos = 5;//-24;
  var iYPos = -13;
  // Während der Wende
  if (GetAction() eq "Turn") iPos = -5 + 1 * GetPhase();
  // Entsprechend der Richtung
  if (GetDir() == DIR_Right()) iPos = -iPos;
  // Reiter-Vertex anpassen
  SetVertex(0, 0, iPos); SetVertex(0, 1, iYPos);
  var pRider;
  if(pRider = GetRider()) SetDir(GetDir(), pRider);
}

private func ClearCommands()
{
  SetCommand(this(), "None");
  return(1);
}

private func Gallopping()
{
  // Geschwindigkeit und Geräusch
  SetPhysical("Swim",200000,2);
  // Angehalten (z.B. durch Kollision)
  if (GetComDir() == COMD_Stop()) { iGait = 0; SetAction("Stand"); }
  return(1);
}

private func Trotting()
{
  // Geschwindigkeit und Geräusch
  SetPhysical("Swim", 150000, 2);
  // Angehalten (z.B. durch Kollision)
  if (GetComDir() == COMD_Stop()) 
    { iGait = 0; SetAction("Stand"); }
  return(1);     
}

private func Walking()
{                            
  // Reiter-Position zurücksetzen
  ResetRiderPos();
  // Vorherige Gangart wieder aufnehmen (nach Sprung oder Wendung)
  if (iGait != 1 || GetActionTarget()) 
    { SetGait(iGait); return(); }
  // Geschwindigkeit und Geräusch
  SetPhysical("Swim",100000,2);
  return(1);
}

private func SetGait(inGait)
{                                
  iGait = inGait;
  if(GetAction() ne "Stand" && GetAction() ne "Swim" && GetAction() ne "Trot" && GetAction() ne "Gallop" &&
     GetAction() ne "Pull" && GetAction() ne "Pull2" && GetAction() ne "Pull3")
    return();
  // Neue Gangart              
  if(inGait == 0) { SetAction("Stand"); }
  if(inGait == 1) { SetAction("Swim"); }
  if(inGait == 2) { SetAction("Trot"); }
  if(inGait == 3) { SetAction("Gallop"); }
  if(inGait == 0) 
    SetComDir(COMD_Stop());
  else
    if(GetDir() == DIR_Left()) SetComDir(COMD_Left()); else SetComDir(COMD_Right());
}


private func MoveToAdjustAction()
{
  // Stehen
  var inGait = -1;
  if (Abs(Distance(GetX(),GetY(),GetCommand(0,2),GetCommand(0,3))) < 10) 
    inGait = 0;
  // Losgehen
  if (Inside(Abs(Distance(GetX(),GetY(),GetCommand(0,2),GetCommand(0,3))), 10, 75))
    inGait = 1;
  // Traben
  if (Inside(Abs(Distance(GetX(),GetY(),GetCommand(0,2),GetCommand(0,3))), 75, 150))
    inGait = 2;
  // Galoppieren
  if (Abs(Distance(GetX(),GetY(),GetCommand(0,2),GetCommand(0,3))) > 150)
    inGait = 3;
  if(inGait > iGait) SetGait(iGait + 1);
  if(inGait < iGait) SetGait(iGait - 1);
  return(0);                      
}


// bis hier

/* Initialisierung */

protected func Construction() 
{
  var pEnv;
  if (pEnv=FindObject(CLFS))
{
	var rnd = RandomX(245, 150);
	SetClrModulation(RGB(rnd+RandomX(10,0), rnd+RandomX(10,0), rnd+RandomX(10,0)));
}
  else SetColorDw(RGB(255,255,255));
  SetAction("Swim");
  SetComDir(COMD_Left());
  if(Random(2)) SetComDir(COMD_Right());
  SetGait(1);
}

/* TimerCall */

private func Activity() 
{
  if(GetRider())
{ 
// Angehalten: Stehen
  if (GetComDir() == COMD_Stop())
    if (!GetXDir())
      if (GetAction() eq "Walk" || GetAction() eq "Trot" || GetAction() eq "Gallop")
        SetAction("Stand");
  // Bewegungskommando: Aktion anpassen
  if (GetCommand() eq "MoveTo")
    MoveToAdjustAction();
  // Ggf. Reiterposition anpassen
  if(GetAction() ne "Jump")
    ResetRiderPos();   
  // Pferd steht?
  if(GetAction() eq "Stand"){ SetGait(iGait); SetYDir(15);}
return();
}
  if (GetAction() eq "Walk") WalkDir();
  if(iGait!=1) SetGait(1);  
  
  var pBait;
  // Nur wenn der Fisch ausgewachsen ist
  if (GetCon()==100) 
   // Soll nicht zu einfach sein...
   if (!Random(8))
     // Ein Köder in der Nähe?
     if (pBait = FindObject( 0, -250,-250,500,500, OCF_InLiquid(), "Bait"))
       // Je nach Köderqualität...
      if (Random(100) < pBait->~IsBait())
         // ...hinschwimmen
         SetCommand(this(), "Follow", pBait);
  
  // Fortpflanzung (sollte eigentlich laichen...)
  if (!Random(ReproductionRate())) 
    Reproduction();
  
  // Vor Bedrohungen flüchten
  var threat, found_threat;
  if (!Contained() && InLiquid()) 
  {
    while (threat = FindObject(0, -1, -1, -1, -1, 0, 0, 0, 0, threat)) 
    {
      if (ObjectDistance(this(), threat) > 100) break;
      // Bedrohung bewegt sich nicht?
      if (Inside(GetXDir(threat), -2, +2) && Inside(GetYDir(threat), -3, +3)) continue;
      // Kontakt zum Boden?
      if (GetContact(threat, -1, 8)) continue;
      // Bedrohung nicht im Wasser oder in einem Container -> Nächstes Objekt
      if (!InLiquid(threat) || Contained(threat)) continue;
      // Keine Regelobjekte, Luftblasen etc.
      var cat = GetCategory(threat);
      if (!(cat & C4D_Living()   || cat & C4D_Object() ||
          cat & C4D_Vehicle()  || cat & C4D_Structure())) continue;
      if (cat & C4D_Background() || cat & C4D_Parallax())   continue;
      if (GetID(threat) == FXU1) continue;
      // Keine unsichtbaren Objekte
      if (GetVisibility(threat)) continue;
      // Keine toten Clonks
      if (threat->~IsClonk() && !GetAlive(threat)) continue;
      // Sind ziemlich naiv und schwimmen nicht vor anderen Tieren weg
      if (!GetAlive(threat) || threat->~IsClonk()) {
        found_threat = true;
        var xdist = GetX(threat) - GetX();
        var ydist = GetY(threat) - GetY();
        var axdist = Abs(xdist);
        var aydist = Abs(ydist);
        var xsign = xdist / axdist;
        var ysign = ydist / aydist;
        var fleex = GetX() - xsign * (1500 / BoundBy(axdist, 20, 80)); // 20..80 -> 70..15
        var fleey = GetY() - ysign * (1000 / BoundBy(aydist, 20, 80)); // 20..80 -> 50..10
        SetCommand(this(), "MoveTo", 0, fleex, fleey, 0, true);
      }
    }
  }

  // Bewegung zum Fischturm (hat hoehere Prioritaet als Gefahren)
  var fish_tower = FindObject(FSTW, -250, -250, 500, 500, OCF_Fullcon() );
  // nicht das U-Boot steuern wenn darin befindlich :D
  if(!Contained()) 
    // Fischturm gefunden
    if(fish_tower)
    {
      SetCommand(this(), "MoveTo", 0, GetX(fish_tower) - 150 + Random(300), GetY(fish_tower) - 150 + Random(300) );
      found_threat = true;
    }
  
  // Wenn keine Bedrohung mehr oder nicht im Wasser aufhören zu fliehen
  if (!InLiquid() || Contained() || (!found_threat && GetCommand() eq "MoveTo")) 
    SetCommand(this(), "None");
  
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

/* Aktionen von außerhalb */

public func Eat(pByObject)
{
  pByObject->~Feed(50);
  RemoveObject();
  return(1);
}
  
public func Kill()
{
  DoEnergy(-100);
}
 
public func Cook()
{
  ChangeDef(CHIC);
  SetAlive(0);
  return(1);
}

/* Kontakt */

protected func ContactLeft()
{
  if(GetRider()) return(SetGait(0));
  return(TurnRight());
}

protected func ContactRight()
{
  if(GetRider()) return(SetGait(0));
  return(TurnLeft());
}

protected func ContactTop()
{
  if(GetRider()) return(SetGait(0));
  SetComDir(COMD_Down());
  return(1);
}

protected func ContactBottom()
{
  if(GetRider()) return(SetGait(0));
  if (GetAction() ne "Walk") 
  {
    SetComDir(COMD_Up());
    if (Random(3)) return(0);
  }
  if (Random(10)) SetComDir(COMD_Right());
  if (Random(10)) SetComDir(COMD_Left());
  return(1);
}

/* Bewegung */

private func TurnRight()
{
  if(GetAction()S="Turn") return();
if(!GetDir()) SetAction("Turn");
  if (Stuck()) return(0);
  if (GetXDir() < 0) SetXDir(0);
  if (!Random(3)) SetYDir(RandomX(-5, +5));
SetDir(DIR_Right());
  SetComDir(COMD_Right()); 
  return(1);
}

private func TurnLeft()
{
  if(GetAction()S="Turn") return();
if(GetDir()) SetAction("Turn");
  if (Stuck()) return(0);
  if (GetXDir() > 0) SetXDir(0);
  if (!Random(3)) SetYDir(RandomX(-5, +5));
SetDir(DIR_Left());
  SetComDir(COMD_Left()); 
  return(1);
}

private func WalkDir()
{
  SetComDir(COMD_Left());
  if (Random(2)) SetComDir(COMD_Right());
  return(1);
}

/* Aktionen */

protected func Death()
{
  // Inhalt auswerfen
  while (Contents()) Exit(Contents());
  if (GetRider()) DropRider();
  // Gegebenenfalls Sattel verlieren
  Unsaddle();

  SetDir(0);
  ChangeDef(DHIC);
  SetAction("Dead");
  return(1);
}

public func DropRider()
{
  return(Fling(GetRider(), 0, 1));
}

public func Birth()
{
  SetAction("Swim");
  SetComDir(COMD_Left());
  if (Random(2)) SetComDir(COMD_Right());
  return(1);
}

private func SpecialReprodCond()
{
  return(GetAction() eq "Swim");
}

public func Entrance(container) 
{
  // Damit der Fisch nicht aus U-Booten flieht und so.
  SetCommand(this(), "None");
}
  
/* Anti-Sofort-Wieder einsammeln */

public func RejectEntrance(pContainer)
{
  // nur einsammeln lassen wenn er nicht springt
  // Aber nur von Clonks - Kochtöpfe dürfen beispielsweise immer
  if (GetAction() eq "Jump" || GetAction() eq "Walk")
    if (GetCategory(pContainer) & C4D_Living())
      if (Random(10))
        return(1);
}
