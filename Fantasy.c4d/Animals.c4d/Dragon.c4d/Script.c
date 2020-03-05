/*-- Drache --*/

#strict

/*---------------------------------------------- Konstanten ----------------------------------------------------------*/

static const DRGN_MaxFlightSpeedX = 150, // Maximale Fluggeschwindigkeit
             DRGN_MaxFlightSpeedY = 150,
             DRGN_FlightComAccelX = 300, // Änderung der Zielfluggeschwindigkeit bei einem Tastendruck
             DRGN_FlightComAccelY = 300;

static const DRGN_WakeUp_Chance = 20,
             DRGN_ReturnHome_Chance = 18,
             DRGN_TakeOff_Chance = 13,
             DRGN_Look_Chance = 3,
             DRGN_GoSleep_Chance = 10, 
             DRGN_GeneralMove_Chance = 3;
             
static const DRGN_FreeMoveTo_Timeout = 20;

/*----------------------------------------------- Locals -------------------------------------------------------------*/

local fFlying;      // Drache fliegt
local fJumping;     // Drache springt
local fWalking;     // Drache läuft
local iComX, iComY; // Gewünschte Flugrichtung
local szNextAct;    // Nächste Aktivität nach Walk/Fly-Übergangsaktivität
local iCurrentRotation; // Momentane Drehung am Boden
local iVtxDir;          // Drehung, nach der die Vertices ausgerichtet sind (für automagische Walk-Umdrehungen)
local pHome;        // Heimat-Horst
local iAnger;       // Wie wuetend der Drache ist
local iMoveToTime;  // Dauer eines MoveTo-Befehls
local iHeadDrawIn, iTailDrawIn; // Einziehen des Kopf- bzw. Schwanz-Vertex
local iTopDrawIn, iFeetDrawIn;

// Vertices
// 0  (variabel)     Reitvertex   CNAT NoCollision
// 1  (+/-55, 0)     Kopf         CNAT MultiAttach + Left/Right
// 2  (+/-55, 10/30) Schwanz      CNAT MultiAttach + Left/Right
// 3  (0, 45)        Füße         CNAT MultiAttach + Bottom
// 4  (0, -30)       Oberkante    CNAT Top
//
// Generelles Abrutschen lebender Drachen (Umkippen) wird über das Alive=1-Flag verhindert
// Während des Laufens sorgt die WALK-Prozedur intern dafür, dass die Drehung dem Boden
// angepasst wird. Bei den übrigen Aktionen wir AdjustWalkRotation manuell aufgerufen.
// Der Schwanzvertex wird in AdjustWalkRotation mit einbezogen, und verhindert, dass der
// Schwanz beim Laufen im Boden steckt

/*--------------------------------------------- Initialisierung ------------------------------------------------------*/

protected func Initialize()
{ 
  // Voll gesund
  DoEnergy(GetPhysical("Energy") / 1000);
  // Wenn wir wachsen, wurde Initialize bereits bei Birth aufgerufen, ist daher
  // nun nicht mehr notwendig.
  if(GetEffect("IntGrow", this())) return(false);  
  // Gehen
  SetAction("Walk");
  fWalking = 1;
  // Zufällige Richtung
  SetDir(Random(2));
  SetComDir(COMD_Stop());
  // Rote Farbe
  SetClrModulation(RGB(100,40,0));
  // Anti-Umkippmodus
  SetAlive(1);
  // Anfangsvertices setzen
  UpdateVertices();
  // Anfaenglich frei
  SetFree(true);
  // Dauerhafter Stuck-Check
  AddEffect("StuckCheck", this(), 1, 5, this());
}

public func Redefine(idDef)
{
  ChangeDef(idDef);
  SetAction("Walk");
  return(1);
}

public func Birth(object home)
{
  // Allgemeine Initialisierung
  Initialize();
  // "Aufwach"-Aktion
  WakeUp();
  // Heimat-Horst merken
  pHome = home;
  // Wachstums-Effekt
  AddEffect("IntGrow", this(), 1, 35, this() );
  // Sound
  Sound("DragonBirth");
}

/*------------------------------------------------- Wachstum ---------------------------------------------------------*/

protected func FxIntGrowStart(object target, int number)
{
  // In EffectVar(0) wird das "echte" Con gespeichert, damit beim
  // Vergroessern keine Informationen verloren gehen
  EffectVar(0, target, number) = GetCon() * 1000;
}

protected func FxIntGrowTimer(object target, int number)
{
  // Con je nach Growth-Wert erhoehen
  EffectVar(0, target, number) = Min(EffectVar(0, target, number) + 100 * GetDefGrowth(GetID()), 100000);
  // Con anpassen
  SetCon(EffectVar(0, target, number) / 1000);
  // Wenn voll ausgewachsen, Grow-Effekt loeschen
  if(EffectVar(0, target, number) >= 100000)
    return(-1);
}

/*------------------------------------------------ Steuerung ---------------------------------------------------------*/

private func Comdir2Speed(int comdir, &vx, &vy)
{
  vx = vy = 0;
  var max = GetPhysical("Float", 0, this());
  if(comdir == COMD_Up) { vy = -max; }
  else if(comdir == COMD_UpLeft) { vy = -max; vx = -max; }
  else if(comdir == COMD_Left) { vx = -max; }
  else if(comdir == COMD_DownLeft) { vy = max; vx = -max; }
  else if(comdir == COMD_Down) { vy = max; }
  else if(comdir == COMD_DownRight) { vy = max; vx = max; }
  else if(comdir == COMD_Right) { vx = max; }
  else if(comdir == COMD_UpRight) { vy = -max; vx = max; }
  else return(false);
  return(true);
}

private func Sign(int x)
{
  if(x > 0) return(1);
  return(-1);
}

protected func FxIntJnRMoveStart(object target, int number, int temp, int vx, int vy)
{
  if(temp) return();
  EffectVar(0, target, number) = vx;
  EffectVar(1, target, number) = vy;
}

protected func FxIntJnRMoveTimer(object target, int number, int time)
{
  if(!IsFlying()) return(-1);

  var dest_vx = EffectVar(0, target, number);
  var dest_vy = EffectVar(1, target, number);

  iComX = BoundBy(dest_vx - GetXDir(0, 100), -DRGN_MaxFlightSpeedX, DRGN_MaxFlightSpeedX);
  iComY = BoundBy(dest_vy - GetYDir(0, 100), -DRGN_MaxFlightSpeedY, DRGN_MaxFlightSpeedY);

  if(iComX == 0 && iComY == 0) return(-1);
}

public func ControlUpdate(object controller, int comdir)
{
  if(IsFlying())
  {
    var dest_vx, dest_vy;
    Comdir2Speed(comdir, dest_vx, dest_vy);
    if(dest_vx == 0) dest_vx = 50 * Sign(GetXDir(0, 100));
    if(dest_vy == 0) dest_vy = 50 * Sign(GetYDir(0, 100));

    if(GetEffect("IntJnRMove", this()))
      RemoveEffect("IntJnRMove", this());

    AddEffect("IntJnRMove", this(), 1, 10, this(), 0, dest_vx, dest_vy);
  }
  else if(IsWalking())
  {
    if(comdir == COMD_Right() || comdir == COMD_UpRight() || comdir == COMD_DownRight)
      MoveRight();
    else if(comdir == COMD_Left() || comdir == COMD_UpLeft() || comdir == COMD_DownLeft)
      MoveLeft();
    else
      SetComDir(COMD_Stop);
  }
}

public func ControlLeft(object controller)
{
  // Nicht mehr am Leben
  if (!GetAlive()) return();
  // Kontrolle zurücksetzen
  ResetControl();
  // Bewegung
  if(!GetPlrJumpAndRunControl(controller->GetController()))
    MoveLeft();
}
  
public func ControlRight(object controller)
{
  // Nicht mehr am Leben
  if (!GetAlive()) return();
  // Kontrolle zurücksetzen
  ResetControl();
  // Bewegung
  if(!GetPlrJumpAndRunControl(controller->GetController()))
    MoveRight();
}

public func ControlUp(object controller)
{
  // Nicht mehr am Leben
  if (!GetAlive()) return();
  // Kontrolle zurücksetzen
  ResetControl();
  // Im Flug: Flugsteuerung  
  if (IsFlying())
  {
    if(!GetPlrJumpAndRunControl(controller->GetController()))
      iComY = Max(iComY - DRGN_FlightComAccelY, -DRGN_MaxFlightSpeedY);

    return(1);
  }
  // Im Sprung: losfliegen
  if (IsJumping())
    return(TakeOff());
  // Auf dem Boden: springen
  if (!IsFlying()) 
    return(InitiateJump());
}

public func ControlDown(object controller)
{
  // Nicht mehr am Leben
  if (!GetAlive()) return();
  // Kontrolle zurücksetzen
  ResetControl();
  if(!GetPlrJumpAndRunControl(controller->GetController()))
  {
    // Flugsteuerung
    if (IsFlying())
    {
      iComY = Min(iComY + DRGN_FlightComAccelY, DRGN_MaxFlightSpeedY);
      return(1);
    }
    // Auf dem Boden: anhalten
    SetComDir(COMD_Stop());
  }
  return(1);
}

