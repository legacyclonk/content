/*-- Drache --*/

#strict
#appendto DRGN

static const DRGN_FireBreathRange  = 200,   // Ab dieser Feindentfernung spuckt der Drache Feuer
              DRGN_EnemySearchRange = 700,  // Suchtrechteck für Feindclonks
              DRGN_FireOffsetX      = 52,
              DRGN_FireOffsetY      = 13,
              DRGN_CaveLeftX        = 1600, // Ende der Höhle (X) - diesen Bereich nicht verlassen
              DRGN_CaveLeftY        = 500,
              DRGN_CaveTopY        = 280,
              DRGN_CaveRightX       = 2000, // Rechtes Ende der Höhle (ohne Drachenhorst)
              DRGN_GoSleep_Chance   =  3, // overload
              DRGN_Look_Chance      = 30; // overload
              
static DRGN_ctrl_tx, DRGN_ctrl_ty, DRGN_ctrl_stop; // Im Intro zugewiesen: Flugziel, bzw. Stop-Befehl

// Aufsteigen geht nicht
protected func ActivateEntrance() {}

// Gescriptet über Steuerungsbefehle; der Drache bleibt trotzdem frei!
protected func ResetControl() {}

// Intro-Steuerung
public func IntroControl(int tx, int ty, bool fStop)
  {
  DRGN_ctrl_tx = tx;
  DRGN_ctrl_ty = ty;
  DRGN_ctrl_stop = fStop;
  if (!DRGN_ctrl_tx) SetCommand(0, "None");
  return(true);
  }

// Geändertes, freies Verhalten mit etwas mehr Aggression
protected func FxIntDragonFreeTimer(object target, int number, int time)
  {
  // Wenn der Drache halten soll: Nichts tun
  if (DRGN_ctrl_stop) { if (DRGN_ctrl_tx) SetCommand(0, "MoveTo", 0, DRGN_ctrl_tx, DRGN_ctrl_ty); SetComDir(COMD_None); return; }
  // Wenn es ein Flugziel gibt: Dorthin fliegen
  if (DRGN_ctrl_tx)
    {
    // Schon da? -> Intro weiterführen
    if (Distance(DRGN_ctrl_tx, DRGN_ctrl_ty, GetX(), GetY()) < 100)
      {
      DRGN_ctrl_tx = DRGN_ctrl_ty = 0;
      DRGN_ctrl_stop = true;
      GameCall("OnDragonReachTarget", DRGN_ctrl_tx, DRGN_ctrl_ty);
      return;
      }
    return(SetCommand(0, "MoveTo", 0, DRGN_ctrl_tx, DRGN_ctrl_ty));
    }
  // WalkTransition-Aktivität stoppen
  if (GetAction() eq "WalkTransitionF")
    if (!GetComDir() || GetComDir() == COMD_Stop)
      {
      var iPhase = GetPhase();
      SetAction("WalkTransitionB");
      SetPhase(iPhase);
      }
  // Zuhause zuweisen
  if(!pHome) pHome = FindObject(DLAR, 0, 0, -1, -1);
  
  // Drache ist nicht mehr in der Höhle?
  if (GetX() < DRGN_CaveLeftX || GetY() < DRGN_CaveTopY)
    return(MoveTo(DRGN_CaveLeftX+200, DRGN_CaveLeftY));
  
  // In der Höhle wird nicht geflogen!
  if (WildcardMatch(GetAction(), "Fly*"))
    {
    SetCommand(this(), "None");
    SetComDir(COMD_Down);
    return true;
    }
  
  // Naheliegendsten Gegnerclonk suchen
  var pEnemy;
  for (var pCheck in FindObjects(Find_Distance(DRGN_EnemySearchRange), Find_OCF(OCF_Alive), Find_NoContainer(), Find_Not(Find_Owner(10)), Find_Not(Find_Owner(NO_OWNER))))
    if (PathFree(GetX(), GetY(), GetX(pCheck), GetY(pCheck)))
      {
      pEnemy = pCheck;
      break;
      }
    
  // Gucken
  if(!Random(DRGN_Look_Chance) && GetAction() eq "Walk")
    {
    Walk2Action("Look");
    SetCommand(this(), "Wait", 0,0,0,0, 10);
    // Walk2Action scheint eine ComDir zu setzen, die es dann nicht zurueck
    // setzt, dadurch laeuft der Drache dauernd gegen den Bildschirmrand.
    AppendCommand(this(), "Call", this(), 0,0,0,0, "StopComDir");
    AppendCommand(this(), "Wait", 0,0,0,0, 40);
    return(true);
    }
    
  if (pEnemy)
    {
    //Message("Attack Enemy %s", this(), GetName(pEnemy));
    // Aktion mit Gegner
    // Schlafend: Aufwachen
    if (IsSleeping()) return(WakeUp());
    // Rumlaufen (immer Richtung Höhlenmitte)
    if (!Random(10) && GetAction() eq "Walk")
      if (Abs(GetX()-DRGN_CaveLeftX) < Abs(GetX()-DRGN_CaveRightX))
        return(ControlRight());
      else
        return(ControlLeft());
    // Feuer spucken auf Gegner
    return(BreathFireAt(GetX(pEnemy)-GetX(), GetY(pEnemy)-GetY()));
    }
  else
    {
    //Message("No Enemy", this());
    // Aktion ohne Gegner
    // Schlafen
    if(!Random(DRGN_GoSleep_Chance) && !IsSleeping())
      {
      // Haben wir einen Heimat-Horst?
      if(pHome)
        {
        // Dann schlafen wir nur da.
        MoveTo(pHome->GetX(), pHome->GetY() );
        AppendCommand(this(), "Call", this(), 0,0,0,0, "GoSleep");
        }
      else
        {
        // Sonst einfach mal hinlegen
        StopComDir();
        Walk2Action("GoSleep");
        }
      return(true);
      }
    // Sonst stehen bleiben
    StopComDir();
    }
  }


