/*-- Pferd --*/

#strict

/* Locals */

local iGait; // Gangart: 0 Stehen, 1 Gehen, 2 Trab, 3 Galopp
local idSaddle; // Gesattelt (Satteltyp)
local iGraphics;

/* IDs */

private func ID_Unsaddled() { return(HORS); }
private func ID_Dead()      { return(DHRS); }

private func MaxGait()
{
  if(GetProcedure() S= "PULL") return(2);
  return(3);
}

/* Initialisierung */

protected func Initialize()
{
  // zufälliges Aussehen (schlägt fehl, wenn keine Zusatzgrafik vorhanden)
  iGraphics = Random(3);
  SetGraphics(Format("%d", iGraphics), this(),GetID());

  SetAction("Walk");
  SetDir(DIR_Left());
  if (Random(2)) SetDir(DIR_Right());
  SetComDir(COMD_Stop());
  // Satteltyp initialisieren
  if (GetID(this()) == HRSS) idSaddle = SADL;
  if (GetID(this()) == HRSK) idSaddle = KSDL;
}

public func Redefine(idDef)
{
  var graphics = iGraphics;
  ChangeDef(idDef);
  SetAction("Walk");
  // Grafik anpassen (schlägt fehl, wenn keine Zusatzgrafik vorhanden)
  if(this()->~IsHorse()) SetGraphics(Format("%d", graphics), this(),GetID());
  return(1);
}

/* TimerCall */

private func Activity()
{
  // Locals übertragen (für alte Savegames)
  if(!iGait && Local(0)) { iGait = (Local(0)+1)%5; if(GetComDir() == COMD_None()) iGait = 0; Local(0) = 0; }  
  if(!idSaddle && Local(1)) { idSaddle = Local(1); Local(1) = 0; }
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
  if(GetAction() eq "Stand") SetGait(iGait);
  // Anhalten, wenn ohne Reiter
  return(StopRunning());
}

private func MoveToAdjustAction()
{
  // Stehen
  var inGait = -1;
  var maxGait = MaxGait();

  if (Abs(Distance(GetX(),GetY(),GetCommand(0,2),GetCommand(0,3))) < 10) 
    inGait = Min(maxGait, 0);
  // Losgehen
  if (Inside(Abs(Distance(GetX(),GetY(),GetCommand(0,2),GetCommand(0,3))), 10, 75))
    inGait = Min(maxGait, 1);
  // Traben
  if (Inside(Abs(Distance(GetX(),GetY(),GetCommand(0,2),GetCommand(0,3))), 75, 150))
    inGait = Min(maxGait, 2);
  // Galoppieren
  if (Abs(Distance(GetX(),GetY(),GetCommand(0,2),GetCommand(0,3))) > 150)
    inGait = Min(maxGait, 3);
  if(inGait > iGait) SetGait(iGait + 1);
  if(inGait < iGait) SetGait(iGait - 1);
  return(0);                      
}

private func StopRunning()
{
  // Richtige Aktion?
  if(GetAction() ne "Walk" && GetAction() ne "Trot" && GetAction() ne "Gallop") return();
  if(!iGait) return();
  // Kein Reiter?
  if(GetRider()) return();
  // Keiner der mich schiebt?
  if(GetPusher()) return();
  // diesmal einfach nicht langsamer werden
  if(Random(3)) return();
  // Geschwindigkeit stufenweise verringern
  SetGait(iGait-1);
}

/* Reiten */

protected func ActivateEntrance(pRider)
{
  // Nur ein Reiter zur Zeit
  if (GetRider()) return(0);
  // Nur im Stehen, Gehen, Schwimmen, Ziehen
  if (GetAction() ne "Walk" && GetAction() ne "Stand" && GetAction() ne "Swim" && GetAction() ne "Pull") 
    return(0);
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
    NoGoodRider(pRider);
    return(0);
  }
  // Clonkposition anpassen
  SetPosition(GetX(), GetY()-5, pRider);
  // Erfolg
  return(1);
}

/* Steuerung */

private func SetControl(int com, object controller)
{
  if(GetEffect("IntControl", this()))
    RemoveEffect("IntControl", this());

  AddEffect("IntControl", this(), 1, 9, this(), 0, com, controller);
}

