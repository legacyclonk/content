/*-- Wildpferd --*/

#strict

#include ANIM

/* Locals */

local iGait; // Gangart: 0 Stehen, 1 Gehen, 2 Trab, 3 Galopp
local idSaddle; // Gesattelt (Satteltyp)
local iGraphics;
local iWild; // Gezähmtheit: 15 wild 0 gezähmt

/* IDs */

private func ID_Tamed()     { return(HORS); }
private func ID_Dead()      { return(DHRS); }
private func ID_Saddled() { return(HRSS); }

/* Initialisierung */

protected func Initialize()
{                         
  // Tut es schon was? dann wurde es wahrscheinlich geboren und brauch kein Initialize() mehr	
  if(!ActIdle()) return();
  SetAction("Walk");
  SetDir(DIR_Left());
  if (Random(2)) SetDir(DIR_Right());
  SetComDir(COMD_Stop());
  SetLocal(0,0);
  // Farbe
  iGraphics = Random(3);
  // Das Wildpferd verwendet die Grafiken vom normalen Pferd
  if(iGraphics) SetGraphics(Format("%d",iGraphics), this(), ID_Tamed());
  else SetGraphics("", this(), ID_Tamed());
}

public func Redefine(idDef)
{
  ChangeDef(idDef);
  SetAction("Walk");
  return(1);
}

/* TimerCall */