/* Feuer spucken */

private func BreathFireAt(int tx, int ty) // Koordinaten zum Zieloffset
  {
  // Spucken wir schon?
  if (GetEffect("IntFireBreath", this()))
    {
    // Dann Drehung anpassen
    tx -= GetDir()*DRGN_FireOffsetX*2-DRGN_FireOffsetX;
    ty -= DRGN_FireOffsetY;
    var iRotBy = Abs(45+GetR()*(GetDir()*2-1)) - Abs((Angle(0, 0, tx, ty)+180)%360-180);
    SetRDir(BoundBy(iRotBy/(GetDir()*10-5), -8, 8));
    return(1);
    }
  // Springen, wenn das Ziel oberhalb steht
  if (ty < 0 && GetContact(0, -1, 8) && !Random(5)) return(Jump());
  // ggf. umdrehen oder zum Ziel hinlaufen
  if ((tx > 0) == !GetDir() || Abs(tx) > DRGN_FireBreathRange)
    if (tx<0) return(ControlLeft()); else return(ControlRight());
  // Sonst hier stehen bleiben
  StopComDir();
  SetCommand(this(), "None");
  // Und los!
  return(DoFireBreath());
  }
  
public func DoFireBreath()
  {
  // Geth? Wenn nicht, geht es vielleicht später mit Anhalten
  if (!CanBreatheFire()) return();
  // Ansonsten, spucken!
  CreateFireBreath(this(), 40, GetDir()*DRGN_FireOffsetX*2-DRGN_FireOffsetX, DRGN_FireOffsetY, 45+GetR()*(GetDir()*2-1), GetRDir(), 40);
  return(AddEffect("IntFireBreath", this(), 1, 1, this(), 0, FireBreathExists(this()) ));
  }
  
  
protected func ControlLeft(object controller)
  {
  //Log("ControlLeft");
  SetComDir(COMD_Left());
  if (!controller) controller = this();
  return(_inherited(controller));
  }
  
  
protected func ControlRight(object controller)
  {
  //Log("ControlRight");
  SetComDir(COMD_Right());
  if (!controller) controller = this();
  return(_inherited(controller));
  }
  
  
protected func StopComDir()
  {
  //Log("StopComDir");
  SetComDir(COMD_Stop());
  return(_inherited());
  }