public func ControlDownDouble(pByObject)
{
  // Nicht mehr am Leben
  if (!GetAlive()) return();
  // Kontrolle zurücksetzen
  ResetControl();
  // Auf dem Boden: Reiter absitzen (im Flug ist absitzen per ControlDownDouble nicht möglich)
  if (GetContact(0, 3, 8))
    if (pByObject == GetRider())
    {
      // Reiter absitzen
      ObjectSetAction(pByObject, "Walk");
      return(1);
    }
  // Keine Steuerung
  return(0);
}

/* Feuerspeien */

public func ControlDig() 
{ 
  // Nicht mehr am Leben
  if (!GetAlive()) return();
  // Kontrolle zurücksetzen
  ResetControl();
  // Geth?
  if (!CanBreatheFire()) return(1);
  // Spucken wir schon?
  if (GetEffect("IntFireBreath", this())) return(1);
  // Ansonsten, auf gehts
  CreateFireBreath(this(), 40, GetDir()*104-52, 13, 45+GetR()*(GetDir()*2-1), GetRDir(), 40);
  GetFireBreath(this())->AttachToVertex(1);
  AddEffect("IntFireBreath", this(), 1, 1, this(), 0, FireBreathExists(this()) );
  Sound("DragonFire");
  return(1);
}

public func ControlDigSingle()
{
  // Nicht mehr am Leben
  if (!GetAlive()) return(1);
  // Fehlermeldung "kann nicht graben" verhindern
  return(1);  
}

public func ControlCommand(strCommand, pTarget, iTx, iTy)
{
  // Nicht mehr am Leben
  if (!GetAlive()) return();
  // Kontrolle zurücksetzen
  ResetControl();
  // Kommando setzen
  return(SetCommand(this(), strCommand, pTarget, iTx, iTy));
}

/*------------------------------------------------ Feueratem ---------------------------------------------------------*/

protected func FxIntFireBreathStart(object target, int number, bool temp, object firebreath)
{
  if(temp) return();
  EffectVar(0, target, number) = firebreath;
}

protected func FxIntFireBreathTimer(object target, int number, int time)
{
  // Aufhoeren, wenn wir nicht mehr Feuer spucken koennen
  if(!CanBreatheFire() ) return(-1);

  // Effekt weg? Dann setzen wir EffectVar(1) auf die momentane
  // Aktivitaetszeit.
  if(!EffectVar(0, target, number) && !EffectVar(1, target, number) )
    EffectVar(1, target, number) = time;

  // Nach weiteren 35 Frames loeschen wir den Effekt - damit kann der
  // Drache erneut Feuer spucken
  if(EffectVar(1, target, number) && time - 35 >= EffectVar(1, target, number) )
    return(-1);
}

protected func FxIntFireBreathStop(object target, int number, bool temp)
{
  if(temp) return();
  // Effekt wurde unterbrochen: Aufhoeren mit Feuer spucken.
  if(EffectVar(0, target, number) )
    EffectVar(0, target, number)->RemoveObject();
}

public func CanBreatheFire()
{
  // Kann der Drache momentan Feuer speien?
  return(GetAction() S= "Fly" || WildcardMatch(GetAction(), "FlyFast*") || WildcardMatch(GetAction(), "Glide*") || GetAction() S= "Walk" || WildcardMatch(GetAction(), "Jump*") );
}

/*-------------------------------------------------- Bewegung --------------------------------------------------------*/

private func MoveLeft()
{  
  // Auf dem Boden: wenden
  if (GetDir()==DIR_Right() && (GetAction() eq "Walk" || (WildcardMatch(GetAction(), "Slide*") && GetContact(0, -1, 8))))
  {
    SetComDir(COMD_Left()); SetDir(DIR_Left());
    SetAction("WalkTurn"); SetXDir();
    return(1);
  }
  // Auf dem Boden: loslaufen
  else if (GetAction() eq "Walk")
  {
    SetComDir(COMD_Left());
    return(1);
  }
  // Im Flug
  else if (IsFlying())
  {
    iComX = Max(iComX - DRGN_FlightComAccelX, -DRGN_MaxFlightSpeedX);
    return(1);
  }
  return();
}

private func MoveRight()
{  
  // Auf dem Boden: wenden
  if (GetDir()==DIR_Left() && (GetAction() eq "Walk" || (WildcardMatch(GetAction(), "Slide*") && GetContact(0, -1, 8))))
  {
    SetComDir(COMD_Right()); SetDir(DIR_Right());
    SetAction("WalkTurn"); SetXDir();
    return(1);
  }
  // Auf dem Boden: loslaufen
  else if (GetAction() eq "Walk")
  {
    SetComDir(COMD_Right());
    return(1);
  }
  // Im Flug
  else if (IsFlying())
  {
    iComX = Min(iComX + DRGN_FlightComAccelX, DRGN_MaxFlightSpeedX);
    return(1);
  }
  return();
}

protected func ReverseDir() 
{ 
  return(SetDir(1-GetDir())); 
}

private func StartSlide(int i)
{
  // Nicht mehr am Leben
  if (!GetAlive()) return();
  // Rotation aktualisieren
  WalkRotation();
  // Slide-Aktivität
  SetAction("Slide");
  return(1);
}
  
protected func Sliding() // StartCall aller Slide-Aktionen
{
  // Nicht mehr am Leben
  if (!GetAlive()) return();
  // Überhaupt kein Kontakt? Dann fliegen.
  if (GetContact(0, -1) == 0)
    return(TakeOff());
  // Bodenkontakt? Dann Walk.
  if (GetContact(0, 3, 8)) 
    return(SetAction("Walk"));
  // Deckenkontakt? Dann fliegen.
  if (GetContact(0, -1, 4))     
    return(TakeOff());
  // Slideanimation standardgemäß nicht dauernd wiederholen
  var fForceAnim = !Random(10);
  // Slide in Zielrichtung
  var iXDir=GetXDir(), iYDir=GetYDir(), iRDir;
  if (GetComDir() == COMD_Left()) 
    iXDir-=10;
  else if (GetComDir() == COMD_Right()) 
    iXDir+=10;
  // Keine Zielrichtung: abheben    
  else 
    return(TakeOff(1));
  // Kontakt links/rechts -> Horizontal halten; Redirect YDir
  if (GetContact(0,-1,3)) { iXDir=0; iYDir-=10; fForceAnim=true; }
  // Kopf/Schwanzkontakt->Rotation
  if (GetContact(0,2,8)) iRDir += GetDir()*8-4;
  if (GetContact(0,1,8)) iRDir -= GetDir()*8-4;
  // Irgendein Bodenkontakt: Höher gleiten; sonst absenken (Gravitation)
  if (GetContact(0, -1, 8)) iYDir-=20;
  // X/Y/RDir setzen
  SetXDir(BoundBy(iXDir, -25, 25)); SetYDir(Max(iYDir, -25)); SetRDir(iRDir);
  // Weiter Slide; Animation wechseln
  if (fForceAnim && GetAction() eq "Slide3") SetAction("Slide");
}

/* Flug */

public func TakeOff(bool fVertical)
{
  // Nicht mehr am Leben
  if (!GetAlive()) return();
  // Flag setzen
  fFlying = 1;
  fWalking = 0;
  // Abheben
  SetAction("TakeOff");
  // Startgeschwindigkeit
  iComX=iComY=0;
  if (fVertical)
    iComY=-25;
  else
    iComX=GetDir()*50-25;
  if (GetYDir()>-5) SetYDir(-5);
  // Noch keine Zielrichtung
  SetComDir(COMD_None());
}

protected func TakenOff() // EndCall von Action TakeOff
{
  UpdateVertices();
}

protected func StopFlying()
{
  // Nicht mehr am Leben
  if (!GetAlive()) return();
  // Flag löschen
  fFlying = 0;
  // Updates
  UpdateVertices();
  SetRDir(0); 
  SetR(0);
  //if (Stuck()) UnstickEx(20, 25);
  SetAction("Land");
}

