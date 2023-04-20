/*-- Haken --*/

#strict

local slide, ydir, load;

func Initialize()
{
  AddLightAmbience(45);
}

public func Set(object pSlide) {
  slide = pSlide;
//  CreateObject(STLC, 0,0, GetOwner())->Connect(this(), pSlide);
  SetAction("NoLoad");
  // Effekt, der Bewegung steuert
  AddEffect("Movement", this(), 99, 2, this());
}

public func Down() {
  if(ydir < 2)
    ydir += 2;
}

public func Up() {
  if(ydir > -2)
    ydir -= 2;
}

public func Stop() {
  ydir = 0;
}

/* Bewegung */

public func FxMovementTimer() {
  // Geschwindigkeiten
  var y = BoundBy(GetY() + ydir, GetY(slide) + 10, LandscapeHeight());
  if(CheckSolid(y))
  {
    y = GetY();
    ydir = 0;
  }
	SetPosition(GetX(slide), y);
}

public func CheckSolid(int iY) {
  if(!load)
    return(GBackSolid(0,iY-GetY()));
  iY = iY - GetY();
  var y = load->GetY() + load->GetObjHeight() + load->GetDefOffset(0,1) + iY;
  if(iY < 0)
    y = load->GetY() + load->GetDefOffset(0,1) + iY;
  return(GBackSolid(0,y-GetY()));
}

/* Objekte schnappen */

public func Grab() {
  // Hat schon eines?
  if(GetAction() eq "Load") return(Ungrab());

  var obj;
  obj = FindObjects(Find_Or(Find_AtPoint(0,0), Find_AtPoint(0,20)), Find_NoContainer(), Find_Func("IsCraneGrabable"), Find_Exclude(this()));
  for(var pObj in obj)
    if(GetProcedure(pObj) ne "FLOAT")
      if(!pObj->~NoCrane())
      {
        load = pObj;
        SetAction("Load");
        Sound("Grapple");
        AddEffect("Loading", load, 99, 1, this(), 0, GetX(load)-GetX(), GetY(load)-GetY());
        return(1);
      }
}

public func FxLoadingStart(object target, int no, int temp, int x, int y) {
  if(temp) return();
  EffectVar(0, target, no) = x;
  EffectVar(1, target, no) = y;
}

public func FxLoadingTimer(object target, int no) {
  // Zielposition setzen
  SetPosition(GetX()+EffectVar(0, target, no), GetY()+EffectVar(1, target, no), target);
  SetXDir(0, target);
  SetYDir(0, target);
}

public func Loading() {
  // Kein Ziel mehr?
  if(!load) return(SetAction("NoLoad"));
  // Alles gut
}

public func Ungrab() {
  if(!load) return();
  // Ziel loslassen
  SetAction("NoLoad");
  if(GetEffect("Loading", load)) RemoveEffect("Loading", load);
  load = 0;
  Sound("Grapple");
}

/* Serialisierung */

public func NoSerialize() { return !false; }