protected func FxIntControlStart(object target, int number, int temp, int com, object controller)
{
  if(temp) return();
  EffectVar(0, target, number) = com;
  EffectVar(1, target, number) = controller;
}

protected func FxIntControlTimer(object target, int number, int time)
{
  // Keine Aenderung waehrend eines Sprungs
  if(GetAction() S= "Jump" || GetAction() S= "Turn") return(0);

  var com = EffectVar(0, target, number);
  var controller = EffectVar(1, target, number);

  var tick = EffectVar(2, target, number);
  EffectVar(2, target, number) = (tick + 1) % 4;

  // Ist kein Controller mehr
  // Controller kann auch auf dem Planwagen sitzen. Hm.
  if(!controller)// || controller != GetRider() || (controller->GetAction() ne "Push" && controller->GetActionTarget() != this()))
    return(-1);

  // Bei normaler Beschleunigung oder Abbremsen nur jeden zweiten Timer
  if(com == COMD_Right && GetComDir() == COMD_Right || com == COMD_Left && GetComDir() == COMD_Left || com == COMD_Stop)
    if(tick & 1)
      return(0);

  // Bei Beschleunigung in Gegenrichtung jeden
  if(com == COMD_Right) ComRight(controller);
  if(com == COMD_Left) ComLeft(controller);
  if(com == COMD_Stop) ComStop(controller);

  // Keine weitere Beschleunigung/Abbremsung noetig
  if(GetAction() S= "Swim") return(-1);
  // Ziel erreicht
  if((com != COMD_Stop && iGait == MaxGait()) || (com == COMD_Stop && iGait == 0))
    return(-1);
}

protected func ComLeft(object by)
{
  // Gehen/Stehen/Ziehen: Wenden
  if(iGait <= 1)
    if(GetDir() == DIR_Right)
      { TurnLeft(); return(true); }

  // Schwimmen: Richtung bestimmen
  if(GetAction() eq "Swim")
    { SetComDir(COMD_Left); return(true); }

  // Beschleunigen
  if(iGait < MaxGait())
    if(GetDir() == DIR_Left && (by == GetRider() || iGait == 0 || GetActionTarget()))
      { SetGait(BoundBy(iGait + 1, 0, MaxGait())); return(true); }

  // Abbremsen
  if(GetDir() == DIR_Right)
    { SetGait(BoundBy(iGait - 1, 0, MaxGait())); return(true); }

  return(false);
}

protected func ComRight(object by)
{
  // Gehen/Stehen/Ziehen: Wenden
  if (iGait <= 1)
    if (GetDir() == DIR_Left)
      { TurnRight(); return(true); }

  // Schwimmen: Richtung bestimmen
  if (GetAction() eq "Swim")
    { SetComDir(COMD_Right); return(true); }

  // Beschleunigen
  if(iGait < MaxGait())
    if(GetDir() == DIR_Right && (by == GetRider() || iGait == 0 || GetActionTarget()))
      { SetGait(BoundBy(iGait + 1, 0, MaxGait())); return(true); }

  // Abbremsen
  if(GetDir() == DIR_Left)
    { SetGait(BoundBy(iGait - 1, 0, MaxGait())); return(true); }

  return(false);
}

protected func ComStop(object by)
{
  // Abbremsen
  if(iGait) { SetGait(iGait - 1); return(true); }
  // TODO: COMD_Stop im Wasser?
  return(false);
}

public func ControlLeft(pByObject)
{ 
  [$CtrlLeadDesc$|Image=HOR1:0]
  // Direkte Steuerung löscht Kommandos
  ClearCommands();
  // Aufzug bedienen
  var pElev = GetElev();
  if(pElev && !iGait) if(GetAction(pElev) ne "Wait") return(pElev->~ControlLeft(pByObject));
  return(ComLeft(pByObject));
}

public func ControlRight(pByObject)
{
  [$CtrlLeadDesc$|Image=HOR1:1]
  // Direkte Steuerung löscht Kommandos
  ClearCommands();
  // Aufzug bedienen
  var pElev = GetElev();
  if(pElev && !iGait) if(GetAction(pElev) ne "Wait") return(pElev->~ControlRight(pByObject));
  return(ComRight(pByObject));
}

public func ControlUpdate(object pByObject, int comdir)
{
  // Weitere Beschleunigung bei JumpAndRun-Steuerung
  SetControl(comdir, pByObject);
}