protected func Flying() // PhaseCall aller Flugaktionen
{
  // Nicht mehr am Leben
  if (!GetAlive()) return();
  // Flügelschlagen
  if (WildcardMatch(GetAction(), "Fly*") && (GetPhase() == 2))
    Sound("DragonWing*");
  // Flag setzen (muss immer aktuell sein - auch wenn der Drache per KI/Command in diese Aktion wechselt)
  fFlying = 1;    
  // Steckt? Dann mit dem Fliegen aufhören...
  if (Stuck()) return(StopFlying());
  // Command2Com
  if (GetCommand())
    if (GetCommand() eq "MoveTo")
      if (GetCommand(0, 1))
      {
        // Bewegung zum Objekt
        iComX = BoundBy((GetX(GetCommand(0, 1))-GetX())/4, -DRGN_MaxFlightSpeedX, DRGN_MaxFlightSpeedX);
        iComY = BoundBy((GetY(GetCommand(0, 1))-GetY())/4, -DRGN_MaxFlightSpeedY, DRGN_MaxFlightSpeedY);
      }
      else
      {
        // Bewegung zur Zielposition
        iComX = BoundBy((GetCommand(0, 2)-GetX())/4, -DRGN_MaxFlightSpeedX, DRGN_MaxFlightSpeedX);
        iComY = BoundBy((GetCommand(0, 3)-GetY())/4, -DRGN_MaxFlightSpeedY, DRGN_MaxFlightSpeedY);
      }
    else
      // Nicht MoveTo-Command: Bewegungs-Com löschen (z.B. Follow innerhalb der Follow-Range)
      iComX=iComY=0;
  // Gegenwärtige Geschwindigkeit ermitteln
  var iVx=GetXDir(0,100), iVy=GetYDir(0,100);
  var iTargetRot,iCurrentRotation=GetR();
  var iDir=GetDir()*2-1;
  // Wunschgeschwindigkeit ermitteln
  var idVx=iVx+iComX, idVy=iVy+iComY;
  // Horizontal anpassen
  if (iComX)
  {
    // Turning
    if (GetDir() && idVx<-10 && GetXDir()<200 && GetAction() ne "FlyTurn")
      { SetDir(DIR_Left()); SetAction("FlyTurn"); }
    else if (!GetDir() && idVx>10 && GetXDir()>-200 && GetAction() ne "FlyTurn")
      { SetDir(DIR_Right()); SetAction("FlyTurn"); }
    // Geschwindigkeit nach Com
    iVx += BoundBy(iComX, -20, 20);
    iComX = idVx-iVx;
  }
  // Vertikale Geschwindigkeit anpassen
  var iMaxRaise = Min(Abs(iComX), 10)-Min(Abs(iVx), 20);
  iVy += BoundBy(iComY, iMaxRaise, 20); iComY = idVy-iVy;
  // Drehung anpassen
  if (Abs(iVx) < 100) iTargetRot=0;
  else if (iVx>0) iTargetRot=BoundBy(Angle(iVy, -iVx), 135, 225)-180;
  else iTargetRot=BoundBy(Angle(-iVy, iVx), 135, 225)-180;
  SetRDir();
  if (iTargetRot > iCurrentRotation+2) SetRDir(15,0,100);
  else if (iTargetRot < iCurrentRotation-2) SetRDir(-15,0,100);
  // Neu berechnete Geschwindigkeit setzen
  SetXDir(iVx, 0, 100); SetYDir(iVy, 0, 100);
  //Message("XDir: %d  YDir: %d  Com: %d / %d  Rot: %d/%d", this(), iVx, iVy, iComX, iComY, GetR(), iTargetRot);
  // Reit-Vertex anpassen
  UpdateRideVertex();
  UpdateVertices();
  // Fertig
  return(1);
}

protected func FlyingEndAction()
{
  // Nicht mehr am Leben
  if (!GetAlive()) return();
  // Feuerball geplant?
  if (szNextAct)
  {
    var szNA = szNextAct; szNextAct=0;
    return(SetAction(szNA));
  }
  // Nächste Flugaktivität bestimmen
  // Bei hoher Geschwindigkeit und nicht "Steigflug": Gleitflug
  var iVx=GetXDir(), iVy=GetYDir();
  if (Abs(iVx) > 25)
    if (iVy>8-Random(30))
      // Reiner Gleitflug je nach Steigung
      return(SetAction("Glide"));
    else
      // Schnelle Flügelschläge im Gleitflug
      return(SetAction("FlyFast"));
  return(SetAction("Fly"));
}

protected func FlyTurningProcess()
{
  // Nicht mehr am Leben
  if (!GetAlive()) return();
  // Turn-Vertexupdate
  UpdateVertices(100-GetPhase()*6);
  // Reit-Vertexupdate  
  UpdateRideVertex();
  // Festhängen
  if (Stuck())
  {
    // Steckt: Umdrehen
    var iPhase = GetPhase();
    SetAction("FlyTurnBack");
    SetPhase(15-iPhase);
  }
  // Normale Flugverarbeitung
  return(Flying());
}

protected func FlyTurningBProcess()
{
  // Turn-Vertexupdate
  UpdateVertices(GetPhase()*6+5);
  return(Flying());
}

protected func FlyTurnDone()
{
  // Drehung vollendet: Vertices aktualisieren
  UpdateVertices();
  // Hängt fest?
  if (Stuck())
    // Dann wieder zurückdrehen
    if (iVtxDir) return(MoveLeft()); else return(MoveRight());
  return(1);
}

protected func FlyTurnBackDone()
{
  // TurnBack-Drehung: Richtung wieder zurücksetzen
  SetDir(1-GetDir());
  return(FlyTurnDone());
}

protected func Hit()
{
  // Nicht mehr am Leben
  if (!GetAlive()) return();
  // Gegen die Wand geflogen?
  if (WildcardMatch(GetAction(), "Glide*"))
    // Aus Gleit-Aktion in Flug-Aktion wechseln
    SetAction("Fly");
  return(1);
}

/*-------------------------------------------------- Kontakt ---------------------------------------------------------*/

protected func ContactTop()
{
  // Im Flug
  if (IsFlying())
  {
    // Kontakt am oberen Spielfeldrand ignorieren
    if (GetY() >= 100)
      // Auf dem Weg nach oben
      if (GetYDir() <= 0)
        // Leicht nach unten rutschen, um an kleineren Ecken vorbei zu kommen
        SetYDir(+20);  
    // Nichts weiter machen    
    return();    
  }
}

protected func ContactBottom()
{
  // Nicht mehr am Leben
  if (!GetAlive()) return();
  // Im Flug
  if (IsFlying())
  {
    // Bodenkontakt mit den Füßen
    if (GetContact(0, 3, 8)) 
      // Auf dem Weg nach unten: landen
      if (GetYDir() >= 0)
        StopFlying();
      // Ansonsten: leicht nach oben rutschen, um über kleinere Ecken hinweg zu kommen
      else
        SetYDir(-20);  
    // Nichts weiter machen    
    return();    
  }
  // Beim Gehen
  if (IsWalking())
  {
    // Bodenkontakt mit den Füßen
    if (GetContact(0, 3, 8)) 
      // Will nach rechts oder nach links laufen
      if ((GetComDir() == COMD_Left()) || (GetComDir() == COMD_Right()))
        // Wahrscheinlich hängen wir an einer Ecke fest: durch einen kleinen Sprung befreien
        InitiateJump();        
    // Nichts weiter machen            
    return(0);
  }
  // Sprung: Bei leichtem Bodenkontakt Walk setzen, damit der Drache nicht ausrutscht
  // Ab OCF_HitSpeed3 wird ohnehin engineintern Kneel gesetzt
  if ((GetAction() eq "Jump2") && (~GetOCF() & OCF_HitSpeed3()))
    {
    // Kleiner Effekt
    Sound("DragonStep*",0,0,50);
    // Abrutschen verhindern
    SetRDir(); SetXDir();
    SetAction("Walk");
    fJumping = 0;
    fWalking = 1;
    return(1);
  }
  // Tumble
  if (WildcardMatch(GetAction(), "Tumble*"))
  {
    // Am Boden mit Tumble aufhören (sieht blöd aus)
    SetRDir(); 
    SetXDir();
    AdjustWalkRotation(20, 20, 100);
    return(SetAction("KneelDown"));
  }
  // Sliding
  if (WildcardMatch(GetAction(), "Slide*"))
  {
    // Slide-Kontakt: Ausführen!
    Sliding();
    return(AdjustWalkRotation(20, 20, 100));
  }
}

protected func ContactLeft()
{
  // Im Flug
  if (IsFlying())
    // Nicht am linken Spielfeldrand
    if (GetX() >= 100)
      // Auf dem Weg nach links
      if (GetXDir() <= 0)
        // Leicht nach rechts abprallen
        SetXDir(+30);  
}

protected func ContactRight()
{
  // Im Flug
  if (IsFlying())
    // Nicht am rechten Spielfeldrand
    if (GetX() < LandscapeWidth() - 100)
      // Auf dem Weg nach rechts
      if (GetXDir() >= 0)
        // Leicht nach links abprallen
        SetXDir(-30);  
}

/*------------------------------------------------ Aktivitäten -------------------------------------------------------*/

protected func WalkStart() { fWalking=1; }
protected func Landing() { AdjustWalkRotation(20, 20, 100); UnstickTiny(); UpdateRideVertex(); return(1); }
protected func Kneeling() { fJumping=0; AdjustWalkRotation(20, 20, 100); return(Sound("DragonStep2")); }
protected func Looking() { }
protected func WalkAborting() { iCurrentRotation=GetR(); return(SetRDir()); }
protected func WalkRotation() { AdjustWalkRotation(20, 20, 100); UpdateRideVertex(); return(1); }
protected func JumpAborting() { fJumping=0; }

protected func Sleeping()
{ 
  var pLair;
  // Ausgewachsen
  if (GetCon() >= 100)
    // Lange geschlafen
    if (GetActTime() > 500)
      // In einem Horst
      if (pLair = FindObject(DLAR))
        // Noch nicht zu viele Eier
        if (ObjectCount(DEGG, -100,-50,200,100) < 3)
        {
          // Ei legen!
          DropEgg(pLair);
          // Und aufwachen
          WakeUp();
        }
}