private func Activity()
{
  var pEnemy;
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

  if(GetRider()) // Pferd wird wild
  {
    // Nichts machen
    if (GetAction() ne "Walk" && GetAction() ne "Gallop" && GetAction() ne "Stand" && GetAction() ne "Swim") return(1);
    if (!Random(3)) return(SetAction("Rise"));
    // Umdrehen
    if (Random(2)) return(TurnRight());	  
    SetGait(3);
    if (!Random(3)) return(Jump());
    return(0);	  
  }

  if(idSaddle)
  {
    CreateObject(idSaddle,0,+8,GetOwner());
    idSaddle=0;
    if(iGraphics) SetGraphics(Format("%d",iGraphics), this(), ID_Tamed());
    else SetGraphics("", this(), ID_Tamed());
    return(SetAction("Rise"));
  }
  
  if(GetAction() S= "Grazing" && !Random(4)) SetAction("StopGrazing");
  
  // Nichts machen
  if (Random(2) || (GetAction() ne "Walk" && GetAction() ne "Gallop" && GetAction() ne "Stand" && GetAction() ne "Swim")) return(1);

  // Wieder wild werden  
  if(!Random(3)) iWild = Min(iWild+1, 15);
  
  SetGait(1);
  
  // Nachwuschs
//  if (!Random(ReproductionRate())) 
//   Reproduction();	

  // Feind in Sicht: abhauen
  if(pEnemy=EnemyNearby())
    return(RunAway(pEnemy));
  
  // Springen
   if (!Random(3)) return(Jump());

  // Umsehen
  if (GetAction() eq "Walk")
   if (!Random(3)) return(SetAction("StartGrazing")); // Hier könnte man die Aktion Eat einbauen, wenn die Grafik dafür da ist

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
  // Nahrungssuche einstellen
  if (GetCommand()eq"Follow") SetCommand(this(),"None");
  // In die entgegengesetzte Richtung
  if(GetX(pEnemy)>GetX()) TurnLeft();  
  if(GetX(pEnemy)<GetX()) TurnRight();
  // bei drohender Gefahr: weg galoppieren
  if(ObjectDistance(pEnemy)<150)
  {
    // Bewegungskommando
    SetCommand(this(),"None");
    AddCommand(this(),"MoveTo",0,GetX()+100*(GetDir()*2-1),GetY(),0,20);
    // Galoppieren
    SetGait(3);
    Sound("HorseNeigh*");
  }
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

/* Vermehrung */

public func Reproduction()
{
  // Nicht mehr am Leben
  if (!GetAlive()) return(0);
  // Noch nicht ausgewachsen
  if (GetCon() < 100) return(0);
  // Spezielle Anforderung nicht erfüllt
  if (!SpecialReprodCond()) return(0);
  // Schon zu viele Tiere dieser Art
  if (CountMe() >= MaxAnimalCount()) return(0);
  // Reproduktion
  if (!SpecialRepr()) 
  {
    // Normale Reproduktion
    var pChild = CreateConstruction(GetID(this()), 0, 0, -1, 40);
    pChild->~Birth();
    LocalN("iGraphics", pChild) = iGraphics;
    if(iGraphics) SetGraphics(Format("%d",iGraphics), pChild, ID_Tamed());
    else SetGraphics("", pChild, ID_Tamed());
  }
  // Erfolg
  return(1);
}

/* Reiten */

protected func ActivateEntrance(pRider)
{
  // Nur ein Reiter zur Zeit
  if (GetRider()) return(0);
  // Nur im Stehen, Galopp, Gehen, Schwimmen
//  if (GetAction() ne "Walk" && GetAction() ne "Gallop" && GetAction() ne "Stand" && GetAction() ne "Swim") 
//    return(0);
  // Reiter kann nur mit Sattel reiten
  if (pRider->~NeedSaddle())
    {
      // Kein guter Reiter, kann das Wildpferd nicht zähmen
      // außer er hat einen Sattel dabei
      if(FindContents(SADL, pRider))
      {	 
	 idSaddle = SADL;
	 if(iGraphics) SetGraphics(Format("%d",iGraphics), this(), ID_Saddled());
         else SetGraphics("", this(), ID_Saddled());
	 RemoveObject(FindContents(SADL, pRider));
      }
      else return(0, Message("$MsgNeedSaddle$",pRider,GetName(pRider)));
    }
  // Aufsitzen
  if (!ObjectSetAction(pRider,"Ride",this()))
  {
    // Kann nicht reiten	  
    return(0);
  }
  // Clonkposition anpassen
  SetPosition(GetX(), GetY()-5, pRider);
  SetObjectOrder(this(), pRider);
  // gleich das Gallopieren anfangen und aufbäumen
  SetGait(3);
  SetAction("Rise");
  // Erfolg
  return(1);
}

/* Steuerung */

public func ControlLeft(pByObject)
{ 
  [$CtrlCalmDesc$]
  // Beruhigen
  if(GetDir() == DIR_Right())
    Calm();
  // Keine Steuerung 
  return(0);
}

public func ControlRight(pByObject)
{
  [$CtrlCalmDesc$]
  // Beruhigen
  if(GetDir() == DIR_Left())
    Calm();
  // Keine Steuerung 
  return(0);
}

public func Calm()
{
  // Blockereffekt, damit das zähmen nicht zu schnell geht	
  if(GetEffect("WaitCalm", this())) return();	
  // Zähmen
  if(!iWild--)
  {
    Tame(); // Ferig!
    return(1);
  }
  AddEffect("WaitCalm", this(), 1, 20);
  Sound("HorseNeigh*");
}

public func Tame()
{
  // Aktion speichern und ID-Wechsel	
  var szAction = GetAction(), iPhase = GetPhase(), iCol = iGraphics;
  // Vorbereiten auf den Wechsel zu einem Vehicle
  SetCategory(GetCategory(0,ID_Tamed()));
  SetAlive(0);
  DoEnergy(GetEnergy());
  // Zur Passenden ID wechseln
  if(!idSaddle) ChangeDef(ID_Tamed());
  else ChangeDef(ID_Saddled());
  // Aktionen
  ObjectSetAction(this(), szAction);
  SetPhase(iPhase, this());
  // Farbe anpassen
  SetGraphics(Format("%d",iCol)); 
}

public func ControlDownDouble(pByObject)
{
  [$CtrlGetOffDesc$]
  // Reiter: absitzen
  if (pByObject == GetRider())
    { ObjectSetAction(pByObject,"Walk"); return(1); }
  // Keine Steuerung
  return(0);
}

/* Gangart: Actions */

private func SetGait(inGait)
{                                
  iGait = inGait;
  if(GetAction() ne "Stand" && GetAction() ne "Walk" && GetAction() ne "Trot" && GetAction() ne "Gallop" &&
     GetAction() ne "Pull" && GetAction() ne "Pull2" && GetAction() ne "Pull3")
    return();
  // Neue Gangart              
  var fPulling = GetActionTarget();
  if(inGait == 0) { if(fPulling) SetAction("Pull"); else SetAction("Stand"); }
  if(inGait == 1) { if(fPulling) SetAction("Pull"); else SetAction("Walk"); }
  if(inGait == 2) { if(fPulling) SetAction("Pull2"); else SetAction("Trot"); }
  if(inGait == 3) { if(fPulling) SetAction("Pull3"); else SetAction("Gallop"); }
  if(inGait == 0) 
    SetComDir(COMD_Stop());
  else
    if(GetDir() == DIR_Left()) SetComDir(COMD_Left()); else SetComDir(COMD_Right());
  if(fPulling) SetPhase(Random(10));
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
//  DoDamage(+5); Wildpferde bekommen nicht so schnell schaden
  return(1);
}

public func Death()
{
  var iCol = iGraphics;	
  if (GetRider()) DropRider();
  // Verwandeln
  ChangeDef(ID_Dead());
  // Farbe anpassen
  SetGraphics(Format("%d",iCol));
  SetAction("Dead");
  SetDir(DIR_Left());
  return(1);
}

public func DropRider()
{
  return(Fling(GetRider(), 0, 1));
}

/* Kontakt */

protected func ContactLeft()
{
  // Kollision
  if (!Inside(GetXDir(),-30,+30)) return(Collision());
  // Wenden
  TurnRight(); 
  return(1);
}
  
protected func ContactRight()
{
  // Kollision
  if (!Inside(GetXDir(),-30,+30)) return(Collision());
  // Wenden
  TurnLeft(); 
  return(1);
}
    
/* Aktionen */
  
private func Walking()
{                            
  // Reiter-Position zurücksetzen
  ResetRiderPos();
  // Vorherige Gangart wieder aufnehmen (nach Sprung oder Wendung)
  if (iGait != 1 || GetActionTarget()) 
    { SetGait(iGait); return(); }
  // Geschwindigkeit und Geräusch
  SetPhysical("Walk",50000,2);
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
  return(1);  
}
  
private func Tumbling()
{
  // Reiter abwerfen, mit Random (Indianer halten sich gut fest)
  if(!Random(2)) Fling(GetRider(),Random(5)-2,-3); 
  return(1);
}
  
private func Rising()
{
  // Reiter abwerfen, mit Random (Indianer halten sich gut fest) 
  if(!Random(4)) Fling(GetRider(),Random(5)-2,-3);
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
  if(pRider = FindObject(0, 0,0,0,0, OCF_CrewMember(), 0, this()))
    if(WildcardMatch(GetAction(pRider), "*Ride*"))
      return(pRider);
}

public func GetGait()
{
  return(iGait);
}

public func IsStill() { return(iGait == 0); }