public func ControlLeftReleased(pByObject)
{
  ComStop();
}

public func ControlRightReleased(pByObject)
{
  ComStop();
}

public func ControlUp(pByObject)
{
  [$CtrlJumpDesc$]
  // Nur im Gallop
  if (GetAction() eq "Gallop") { Jump(); return(); }
  // Aufzug bedienen
  var pElev = GetElev();
  if(pElev && !iGait) pElev->~ControlUp(pByObject);
  // Kein Sprung
  return(0);              
}

public func ControlDownSingle(pByObject)
{
  [$CtrlStopDesc$|Method=Classic]
  if(!GetPlrJumpAndRunControl(pByObject->GetController()))
  {
    // Direkte Steuerung löscht Kommandos
    ClearCommands(); 
    // Aufzug bedienen
    var pElev = GetElev();
    if(pElev && !iGait) pElev->~ControlDownSingle(pByObject);
    // Abbremsen
    if(iGait) SetGait(iGait - 1);
  }
  // Keine Steuerung
  return(0);
}

private func GetElev()
{
  var pElev = FindObject(ELEC, 0, 1);
  if(GetX(pElev)!=GetX()) return(0);
  else return(pElev);
}

public func ControlDownDouble(pByObject)
{
  [$CtrlGetOffDesc$]
  // Direkte Steuerung löscht Kommandos
  ClearCommands();
  // Reiter: absitzen
  if (pByObject == GetRider())
  {
    ObjectSetAction(pByObject,"Walk");
    SetActionTargets(0,0,pByObject);
    var y=GetY(pByObject),i=0;
    //Stuck?
    while (i++<12)
      if (Stuck(pByObject))
        SetPosition(GetX(pByObject), y+i, pByObject);
      else
        break;
    return(1);
  }
  // Keine Steuerung
  return(0);
}