public func DropEgg(object pLair)
{
  var egg = CreateObject(DEGG);
  egg->SetLair(pLair);
  egg->SetR(Random(360));
}

protected func Walking()
{
  // Reitvertex aktualisieren
  UpdateRideVertex(); 
  
  // Sprung-Flag zurücksetzen
  fJumping = 0;
  fWalking = 1;
    
  // Vertexupdate
  if (GetDir() != iVtxDir)
  {
    UpdateVertices();
    // Hängt fest?
    if (Stuck()) 
      // Befreien
      if (!Unstick())
        // Befreien fehlgeschlagen: wieder zurückdrehen
        if (iVtxDir) 
          return(MoveLeft()); 
        else 
          return(MoveRight());
  }
  
  // Seitlicher Kontakt am Fußvertex...?
  if (GetContact(0,1,8))
    // Abrutschen
    return(StartSlide(1));
    
  // Schrittsound (nur bei Bodenkontakt)
  if ((GetPhase() == 3) || (GetPhase() == 11))
    Sound("DragonStep*");    
}

protected func Jumping() // PhaseCall / StartCall der Jump-Aktionen
{
  // Ungewollter Sprung: rutschen
  if (!IsJumping()) return(StartSlide(2));
  // Im Sprung aufrichten
  if (Abs(GetR()) > 5) SetRDir(-GetR(), 0, 100); else SetRDir();
  // Bodenkontakt?
  if (GetContact(0, -1, CNAT_Bottom))
  {
    // Bodenkontakt am Bodenvertex (Index 3) sollte ohnehin durch ContactBottom abgefangen werden
    // Durch SetPosition-Verschiebungen könnte der Aufruf aber trotzdem hier landen
    if (GetContact(0, 3, CNAT_Bottom)) return(1);
    // Bodenkontakt am Schwanz? und fast horizontal?
    if (GetContact(0, 2, CNAT_Bottom) && Abs(GetR())<20)
      // Erstmal versuchen, zu drehen
      AdjustWalkRotation(20, 20, 100);
    else     
      // Bodenkontakt am anderen Vertex oder am Umkippen: Der Drache steckt im Loch oder hängt irgendwo fest :\
      // Erstmal Slide versuchen
      StartSlide(3);
  }

  // Reitvertex aktualisieren
  UpdateRideVertex();    
    
  return(1);
}

protected func KneelingDown2()
{
  // Nicht mehr am Leben
  if (!GetAlive()) return();
  // Sprung: Nicht zu weit ausholen
  if (IsJumping()) 
    SetAction("KneelUp3"); 
  else 
    SetAction("KneelUp");
  return(1);
}

protected func KneelingUp4()
{
  // Nicht mehr am Leben
  if (!GetAlive()) return();
  // Heruntergekniet für Sprung?
  if (IsJumping())
  {
    // Sprung ausführen
    SetPosition(GetX(), GetY()-10);
    // Falls der Drache irgendwie leicht festhängt...
    if (Stuck()) SetPosition(GetX(), GetY()+10);
    Jump();
  }
  else
  {
    // Ansonsten normal weiterlaufen
    SetAction("Walk");
    fWalking = 1;
  }
  return(1);
}

public func InitiateJump()
{
  // Nicht mehr am Leben
  if (!GetAlive()) return();
  // Nur im Laufen oder Sliden
  if (GetAction() ne "Walk" && !WildcardMatch(GetAction(), "Slide*")) return(1);
  // Aktivität setzen
  SetAction("KneelDown");
  // Sprungflag setzen - nach dem Setzen der Aktivität, da diese das Flag im StartCall zurücksetzt
  fJumping = 1;
  fWalking = 0;
  // Steuerung überladen
  return(1);
}

private func Walk2Action(szTargetAction)
{
  // Nicht mehr am Leben
  if (!GetAlive()) return();
  // Kein Sprung nötig: Aktivität direkt setzen
  var iPhase = GetPhase();
  if (!Inside(iPhase, 2, 13)) return(SetAction(szTargetAction));
  // Ansonsten zur Aktivität hinlaufen
  if (iPhase >= 4)
  {
    SetAction("WalkTransitionF");
    SetPhase(iPhase + 1);
    if (GetDir()) SetComDir(COMD_Right()); else SetComDir(COMD_Left());
  }
  else
  {
    // Oder einen Schritt zurück
    SetAction("WalkTransitionB");
    SetPhase(14 - iPhase);
  }
  szNextAct = szTargetAction;
}

protected func WalkTransition()
{
  var szTemp = szNextAct; szNextAct=0;
  SetR(iCurrentRotation); SetRDir();
  return(SetAction(szTemp));
}

protected func WalkTransitionAborted()
{
  // War wohl nix
  szNextAct=0;
}

protected func WalkTurningProcess()
{
  // Vertexupdates
  UpdateVertices(100 - GetPhase() * 6);
  UpdateRideVertex();
  // Hänngt fest
  if (Stuck())
    if (!Unstick(GetPhase()<8))
    {
      /*// Steckt: Umdrehen       Rückdrehung deaktiviert, gegen Endlos-Drehschleifen...
      var iPhase = GetPhase();
      SetAction("WalkTurnBack");
      SetPhase(15 - iPhase);*/
    }
  return(1);
}

protected func WalkTurningBProcess()
{
  // Turn-Vertexupdate
  UpdateVertices(GetPhase()*6+5);
  if (Stuck()) Unstick(GetPhase()>=8);
  return(1);
}

protected func WalkTurnDone()
{
  // Drehung vollendet: Vertices aktualisieren
  UpdateVertices();
  // Hängt fest?
  if (Stuck()) if (!Unstick())
    // Dann wieder zurückdrehen
    if (iVtxDir) 
      return(MoveLeft()); 
    else 
      return(MoveRight());
  return(1);
}

protected func WalkTurnBackDone()
{
  // TurnBack-Drehung: Richtung wieder zurücksetzen
  SetDir(1-GetDir());
  return(WalkTurnDone());
}

/*--------------------------------------------------- Status ---------------------------------------------------------*/

public func IsFlying()
{
  return(fFlying);
}

public func IsJumping()
{
  return(fJumping);  
}

public func IsWalking()
{
  return(fWalking); // Leider nicht sehr zuverlässig...
}

public func IsSleeping()
{
  return(WildcardMatch(GetAction(), "*Sleep*"));
}

public func IsAngry()
{
  return(iAnger);  
}

/*--------------------------------------------------- Reiten ---------------------------------------------------------*/

protected func ActivateEntrance(pRider)
{
  // Nur ein Reiter zur Zeit
  if (GetRider()) 
    return(0);
  // Aufsitzen
  if (!ObjectSetAction(pRider, "Ride", this()))
  {
    if (GetOCF(pRider) & OCF_CrewMember())
      Message("$MsgNoGoodRider$", pRider, GetName(pRider));
    return(0);
  }
  // Kontrolle zurücksetzen (und Controlled-Mode setzen)
  ResetControl();
  // Clonkposition anpassen - ggf. erst noch richtige Position ermitteln...!
  //SetPosition(GetX(), GetY(), pRider);
  // Erfolg
  return(1);
}

public func GetRider()
{
  var pRider;
  if(pRider = FindObject2(Find_OCF(OCF_CrewMember), Find_ActionTarget(this), Find_Func("IsRiding")))
     return(pRider);
}

public func DropRider()
{
  var pRider = GetRider();
  pRider->SetActionTargets();
  return(Fling(pRider, 0, 1));
}

public func IsStill()
{ 
  // Für die passende Reit-Animation
  return(1);
}

/*-------------------------------------------------- Vertices --------------------------------------------------------*/

private func UpdateVertices(int iTurnPercentage)
{
  iVtxDir = GetDir();
  var iDir = iVtxDir*2-1;
  // Kopf
  var yOffset = 0; if (GetAction() S= "Fly") yOffset = Sin(360 * GetPhase() / 15, 10) - 5;
  var cnat = 1 + GetDir();
  SetVertex(1, 0, iDir * (58 - iTurnPercentage - iHeadDrawIn), 0, 1);
  SetVertex(1, 1, 10 + yOffset, 0, 1);
  SetVertex(1, 2, cnat + CNAT_MultiAttach(), 0, 1);
  // Schwanz
  var yOffset = 0; if (GetAction() S= "Fly") yOffset = -20;
  SetVertex(2, 0, iDir * (iTurnPercentage - 55 + iTailDrawIn), 0, 1);
  SetVertex(2, 1, 30 + yOffset, 0, 1);
  SetVertex(2, 2, 3 - cnat + CNAT_MultiAttach(), 0, 1);
  // Füße
  SetVertex(3, 1, 45 - iFeetDrawIn, 0, 1);
  // Oberkante
  SetVertex(4, 1, -30 + iTopDrawIn, 0, 2);
  // Fertig
  return(1);
}

