/*-- Schlitten --*/

#strict

local crane, clamp, xdir;

protected func Initialize() {
  SetName(GetName(0,GetID()));
  clamp = CreateObject(CLMP, 0, 25, GetOwner());
  clamp->Set(this());
}

public func Set(object pCrane) {
  crane = pCrane;
  SetAction("Sliding", clamp);
  Adjust();
}

public func Adjust() {
  // Position anpassen
  var iX = GetX(), iY;
  // Etwas unterhalb der Schiene
  iY = GetY(crane)+5;
  // X-Wert nicht gut
  if(NotInRange()) iX = Back2Crane();

  SetPosition(iX, iY);
}

public func NotInRange() {
  if(!Inside(GetX(), crane->~LeftX()+10, crane->~RightX()-10))
    return(1);
}

public func Back2Crane() {
  return(BoundBy(GetX(), crane->~LeftX()+10, crane->~RightX()-10));
}

/* Konsolensteuerung */

public func ConsoleControl(int i) {
  if(i == 1) return("$ControlCrane$");
}

public func ConsoleControlled(int i, int Clonk) {
  var pClonk = Object(Clonk);
  if(!pClonk) return();
  // Wird bereits kontrolliert?
  if(GetEffect("Controller", this())) return(Sound("Error", 0, pClonk, 100, GetOwner(pClonk)+1));
  // Steuerung übernehmen
  AddEffect("Controller", this(), 99, 2, this(), 0, GetOwner(pClonk), pClonk);
  SetCursor(GetOwner(pClonk), this(), 1,1);
}

public func FxControllerStart(object target, int no, int temp, int owner, object clonk) {
  if(temp) return();
  EffectVar(0, target, no) = owner;
  EffectVar(1, target, no) = clonk;
}

public func FxControllerTimer(target, no) {
  if(GetCursor(EffectVar(0, target, no)) != target || !EffectVar(1, target, no))
  {
    target->Stop();
    return(-1);
  }
  if(EffectVar(1, target, no)->GetAction() ne "Push" ||
     EffectVar(1, target, no)->GetActionTarget()->~IsConsole() != true)
   {
     target->Stop();
     SetCursor(EffectVar(0, target, no), EffectVar(1, target, no));
     return(-1);
   }
  // Bewegung überprüfen
  target->~CheckMovement();
}

func CrewSelection(bool deselect, bool cursoronly)
{ 
  if(deselect)
  {
    RemoveEffect("Controller2",this());
    Stop();
  }
  else
  {
    AddEffect("Controller2", this(), 99, 2, this(), 0, GetController(), this());
    SetPlrView(GetController(), clamp);
  }
}

func FxController2Effect(string neweffect)
{
  if(neweffect S= "Controller2")
    return(-1);
}

func FxController2Timer()
{
  CheckMovement();
}

/* Steuerung */

public func ControlLeft(object obj) {
  if(xdir > -2)
    xdir -= 2;
  
  SetPlrView(GetController(obj), clamp);
}

public func ControlRight(object obj) {
  if(xdir < 2)
    xdir += 2;
    
  SetPlrView(GetController(obj), clamp);
}

public func ControlUp(object obj) {
  clamp->Up();
  
  SetPlrView(GetController(obj), clamp);
}

public func ControlDown(object obj) {
  clamp->Down();
  
  SetPlrView(GetController(obj), clamp);
}

public func ControlThrow(object obj) {
  clamp->Grab();
  
  SetPlrView(GetController(obj), clamp);
}

public func ControlDig(object obj) {
  xdir = 0;
  clamp->Stop();
		
  SetPlrView(GetController(obj), clamp);
}

public func CheckMovement() {
  // Außerhalb der Reichweite
  if(NotInRange()) { Adjust(); xdir=0; }
  // Der Greifer stößt irgendwo gegen
  if(clamp)
    if(clamp->GBackSolid(xdir * 3, 0))
      xdir = 0;
  // Bei Bewegung Geräusch
  if(xdir != 0) Sound("Elevator", 0, this(), 50, 0, 1);
  else Sound("Elevator", 0, this(), 50, 0, -1);

  SetPosition(GetX() + xdir, GetY());
}

public func CheckY() {
  // Überprüft, ob die Schiene weg ist
  if(!crane) return(RemoveObject());
  if(GetY() != GetY(crane)+10) Adjust();
  // Die Schiene sollte eigentlich nicht bewegt werden :0
}

public func Stop() {
  xdir = 0;
  Sound("Elevator", 0, this(), 100, 0, -1);
  clamp->LocalN("ydir") = 0;
}

// Maus macht mich zZ kaputt
protected func ControlCommand() { return(1); }

/* Serialisierung */

public func NoSerialize() { return !false; }

public func Reconfigure(int dx, int dy, object load)
{
	SetPosition(GetX()+dx, GetY());
	clamp->SetPosition(GetX(clamp), GetY(clamp)+dy);
	if (load) {
		clamp->LocalN("load") = load;
		clamp->SetAction("Load");
		clamp->AddEffect("Loading", load, 99, 1, clamp, 0, GetX(load)-GetX(clamp), GetY(load)-GetY(clamp));
	}
}