public func ControlDigDouble(pByObject)
{
  [$CtrlFastenDesc$|Image=HOR1:2]
  // Direkte Steuerung löscht Kommandos
  ClearCommands();
  // Angespannt: ausspannen
  if (GetActionTarget())
    { DisconnectWagon(); return(1); }
  // Anspannen/Satteln: nur im Stehen oder Gehen
  if (iGait > 1)
    return(0);
  // Wagen suchen und anspannen
  var pWagon;
  if (pWagon = FindHorseWagon())
    { ConnectWagon(pWagon); return(1); }
  // Sattel abnehmen
  if (IsSaddled())
    if (Unsaddle())
      return(1);
  // Satteln (Sattelobjekt im Aufrufer aktivieren)
  var pSaddle;
  if (pSaddle = FindSaddleIn(pByObject))
    if (pSaddle->Activate(pByObject))
      return(1);
  // Keine Aktion
  return(0);    
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

/* Gangart: Actions */

private func SetGait(inGait)
{                                
  iGait = inGait;
  if(GetAction() ne "Stand" && GetAction() ne "Walk" && GetAction() ne "Trot" && GetAction() ne "Gallop" &&
     GetAction() ne "Pull" && GetAction() ne "Pull2" && GetAction() ne "Pull3"  &&
     GetAction() ne "Scale" && GetAction() ne "Scale2" && GetAction() ne "Scale3")
    return();
  var fScaling;
  if(WildcardMatch(GetAction(), "Scale*")) fScaling = 1;
  // Neue Gangart              
  var fPulling = GetActionTarget();
  if(inGait == 0) { if(fPulling) SetAction("Pull"); else SetAction("Stand"); }
  if(inGait == 1) { if(fPulling) SetAction("Pull"); else if(fScaling) SetAction("Scale"); else SetAction("Walk"); }
  if(inGait == 2) { if(fPulling) SetAction("Pull2"); else if(fScaling) SetAction("Scale2"); else SetAction("Trot"); }
  if(inGait == 3) { if(fPulling) SetAction("Pull3"); else if(fScaling) SetAction("Scale3"); else SetAction("Gallop"); }
  if(inGait == 0) 
    SetComDir(COMD_Stop());
  else
    if(GetDir() == DIR_Left()) SetComDir(COMD_Left()); else SetComDir(COMD_Right());
  if(fPulling) SetPhase(Random(10));
}

/* Anspannen und satteln */

private func DisconnectWagon()
{
  GetActionTarget()->~Disconnect(this());
  SetActionTargets(); SetGait(iGait);
  return(1);
}

private func ConnectWagon(pWagon)
{
  pWagon->Connect(this());
  SetActionTargets(pWagon); SetGait(iGait);
  return(1);
}

public func Saddle(idSaddledHorse, idnSaddle)
{
  // Bereits gesattelt
  if (IsSaddled()) return(0);
  // Satteln: nur im Stehen, Gehen, Ziehen
  if (GetAction() ne "Walk" && GetAction() ne "Stand" && GetAction() ne "Pull") 
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

private func TurnRight()
{
  // Wenden aus Gehen, Stehen, Ziehen
  if (GetDir() == DIR_Left())
    if (GetAction() eq "Walk" || GetAction() eq "Stand" || GetAction() eq "Pull") 
      SetAction("Turn");
  // Richtung
  SetDir(DIR_Right());
  SetComDir(COMD_Right());
  return(1);
}

private func TurnLeft()
{
  // Wenden aus Gehen, Stehen, Ziehen
  if (GetDir() == DIR_Right())
    if (GetAction() eq "Walk" || GetAction() eq "Stand" || GetAction() eq "Pull") 
      SetAction("Turn");
  // Richtung
  SetDir(DIR_Left());
  SetComDir(COMD_Left());
  return(1);
}

private func ClearCommands()
{
  SetCommand(this(), "None");
  return(1);
}

/* Einwirkungen */

protected func Damage(iDamage)
{
  // Starker Treffer
  if (iDamage > 10)
    SetAction("Rise");
  // Tot
  if (GetDamage() > 100)
    Death();
  // Wiehern
  Sound("HorseNeigh*");
  return(1);
}
    
private func Collision()
{
  SetAction("Tumble");
  DoDamage(+5);
  return(1);
}

public func Death()
{
  // Inhalt auswerfen
  while (Contents()) Exit(Contents());
  if (GetRider()) DropRider();
  // Gegebenenfalls Sattel verlieren
  Unsaddle();
  // Verwandeln
  var graphics = iGraphics;
  ChangeDef(ID_Dead());
  SetGraphics(Format("%d", graphics), this(),GetID()); //  (schlägt fehl, wenn keine Zusatzgrafik vorhanden)

  SetAction("Dead");
  return(1);
}

public func DropRider()
{
  return(Fling(GetRider(), 0, 1));
}

private func LiquidDamage()
{
  // Material abfragen
  var iMaterial = GetMaterial();
  if (iMaterial == -1) return(0);
  // Säure/Lava (leider hardgecodet)
  if (iMaterial == Material("Acid") || iMaterial == Material("Lava") || iMaterial == Material("DuroLava"))
    DoDamage(+8);
  return(1);
} 

/* Kontakt */

protected func ContactLeft()
{
  if(GetX()<28) return(TurnRight());
  if(!GBackSolid(-3,0) && !GBackSolid(-8,0))
  {
    SetAction("Scale");
    SetPhysical("CanScale",1,2);
    SetComDir(COMD_Up);
    return(1);
  }
  SetPhysical("CanScale",0,2);
  // Kollision
//  if (!Inside(GetXDir(),-30,+30)) return(Collision());
  // Wenden
  TurnRight(); 
  return(1);
}
  
protected func ContactRight()
{
  if(GetX()>LandscapeWidth()-28) return(TurnLeft());
  if(!GBackSolid(3,0) && !GBackSolid(8,0))
  {
    SetAction("Scale");
    SetPhysical("CanScale",1,2);
    SetComDir(COMD_Up);
    return(1);
  }
  SetPhysical("CanScale",0,2);
  // Kollision
//  if (!Inside(GetXDir(),-30,+30)) return(Collision());
  // Wenden
  TurnLeft(); 
  return(1);
}

/* Aktionen */

private func Pulling()
{
  // Zielobjekt verloren
  if (!GetActionTarget())
    return(DisconnectWagon());
  // Wagen wenden
  if (GetDir(GetActionTarget()) == DIR_Left())
    if (GetX() > GetX(GetActionTarget()))
      GetActionTarget()->TurnRight();
  if (GetDir(GetActionTarget()) == DIR_Right())
    if (GetX() < GetX(GetActionTarget()))
      GetActionTarget()->TurnLeft();
  return(1);  
}

private func Pulling1()
{
  Pulling();
  // Geschwindigkeit und Geräusch
  SetPhysical("Walk",50000,2);
  Sound("HorseWalk*");
  return(1);  
}
  
private func Pulling2()
{
  Pulling();
  // Geschwindigkeit und Geräusch
  SetPhysical("Walk",70000,2);
  Sound("HorseTrot*");
  return(1); 
} 
  
private func Pulling3()
{
  Pulling();
  // Geschwindigkeit und Geräusch
  SetPhysical("Walk",130000,2);
  Sound("HorseGallop*");
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
  SetPhysical("Walk",50000,2);
  SetPhysical("Scale",1000000,2);
  Sound("HorseWalk*");
  return(1);
}
  
private func Turning()
{
  // Geräusch
  Sound("HorseWalk*");
  return(1);  
}
  
private func Trotting()
{
  // Geschwindigkeit und Geräusch
  SetPhysical("Walk", 90000, 2);
  SetPhysical("Scale", 1000000, 2);
  Sound("HorseTrot*");
  // Angehalten (z.B. durch Kollision)
  if (GetComDir() == COMD_Stop()) 
    { iGait = 0; SetAction("Stand"); }
  return(1);     
}
  
private func Gallopping()
{
  // Geschwindigkeit und Geräusch
  SetPhysical("Walk",160000,2);
  SetPhysical("Scale",1000000,2);
  Sound("HorseGallop*");
  // Angehalten (z.B. durch Kollision)
  if (GetComDir() == COMD_Stop()) { iGait = 0; SetAction("Stand"); }
  return(1);
}
  
private func Swimming()
{
  // Unter Wasser: auftauchen
  if (GBackSemiSolid(0, -5)) 
    SetComDir(COMD_Up());
  // Gangart zurücksetzen
  iGait = 1;
  // Geräusch
  Sound("HorseSplash*");
  // Schaden durch gefährliche Flüssigkeiten
  LiquidDamage();
  return(1);  
}
  
private func Tumbling()
{
  // Reiter abwerfen 
  Fling(GetRider(),Random(5)-2,-3);
  return(1);
}
  
private func Rising()
{
  // Reiter abwerfen 
  Fling(GetRider(),Random(5)-2,-3);
  return(1);
}

private func Jumping()
{
  if(GetPhase() == 1) SetVertex(0, 1, -6);
  if(GetPhase() == 5) { SetVertex(0, 1, -7); if(GetDir() == DIR_Right()) SetVertex(0, 0, 1); else SetVertex(0, 0, -1); }
}

// Reiter-Position zurücksetzen
private func ResetRiderPos()
{
  SetVertex(0, 0, 0); SetVertex(0, 1, -5); 
}

/* Status */

public func IsHorse() { return(1); }

public func GetRider()
{
  var pRider;
  if(pRider = FindObject2(Find_OCF(OCF_CrewMember), Find_ActionTarget(this), Find_Func("IsRiding")))
     return(pRider);
}

public func GetPusher()
{
  var pRider;
  if(pRider = FindObject(0, 0,0,0,0, OCF_CrewMember(), "Push", this()))
    return(pRider);
}

public func GetGait()
{
  return(iGait);
}
  
private func NoGoodRider(pRider)
{
  if (GetOCF(pRider) & OCF_CrewMember())
    Message("$MsgNoGoodRider$",pRider,GetName(pRider));
  return(0);
}
  
private func FindHorseWagon()
{
  var obj;
  while (obj = FindObject(0, -50,-20,100,40, 0, 0,0, NoContainer(), obj))
    if (obj->~IsHorseWagon())
      return(obj);
  return(0);
}
  
public func IsStill() { return(iGait == 0); }

public func IsSaddled() { return(idSaddle); }

public func FindSaddleIn(pObj)
{
  for(var i = 0, obj; obj = Contents(i, pObj); i++)
    if(obj->~IsSaddle())
      return(obj);
  return(0);
}

protected func CheckStuck()
{                   
  // Verhindert Festhängen
  if(!GetXDir()) if(Abs(GetYDir()) < 5)
    if(Stuck())
      SetPosition(GetX(), GetY() + 1);
  if(GetElev()) SetAction("Stand");  
}