private func ResetVertices()
{
  // Alle Vertices auf DefCore-Position zurücksetzen
  var vertexCount = GetDefCoreVal("Vertices", "DefCore", GetID());
  for (var i = 0; i < vertexCount; i++) 
  {
    var vertexX = GetDefCoreVal("VertexX", "DefCore", GetID(), i);
    var vertexY = GetDefCoreVal("VertexY", "DefCore", GetID(), i);
    SetVertex(i, 0, vertexX, this(), 2);
    SetVertex(i, 1, vertexY, this(), 2);
  }    
}

private func UpdateRideVertex()
{ 
  var strAction = GetAction();
  var iPhase = GetPhase();
  var iDir = GetDir() * 2 - 1;
  
  // Wir müssen die Actions aus der ActMap des Drachen, die teilweise nur
  // Ausschnitte der verschiedenen Basisaktionen sind, wieder auf die Basisaktion
  // inklusive der richtigen Basisphase mappen.
  if (strAction S= "FlyFast") strAction = "Fly";
  if (strAction S= "FlyFast2") { strAction = "Fly"; iPhase += 8; }
  if (strAction S= "Glide2") strAction = "Glide";
  if (strAction S= "WalkTurn") { iPhase = 14 - iPhase; }
  if (strAction S= "TakeOff") { strAction = "Land"; iPhase = 14 - iPhase; }
  if (strAction S= "WalkTransitionF") strAction = "Walk";
  if (strAction S= "WalkTransitionB") { strAction = "Walk"; iPhase += 1; }
  if (strAction S= "Jump") strAction = "Walk";
  if (strAction S= "Jump2") { strAction = "Walk"; iPhase += 2; }
  if (strAction S= "Tumble2") strAction = "Tumble";
  // TODO: FlyTurnBack, Land, KneelDown*, Slide*, KneelUp*, WalkTurnBack, Sleep*, GoSleep, WakeUp, Look*

  SetBaseRideVertex(strAction, iPhase, iDir);
  
}

private func SetBaseRideVertex(strAction, iPhase, iDir)
{
  // Diese Funktion wurde automatisch generiert.
  if (strAction S= "Fly")
  {
    if (iPhase == 0) { SetVertex(0, 0, 39 * iDir, 0, 1); SetVertex(0, 1, -12, 0, 2); }
    if (iPhase == 1) { SetVertex(0, 0, 40 * iDir, 0, 1); SetVertex(0, 1, -10, 0, 2); }
    if (iPhase == 2) { SetVertex(0, 0, 41 * iDir, 0, 1); SetVertex(0, 1, -8, 0, 2); }
    if (iPhase == 3) { SetVertex(0, 0, 42 * iDir, 0, 1); SetVertex(0, 1, -6, 0, 2); }
    if (iPhase == 4) { SetVertex(0, 0, 42 * iDir, 0, 1); SetVertex(0, 1, -6, 0, 2); }
    if (iPhase == 5) { SetVertex(0, 0, 42 * iDir, 0, 1); SetVertex(0, 1, -6, 0, 2); }
    if (iPhase == 6) { SetVertex(0, 0, 42 * iDir, 0, 1); SetVertex(0, 1, -7, 0, 2); }
    if (iPhase == 7) { SetVertex(0, 0, 42 * iDir, 0, 1); SetVertex(0, 1, -9, 0, 2); }
    if (iPhase == 8) { SetVertex(0, 0, 43 * iDir, 0, 1); SetVertex(0, 1, -11, 0, 2); }
    if (iPhase == 9) { SetVertex(0, 0, 42 * iDir, 0, 1); SetVertex(0, 1, -13, 0, 2); }
    if (iPhase == 10) { SetVertex(0, 0, 42 * iDir, 0, 1); SetVertex(0, 1, -14, 0, 2); }
    if (iPhase == 11) { SetVertex(0, 0, 42 * iDir, 0, 1); SetVertex(0, 1, -15, 0, 2); }
    if (iPhase == 12) { SetVertex(0, 0, 41 * iDir, 0, 1); SetVertex(0, 1, -15, 0, 2); }
    if (iPhase == 13) { SetVertex(0, 0, 40 * iDir, 0, 1); SetVertex(0, 1, -14, 0, 2); }
    if (iPhase == 14) { SetVertex(0, 0, 39 * iDir, 0, 1); SetVertex(0, 1, -13, 0, 2); }
  }
  if (strAction S= "FlyTurn") // *name manually adjusted
  {
    if (iPhase == 0) { SetVertex(0, 0, -39 * iDir, 0, 1); SetVertex(0, 1, -12, 0, 2); }
    if (iPhase == 1) { SetVertex(0, 0, -29 * iDir, 0, 1); SetVertex(0, 1, -15, 0, 2); }
    if (iPhase == 2) { SetVertex(0, 0, -13 * iDir, 0, 1); SetVertex(0, 1, -17, 0, 2); }
    if (iPhase == 3) { SetVertex(0, 0, -6 * iDir, 0, 1); SetVertex(0, 1, -19, 0, 2); }
    if (iPhase == 4) { SetVertex(0, 0, -1 * iDir, 0, 1); SetVertex(0, 1, -21, 0, 2); }
    if (iPhase == 5) { SetVertex(0, 0, 4 * iDir, 0, 1); SetVertex(0, 1, -24, 0, 2); }
    if (iPhase == 6) { SetVertex(0, 0, 10 * iDir, 0, 1); SetVertex(0, 1, -27, 0, 2); }
    if (iPhase == 7) { SetVertex(0, 0, 16 * iDir, 0, 1); SetVertex(0, 1, -28, 0, 2); }
    if (iPhase == 8) { SetVertex(0, 0, 23 * iDir, 0, 1); SetVertex(0, 1, -27, 0, 2); }
    if (iPhase == 9) { SetVertex(0, 0, 29 * iDir, 0, 1); SetVertex(0, 1, -25, 0, 2); }
    if (iPhase == 10) { SetVertex(0, 0, 34 * iDir, 0, 1); SetVertex(0, 1, -21, 0, 2); }
    if (iPhase == 11) { SetVertex(0, 0, 37 * iDir, 0, 1); SetVertex(0, 1, -16, 0, 2); }
    if (iPhase == 12) { SetVertex(0, 0, 39 * iDir, 0, 1); SetVertex(0, 1, -13, 0, 2); }
    if (iPhase == 13) { SetVertex(0, 0, 40 * iDir, 0, 1); SetVertex(0, 1, -11, 0, 2); }
    if (iPhase == 14) { SetVertex(0, 0, 39 * iDir, 0, 1); SetVertex(0, 1, -11, 0, 2); }
  }
  if (strAction S= "Land")
  {
    if (iPhase == 0) { SetVertex(0, 0, 39 * iDir, 0, 1); SetVertex(0, 1, -12, 0, 2); }
    if (iPhase == 1) { SetVertex(0, 0, 37 * iDir, 0, 1); SetVertex(0, 1, -16, 0, 2); }
    if (iPhase == 2) { SetVertex(0, 0, 33 * iDir, 0, 1); SetVertex(0, 1, -20, 0, 2); }
    if (iPhase == 3) { SetVertex(0, 0, 28 * iDir, 0, 1); SetVertex(0, 1, -23, 0, 2); }
    if (iPhase == 4) { SetVertex(0, 0, 24 * iDir, 0, 1); SetVertex(0, 1, -23, 0, 2); }
    if (iPhase == 5) { SetVertex(0, 0, 23 * iDir, 0, 1); SetVertex(0, 1, -22, 0, 2); }
    if (iPhase == 6) { SetVertex(0, 0, 27 * iDir, 0, 1); SetVertex(0, 1, -18, 0, 2); }
    if (iPhase == 7) { SetVertex(0, 0, 34 * iDir, 0, 1); SetVertex(0, 1, -9, 0, 2); }
    if (iPhase == 8) { SetVertex(0, 0, 40 * iDir, 0, 1); SetVertex(0, 1, 2, 0, 2); }
    if (iPhase == 9) { SetVertex(0, 0, 43 * iDir, 0, 1); SetVertex(0, 1, 14, 0, 2); }
    if (iPhase == 10) { SetVertex(0, 0, 43 * iDir, 0, 1); SetVertex(0, 1, 19, 0, 2); }
    if (iPhase == 11) { SetVertex(0, 0, 42 * iDir, 0, 1); SetVertex(0, 1, 18, 0, 2); }
    if (iPhase == 12) { SetVertex(0, 0, 42 * iDir, 0, 1); SetVertex(0, 1, 12, 0, 2); }
    if (iPhase == 13) { SetVertex(0, 0, 40 * iDir, 0, 1); SetVertex(0, 1, 4, 0, 2); }
    if (iPhase == 14) { SetVertex(0, 0, 37 * iDir, 0, 1); SetVertex(0, 1, -2, 0, 2); }
  }
  if (strAction S= "Walk")
  {
    if (iPhase == 0) { SetVertex(0, 0, 34 * iDir, 0, 1); SetVertex(0, 1, -7, 0, 2); }
    if (iPhase == 1) { SetVertex(0, 0, 33 * iDir, 0, 1); SetVertex(0, 1, -5, 0, 2); }
    if (iPhase == 2) { SetVertex(0, 0, 33 * iDir, 0, 1); SetVertex(0, 1, -2, 0, 2); }
    if (iPhase == 3) { SetVertex(0, 0, 32 * iDir, 0, 1); SetVertex(0, 1, 0, 0, 2); }
    if (iPhase == 4) { SetVertex(0, 0, 32 * iDir, 0, 1); SetVertex(0, 1, 1, 0, 2); }
    if (iPhase == 5) { SetVertex(0, 0, 33 * iDir, 0, 1); SetVertex(0, 1, 0, 0, 2); }
    if (iPhase == 6) { SetVertex(0, 0, 34 * iDir, 0, 1); SetVertex(0, 1, -3, 0, 2); }
    if (iPhase == 7) { SetVertex(0, 0, 34 * iDir, 0, 1); SetVertex(0, 1, -6, 0, 2); }
    if (iPhase == 8) { SetVertex(0, 0, 34 * iDir, 0, 1); SetVertex(0, 1, -6, 0, 2); }
    if (iPhase == 9) { SetVertex(0, 0, 34 * iDir, 0, 1); SetVertex(0, 1, -4, 0, 2); }
    if (iPhase == 10) { SetVertex(0, 0, 34 * iDir, 0, 1); SetVertex(0, 1, -1, 0, 2); }
    if (iPhase == 11) { SetVertex(0, 0, 33 * iDir, 0, 1); SetVertex(0, 1, 1, 0, 2); }
    if (iPhase == 12) { SetVertex(0, 0, 33 * iDir, 0, 1); SetVertex(0, 1, 1, 0, 2); }
    if (iPhase == 13) { SetVertex(0, 0, 33 * iDir, 0, 1); SetVertex(0, 1, -1, 0, 2); }
    if (iPhase == 14) { SetVertex(0, 0, 34 * iDir, 0, 1); SetVertex(0, 1, -5, 0, 2); }
  }
  if (strAction S= "WalkTurn")
  {
    if (iPhase == 0) { SetVertex(0, 0, 34 * iDir, 0, 1); SetVertex(0, 1, -7, 0, 2); }
    if (iPhase == 1) { SetVertex(0, 0, 32 * iDir, 0, 1); SetVertex(0, 1, -7, 0, 2); }
    if (iPhase == 2) { SetVertex(0, 0, 28 * iDir, 0, 1); SetVertex(0, 1, -6, 0, 2); }
    if (iPhase == 3) { SetVertex(0, 0, 22 * iDir, 0, 1); SetVertex(0, 1, -5, 0, 2); }
    if (iPhase == 4) { SetVertex(0, 0, 16 * iDir, 0, 1); SetVertex(0, 1, -4, 0, 2); }
    if (iPhase == 5) { SetVertex(0, 0, 10 * iDir, 0, 1); SetVertex(0, 1, -5, 0, 2); }
    if (iPhase == 6) { SetVertex(0, 0, 3 * iDir, 0, 1); SetVertex(0, 1, -6, 0, 2); }
    if (iPhase == 7) { SetVertex(0, 0, -3 * iDir, 0, 1); SetVertex(0, 1, -7, 0, 2); }
    if (iPhase == 8) { SetVertex(0, 0, -10 * iDir, 0, 1); SetVertex(0, 1, -7, 0, 2); }
    if (iPhase == 9) { SetVertex(0, 0, -15 * iDir, 0, 1); SetVertex(0, 1, -6, 0, 2); }
    if (iPhase == 10) { SetVertex(0, 0, -19 * iDir, 0, 1); SetVertex(0, 1, -5, 0, 2); }
    if (iPhase == 11) { SetVertex(0, 0, -23 * iDir, 0, 1); SetVertex(0, 1, -5, 0, 2); }
    if (iPhase == 12) { SetVertex(0, 0, -27 * iDir, 0, 1); SetVertex(0, 1, -5, 0, 2); }
    if (iPhase == 13) { SetVertex(0, 0, -30 * iDir, 0, 1); SetVertex(0, 1, -6, 0, 2); }
    if (iPhase == 14) { SetVertex(0, 0, -33 * iDir, 0, 1); SetVertex(0, 1, -7, 0, 2); }
  }
  if (strAction S= "Sleep")
  {
    if (iPhase == 0) { SetVertex(0, 0, 34 * iDir, 0, 1); SetVertex(0, 1, -7, 0, 2); }
    if (iPhase == 1) { SetVertex(0, 0, 35 * iDir, 0, 1); SetVertex(0, 1, -5, 0, 2); }
    if (iPhase == 2) { SetVertex(0, 0, 36 * iDir, 0, 1); SetVertex(0, 1, -3, 0, 2); }
    if (iPhase == 3) { SetVertex(0, 0, 36 * iDir, 0, 1); SetVertex(0, 1, 0, 0, 2); }
    if (iPhase == 4) { SetVertex(0, 0, 36 * iDir, 0, 1); SetVertex(0, 1, 4, 0, 2); }
    if (iPhase == 5) { SetVertex(0, 0, 35 * iDir, 0, 1); SetVertex(0, 1, 9, 0, 2); }
    if (iPhase == 6) { SetVertex(0, 0, 34 * iDir, 0, 1); SetVertex(0, 1, 13, 0, 2); }
    if (iPhase == 7) { SetVertex(0, 0, 32 * iDir, 0, 1); SetVertex(0, 1, 18, 0, 2); }
    if (iPhase == 8) { SetVertex(0, 0, 30 * iDir, 0, 1); SetVertex(0, 1, 22, 0, 2); }
    if (iPhase == 9) { SetVertex(0, 0, 28 * iDir, 0, 1); SetVertex(0, 1, 26, 0, 2); }
    if (iPhase == 10) { SetVertex(0, 0, 26 * iDir, 0, 1); SetVertex(0, 1, 29, 0, 2); }
    if (iPhase == 11) { SetVertex(0, 0, 24 * iDir, 0, 1); SetVertex(0, 1, 32, 0, 2); }
    if (iPhase == 12) { SetVertex(0, 0, 23 * iDir, 0, 1); SetVertex(0, 1, 34, 0, 2); }
    if (iPhase == 13) { SetVertex(0, 0, 22 * iDir, 0, 1); SetVertex(0, 1, 35, 0, 2); }
    if (iPhase == 14) { SetVertex(0, 0, 21 * iDir, 0, 1); SetVertex(0, 1, 36, 0, 2); }
  }
  if (strAction S= "Glide")
  {
    if (iPhase == 0) { SetVertex(0, 0, 41 * iDir, 0, 1); SetVertex(0, 1, -8, 0, 2); }
    if (iPhase == 1) { SetVertex(0, 0, 41 * iDir, 0, 1); SetVertex(0, 1, -8, 0, 2); }
    if (iPhase == 2) { SetVertex(0, 0, 41 * iDir, 0, 1); SetVertex(0, 1, -7, 0, 2); }
    if (iPhase == 3) { SetVertex(0, 0, 41 * iDir, 0, 1); SetVertex(0, 1, -7, 0, 2); }
    if (iPhase == 4) { SetVertex(0, 0, 41 * iDir, 0, 1); SetVertex(0, 1, -7, 0, 2); }
    if (iPhase == 5) { SetVertex(0, 0, 41 * iDir, 0, 1); SetVertex(0, 1, -7, 0, 2); }
    if (iPhase == 6) { SetVertex(0, 0, 40 * iDir, 0, 1); SetVertex(0, 1, -7, 0, 2); }
    if (iPhase == 7) { SetVertex(0, 0, 40 * iDir, 0, 1); SetVertex(0, 1, -7, 0, 2); }
    if (iPhase == 8) { SetVertex(0, 0, 40 * iDir, 0, 1); SetVertex(0, 1, -7, 0, 2); }
    if (iPhase == 9) { SetVertex(0, 0, 40 * iDir, 0, 1); SetVertex(0, 1, -7, 0, 2); }
    if (iPhase == 10) { SetVertex(0, 0, 40 * iDir, 0, 1); SetVertex(0, 1, -7, 0, 2); }
    if (iPhase == 11) { SetVertex(0, 0, 40 * iDir, 0, 1); SetVertex(0, 1, -7, 0, 2); }
    if (iPhase == 12) { SetVertex(0, 0, 40 * iDir, 0, 1); SetVertex(0, 1, -7, 0, 2); }
    if (iPhase == 13) { SetVertex(0, 0, 40 * iDir, 0, 1); SetVertex(0, 1, -7, 0, 2); }
    if (iPhase == 14) { SetVertex(0, 0, 40 * iDir, 0, 1); SetVertex(0, 1, -8, 0, 2); }
  }
  if (strAction S= "Tumble") // *name manually adjusted
  {
    if (iPhase == 0) { SetVertex(0, 0, 34 * iDir, 0, 1); SetVertex(0, 1, -7, 0, 2); }
    if (iPhase == 1) { SetVertex(0, 0, 34 * iDir, 0, 1); SetVertex(0, 1, -10, 0, 2); }
    if (iPhase == 2) { SetVertex(0, 0, 32 * iDir, 0, 1); SetVertex(0, 1, -15, 0, 2); }
    if (iPhase == 3) { SetVertex(0, 0, 30 * iDir, 0, 1); SetVertex(0, 1, -19, 0, 2); }
    if (iPhase == 4) { SetVertex(0, 0, 28 * iDir, 0, 1); SetVertex(0, 1, -24, 0, 2); }
    if (iPhase == 5) { SetVertex(0, 0, 25 * iDir, 0, 1); SetVertex(0, 1, -28, 0, 2); }
    if (iPhase == 6) { SetVertex(0, 0, 21 * iDir, 0, 1); SetVertex(0, 1, -32, 0, 2); }
    if (iPhase == 7) { SetVertex(0, 0, 18 * iDir, 0, 1); SetVertex(0, 1, -34, 0, 2); }
    if (iPhase == 8) { SetVertex(0, 0, 15 * iDir, 0, 1); SetVertex(0, 1, -35, 0, 2); }
    if (iPhase == 9) { SetVertex(0, 0, 13 * iDir, 0, 1); SetVertex(0, 1, -36, 0, 2); }
    if (iPhase == 10) { SetVertex(0, 0, 15 * iDir, 0, 1); SetVertex(0, 1, -35, 0, 2); }
    if (iPhase == 11) { SetVertex(0, 0, 21 * iDir, 0, 1); SetVertex(0, 1, -32, 0, 2); }
    if (iPhase == 12) { SetVertex(0, 0, 28 * iDir, 0, 1); SetVertex(0, 1, -28, 0, 2); }
    if (iPhase == 13) { SetVertex(0, 0, 34 * iDir, 0, 1); SetVertex(0, 1, -21, 0, 2); }
    if (iPhase == 14) { SetVertex(0, 0, 40 * iDir, 0, 1); SetVertex(0, 1, -13, 0, 2); }
  }
}

private func Unstick(bool fReverseRot)
{
  var iReverseRot;
  if (fReverseRot) iReverseRot=-1; else iReverseRot=1;
  // Erstmal etwas anheben
  SetPosition(GetX(), GetY()-7);
  // Drehung je nach Festhängepunkt
  var iRot, iOldRot=GetR();
  if (GetContact(0, 1, 16)) --iRot;
  if (GetContact(0, 2, 16)) ++iRot;
  if (iRot) SetR(BoundBy(GetR()+iRot*(GetDir()*20-10)*iReverseRot, -45, 45));
  // Hängt noch?
  if (!Stuck()) return(1);
  // Zurücksetzen
  SetPosition(GetX(), GetY()+7);
  SetR(iOldRot);
  // Kein Erfolg
  return();
}

private func UnstickTiny()
{
  if (Stuck()) SetPosition(GetX(), GetY()-2);
  return(1);
}

/*private func UnstickEx(int iRRange, int iYRange) - Diese Funktion steht im Verdacht, für das komplette Umkippen
{                                                    des Drachens verantwortlich zu sein...
  var x=GetX(), y=GetY(), r=GetR(), dy;
  while ((dy+=5)<=iYRange)
  {
    // Verschiebung testen
    SetPosition(x, y-dy);
    if (!Stuck()) return(1);
    // In verschiedenen Drehungen testen
    if (!iRRange) continue; 
    SetR(r+iRRange); if (!Stuck()) return(1);
    SetR(r-iRRange); if (!Stuck()) return(1);
    SetR(r);
  }
  // Fehlschlag
  SetPosition(x, y);
  return();
}*/

protected func FxStuckCheckTimer(object target, int number)
{
  // Steckt fest
  if (Stuck())
  {
    var fUpdate = false;
    if (StuckAtHead())
      if (iHeadDrawIn < 40)
        { iHeadDrawIn++; fUpdate = true; }
    if (StuckAtTail())
      if (iTailDrawIn < 50)
        { iTailDrawIn++; fUpdate = true; }
    /*if (StuckAtFeet())
      if (iFeetDrawIn < 20)
        { iFeetDrawIn++; fUpdate = true; }*/
    if (StuckAtTop())
      if (iTopDrawIn < 20)
        { iTopDrawIn++; fUpdate = true; }
    if (fUpdate) 
      UpdateVertices();
  }
  // Frei
  else
  {
    if (iHeadDrawIn) iHeadDrawIn--;
    if (iTailDrawIn) iTailDrawIn--;
    if (iFeetDrawIn) iFeetDrawIn--;
    if (iTopDrawIn) iTopDrawIn--;
  }
}

private func StuckAtHead()
{
  return (GBackSolid(GetVertex(1, 0), GetVertex(1, 1)));
}

private func StuckAtTail()
{
  return (GBackSolid(GetVertex(2, 0), GetVertex(2, 1)));
}

private func StuckAtFeet()
{
  return (GBackSolid(GetVertex(3, 0), GetVertex(3, 1)));
}

private func StuckAtTop()
{
  return (GBackSolid(GetVertex(4, 0), GetVertex(4, 1)));
}

/*-------------------------------------------------- Schaden ---------------------------------------------------------*/

protected func Damage(int iChange)
{
  // Nur, wenn wirklich Schaden erlitten
  if (iChange > 0)
  {
    // Energie anpassen
    DoEnergy(-iChange);
    // Hörbares Feedback geben
    Sound("DragonGrowl");
    // Schadensverhalten
    DamageReaction(iChange);
  }
}

protected func RejectCollect(id def, object obj)
{
  // Bewegt sich das Objekt?
  if( (obj->GetOCF() & OCF_HitSpeed1) && (~obj->GetCategory() & C4D_StaticBack) )
  {
    // Checken, ob in einem gewissen Bereich
    if (IsOutsideArrowHitZone(obj)) return true;
    // TODO: Sachen abprallen lassen?
    ProjectileHit(obj);
    // Von Gegenstand getroffen
    if (GetOCF(obj) & OCF_Collectible())
      // Kollision im Gegenstand aufrufen
      PrivateCall(obj, "Hit");
  }

  // Drachen nehmen nichts auf.
  return(true);
}

private func ProjectileHit(object obj)
{
  // Bewegungsvektor
  var ang = Angle(0, 0, obj->GetXDir(), obj->GetYDir() );
  // Staerke abhaengig von der Geschwindigkeit und Masse
  var strength = (obj->GetXDir()**2 + obj->GetYDir()**2) * obj->GetMass()/20;
  // Bewegungsfaktor
  var x = Sin(ang, strength), y = -Cos(ang, strength);
  // Schaden verursachen
  var damage = Sqrt(strength) / 25;
  Damage(damage);
  // Schlafende Drachen aufwecken
  if (IsSleeping())
    WakeUp();
  // Frei rumlaufen, wenn kein Reiter (Drachen lassen sich nicht bewerfen)
  if (IsControlled() && !GetRider())
    SetFree();
  // Drache wird wuetend
  iAnger += damage * 10;
}

// Pfeiltreffer - neben regulärem Pfeilschaden noch extra Projektiltreffer für die Reaktion
public func OnArrowHit(object arrow)
{
  ProjectileHit(arrow);
}

public func IsOutsideArrowHitZone(object arrow)
{
  // Kleineres Collection-Rechteck bei kleinerem Drachen
  var coll_x = GetDefCoreVal("Collection", "DefCore", GetID(), 0) * GetCon() / 100;
  var coll_y = GetDefCoreVal("Collection", "DefCore", GetID(), 1) * GetCon() / 100;
  var coll_width = GetDefCoreVal("Collection", "DefCore", GetID(), 2) * GetCon() / 100;
  var coll_height = GetDefCoreVal("Collection", "DefCore", GetID(), 3) * GetCon() / 100;
  // Gucken, ob in dem Rechteck drin
  var obj_rel_x = arrow->GetX() - GetX(), obj_rel_y = arrow->GetY() - GetY();
  if(obj_rel_x < coll_x || obj_rel_x > coll_x + coll_width)
    return(true);
  if(obj_rel_y < coll_y || obj_rel_y > coll_y + coll_height)
    return(true);
}

public func IsArrowTarget() { return true; }

public func GetArrowStickTime() { return 36; } // Nur kurz stecken, da sich der Drache schnell aus dem Pfeil "rausanimiert"

/*----------------------------------------------------- Tod ----------------------------------------------------------*/

public func Death()
{
  // Geräusch
  Sound("DragonDeath");
  // Inhalt auswerfen
  while (Contents()) Exit(Contents());
  // Reiter abwerfen
  if (GetRider()) DropRider();
  // Todesaktion
  SetAction("Dead");
  // Alive-Flag löschen
  SetAlive(0);
  return(1);
}

public func DeathConversion()
{
  // Verwandeln
  ChangeDef(DRGD);
  SetAction("Dead");
  SetPhase(0);
  SetR(0);
  ResetVertices();
  return(1);
}

/*-------------------------------------------------- Verhalten -------------------------------------------------------*/

private func Activity() // TimerCall, alle 25 Frames
{
  // In Flüssigkeit: auftauchen
  if (InLiquid())
    SetCommand(this(), "MoveTo", 0, GetX(), GetY()-100);
  // Mit der Zeit wieder beruhigen
  if (iAnger) --iAnger;
}  

public func IsControlled()
{
  return(GetEffect("IntDragonFree", this()) == 0);
}

public func IsFree()
{
  return(!IsControlled() );
}

public func SetControlled(bool fNoSound)
{
  // Generelles Feedback beim Wechsel in den kontrollierten Modus
  if (IsFree())
    if (!fNoSound)
      Sound("DragonGrowl");
  // DragonFree-Effekt weg
  if (GetEffect("IntDragonFree", this()) )
    RemoveEffect("IntDragonFree", this());
  // DragonControlled-Effekt dazu
  if (!GetEffect("IntDragonControlled", this()) )
    AddEffect("IntDragonControlled", this(), 1, 35, this() );
  return(1);
}

public func SetFree(bool fNoSound)
{
  // Generelles Feedback beim Wechsel in den freien Modus
  if (IsControlled())
    if (!fNoSound)
      Sound("DragonRoar");  
  // DragonFree-Effekt dazu
  if (!GetEffect("IntDragonFree", this()) )
    AddEffect("IntDragonFree", this(), 1, 25, this() );
  // DragonControlled-Effekt weg
  if (GetEffect("IntDragonControlled", this()) )
    RemoveEffect("IntDragonControlled", this() );
  return(1);
}

private func ResetControl() // Bei jeder Art von eingehendem Steuerungsbefehl
{
  // Schlafend: aufwachen
  if (IsSleeping())
    WakeUp();
  // Kontrollierter Modus
  SetControlled();
  // Zaehler resetten, damit der Drache nicht in den Free Mode verfaellt
  var effect_number = GetEffect("IntDragonControlled", this() );
  if(effect_number) EffectCall(this(), effect_number, "Reset");
  // Kommandos löschen
  SetCommand(this(), "None");
}

public func GoSleep(object caller)
{
  // Reiter haben auf dem Drachen nix verloren, wenn dieser schlaeft
  if(GetRider() ) DropRider();
  Walk2Action("GoSleep");
}

public func WakeUp()
{
  return (SetAction("WakeUp"));
}

public func MoveTo(int x, int y, bool dont_adjust)
{
  // Punkt auf der Erdoberflaeche bestimmen, wenn in Erde
  var horizont = HorizonTop(x, y);
  // Nicht moeglich? Dann lassen wir das MoveTo besser.
  if(horizont == -1) return(false);

  // Hat sich der Punkt geaendert? Dann ist er wohl auf der Oberflaeche.
  // Also gehen wir noch ein Stueck weiter rauf.
  if(horizont != y)
    y = horizont - GetObjHeight()/2;

  // Passen wir uns auch nach unten an?
  if(!dont_adjust)
  {
    // Punkt unten bestimmen
    horizont = HorizonBottom(x, y);
    if(horizont == -1)
    {
      // Nicht moeglich.
      if(!GetScenarioVal("BottomOpen", "Landscape") )
        // Bei Nicht offener Landschaft koennen wir einfach nach
        // ganz unten fliegen.
        horizont = LandscapeHeight();
      else
        // Ansonsten lassen wirs wohl auch besser.   
        return(false);
    }
    // Sonst ist hier der gesuchte Punkt.
    y = horizont - (GetDefHeight(GetID() ) * GetCon() / 100)/3;
  }

  // Zu hoch fuer den Drachen? An den Punkt kommt er nie, wegen BorderBound.
  if(y < GetObjHeight() ) return(false);

  // Sonst, auf gehts.
  SetCommand(this(), "MoveTo", 0, x, y, 0, C4CMD_MoveTo_NoPosAdjust);
  AppendCommand(this(), "Call", this(), 0,0,0,0, "StopComDir");

  return(true);
}

public func StopComDir() { return(SetComDir(COMD_Stop()) ); }

public func DamageReaction(int iDamage)
{
  // Keine Schadensreaktion mit Reiter
  if (GetRider())
    return(0);
  
  // Zufallsposition finden, zum Ausweichen
  var x = BoundBy(GetX() - 150 + Random(301), GetObjWidth() + 10, LandscapeWidth() - GetObjWidth() - 10);
  var y = BoundBy(GetY() - 150 + Random(301), GetObjHeight() + 10, LandscapeHeight() - GetObjHeight() - 10);
  // An den Boden anpassen, wenn wir nicht fliegen
  var adjust = !IsFlying();
  // Oder wenn wir landen wollen
  if (IsFlying() && !Random(8) ) adjust = true;
  return(MoveTo(x, y, !adjust));
}

protected func FxIntDragonFreeTimer(object target, int number, int time)
{
  // Kein Zuhause?
  if(!pHome)
  {
    // Eines suchen
    pHome = FindObject(DLAR, 0, 0, -1, -1);
    // Zu weit weg?
    if(ObjectDistance(pHome) > 500)
      pHome = 0;
  }

  // Schlafen wir? Dann evtl. aufwachen
  if (IsSleeping())
    if (!Random(DRGN_WakeUp_Chance))
      return(WakeUp());
    else
      return(0);

  // MoveTo-Dauer bemessen
  if (GetCommand() S= "MoveTo")
    iMoveToTime++;
  else
    iMoveToTime = 0;

  // Ein freier Drache vergisst nach einer Maximaldauer sein
  // MoveTo-Kommando, damit er nicht unbegrenzt lange an
  // nicht ausführbaren Kommandos festhängt
  if (iMoveToTime > DRGN_FreeMoveTo_Timeout)
    ClearCommand();

  // Haben wir noch was zu tun? Dann lassen wirs erstmal..
  if (GetCommand()) return();
  
  // Nach Hause?
  if(!Random(DRGN_ReturnHome_Chance) && pHome)
    return(MoveTo(pHome->GetX(), pHome->GetY() ));
  // Einfach so rumfliegen/laufen
  if (IsFlying() || !Random(DRGN_GeneralMove_Chance))
  {
    // Zufallsposition finden, zum Hinfliegen
    var x = BoundBy(GetX() - 150 + Random(301), GetObjWidth() + 10, LandscapeWidth()-GetObjWidth() - 10);
    var y = BoundBy(GetY() - 150 + Random(301), GetObjHeight() + 10, LandscapeHeight() - GetObjHeight() - 10);

    // An den Boden anpassen, wenn wir nicht fliegen
    var adjust = !IsFlying();
    // Oder wenn wir landen wollen
    if(IsFlying() && !Random(8) ) adjust = true;
    return(MoveTo(x, y, !adjust) );
  }
  // Losfliegen
  if(!Random(DRGN_TakeOff_Chance) && GetAction() S= "Walk")
    return(TakeOff() );
  // Im Fliegen kann man weder gucken noch schlafen
  if (!IsFlying())
  {
    // Gucken
    if(!Random(DRGN_Look_Chance) && GetAction() S= "Walk")
    {
      Walk2Action("Look");
      SetCommand(this(), "Wait", 0,0,0,0, 10);
      // Walk2Action scheint eine ComDir zu setzen, die es dann nicht zurueck
      // setzt, dadurch laeuft der Drache dauernd gegen den Bildschirmrand.
      AppendCommand(this(), "Call", this(), 0,0,0,0, "StopComDir");
      AppendCommand(this(), "Wait", 0,0,0,0, 40);
      return(true);
    }

    // Schlafen    
    if (!Random(DRGN_GoSleep_Chance))
      if (!IsAngry())
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
          Walk2Action("GoSleep");
        }
        return(true);
      }
  }
}

protected func FxIntDragonControlledTimer(object target, int number)
{
  // Zaehler erhoehen
  EffectVar(0, target, number) ++;
  // Ist der Reiter schon zu lange untaetig? Dann wieder in den Free Mode
  // wechseln. Auch wechseln, wenn wir fuehrerlos fliegen, es macht sich
  // naemlich nicht so toll, wenn der Drache ewig in der Luft stehen bleibt
  if(EffectVar(0, target, number) >= 60 || (!GetRider() && IsFlying()))
    SetFree();
}

protected func FxIntDragonControlledReset(object target, int number)
{
  // Den Zaehler bei eingehender Steuerung zuruecksetzen
  EffectVar(0, target, number) = 0;
}

/* Private Hilfsfunktionen */

private func HorizonTop(int x, int y)
{
  for(;y >= 0; -- y)
  {
    if(!GBackSemiSolid(x - GetX(), y - GetY()) )
    {
       // Nur wenn auch genug Platz da ist.
       if(PathFree(x, y, x, y - GetObjHeight()) )
       {
         return(y);
       }
       else
       {
         // TODO: Wenn nicht evtl. nach unten weiterschauen?
       }
    }
  }
  return(-1);
}

private func HorizonBottom(int x, int y)
{
  for(;y < LandscapeHeight(); ++ y)
  {
    if(GBackSemiSolid(x - GetX(), y - GetY()) )
    {
      // Nur wenn auch genug Platz da ist.
      if(PathFree(x, y - 1, x, y - GetObjHeight()) )
      {
        return(y - 1);
      }
    }
  }
  return(-1);
}

public func ClearCommand()
{
  SetCommand(this(), "None");
}
