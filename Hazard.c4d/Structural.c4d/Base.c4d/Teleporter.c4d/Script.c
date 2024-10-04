/*-- Teleporter --*/
// Newton: einiges erneuert in Hazard 2.1

#strict 2

local pCurrentTarget;  //Zweiter Teleporter (Ziel)
local pLight;
local isPrivate;

func Initialize() 
{
  SetAction("Activated");
  SetEntrance(0);
  isPrivate = false;

  pLight = AddLight(100,RGBa(100,220,255));
}

func ActivateEntrance(object clonk)
{
  SetCommand(clonk, "None");
  if(GetAction() != "Activated")
  {
    PlayerMessage(clonk->GetOwner(), "$Inactive$", this);
    return;
  }
  CheckBeam(clonk);
}

func CheckBeam(object clonk)
{
  // Haben wir ein Ziel?
  if(!pCurrentTarget)
  {
    Sound("Error");
    if(clonk) PlayerMessage(clonk->GetOwner(), "$NoTarget$", this);
    return;
  }
  // Ziel nicht aktiv?
  if(pCurrentTarget->GetAction() != "Activated")
  {
    Sound("Error");
    if(clonk) PlayerMessage(clonk->GetOwner(), "$TargetInactive$", this);
    return;
  }
  BeamObjects();
}

func BeamObjects()
{
  for(var clonk in FindObjects(Find_InRect(-11, -13, 22, 43),
                               Find_NoContainer(),
                               Find_Not(Find_Category( C4D_Structure | C4D_StaticBack | C4D_Vehicle)),
                               Find_Exclude(this)))
  {
    Beam(clonk, pCurrentTarget);
    SetAction("Beam");
  }
}

global func Beam(object clonk, object target, int x, int y)
{
  if(!clonk) return;

  AddEffect("Beam", clonk, 180, 1, 0, TELE, target, x, y);
}

func Activity()
{
  //Tolle Effekte bitte hierher.
  // kk!
  var x,y,xdir,ydir;
  for(var i = -7; i < 9; i += 3)
  {
    x = Cos(GetR()+90+i, 23);
    y = Sin(GetR()+90+i, 23);
    xdir = -x/3;
    ydir = -y/3;
    CreateParticle("PSpark", x, y, xdir, ydir, 50, RGB(152, 209, 210));
  }
}

private func CreateBeamSpark(int x, int y, int width, int height)
{
    x = RandomX( x - width / 2, x + width / 2);
    y = RandomX( y - height / 2, y + height / 2);
    
    CreateParticle("LsrSprk",x,y,0,-2,170,RGBa(188, 127, 220, 50));
}

public func FxBeamEffect(string name, object target)
{
  if(name == "Beam") return -1;
}

public func FxBeamStart(object target, int effectNumber, int temp, beamTo, x, y)
{
  if(temp) return;
  
  EffectVar(1, target, effectNumber) = GetClrModulation(target);   // color modulation
  EffectVar(2, target, effectNumber) = beamTo; // Objekt zu dem gebeamt wird
  if(beamTo)
  {
    x = beamTo->GetX();
    y = beamTo->GetY() + target->GetObjHeight()/2;
  }
  EffectVar(3, target, effectNumber) = x; // [opt] x-Position zu der gebeamt wird (wenn Objekt = null)
  EffectVar(4, target, effectNumber) = y; // [opt] y-Position zu der gebeamt wird (wenn Objekt = null)
  Sound("DeEnergize");
  SetCommand(target, "None");
}

public func FxBeamStop(object target, int effectNumber, int reason, bool temp)
{
  if(temp || reason) return;
  
  var clrModulation = EffectVar(1, target, effectNumber);
  var beamTarget = EffectVar(2, target, effectNumber);
  var beamToX = EffectVar(3, target, effectNumber);
  var beamToY = EffectVar(4, target, effectNumber);
  
  //Umsetzen und wiederherstellen
  target->SetPosition(beamToX, beamToY);
  target->SetClrModulation(clrModulation);
}

public func FxBeamTimer(object target, int effectNumber, int time)
{
  //Ausfaden
  var newVisibility = GetRGBaValue(target->GetClrModulation(), 0) + 5;
  SetObjAlpha(newVisibility, target);

  //Scheinbar sind wir weg. :O
  if(newVisibility > 230) return -1;

  var beamToX = EffectVar(3, target, effectNumber);
  var beamToY = EffectVar(4, target, effectNumber);
  
  //Partikelwowowo!
  for(var i = 0; i < Sqrt(time); ++i)
  {
    var width = target->GetObjWidth();
    var height = target->GetObjHeight();
    CreateBeamSpark(AbsX(target->GetX()), AbsY(target->GetY()), width, height);
    CreateBeamSpark(AbsX(beamToX), AbsY(beamToY), width, height);
  }
}

func MenuSetTarget(id icon, object target)
{
  SetTarget(target);
}

//Ziel setzen
func SetTarget(object target) 
{
  if(target->GetID() != TELE) return false;
  pCurrentTarget = target;
  return true;
}


//Ziel bestimmen
func SearchTargets(object clonk)
{
  //Gibt es jemanden, der die Konsole bedient?
  if(! clonk) return(0);
  //Menü bauen
  CreateMenu(HSGN, clonk, this());

  var teleporters = FindObjects(Find_ID(TELE),
                                Find_Exclude(this),
                                Find_Not(Find_Func("IsPrivateTeleporter")),
                                Find_Not(Find_Hostile(GetOwner())));
  for(var teleporter in teleporters)
  {
    AddMenuItem("$ChooseTarget$", "MenuSetTarget", GetID(), clonk, 0, teleporter);
  }
  // Effekt für tolles Feature
  AddEffect("TeleMenu", clonk, 101, 5, this, 0, teleporters);
}

//Effekt zur Anzeige der Teleporter, die zur Auswahl stehen
func FxTeleMenuStart(object target, int no, int temp, array targets)
{
  if(temp) return;
  EffectVar(0, target, no) = targets;
}

func FxTeleMenuTimer(object target, int no)
{
  // Menü weg?
  if(GetMenu(target) != HSGN) return -1;

  var i = GetMenuSelection(target);
  SetPlrView(target->GetOwner(), EffectVar(0, target, no)[i]);
}

//Konsolenkontrolle
public func ConsoleControl(int i)
{
  if(i == 1) return "$BeamMeUp$";
  if(i == 2)
  {
    if(GetAction() == "Deactivate") return "$TurnOn$";
    else                            return "$TurnOff$";
  }
  if(i == 3) return "$SearchTarget$";
}

public func ConsoleControlled(int i, int clonkNum, int consoleNum)
{
  var clonk = Object(clonkNum);
  
  if(i == 1) CheckBeam(clonk);
  if(i == 2)
  {
    if(GetAction() == "Deactivate") Activate();
    else                            Deactivate();
  }
  if(i == 3 && clonk) SearchTargets(clonk);
}

func Activate() {
  SetAction("Activate");
  pLight->TurnOn();
}

func Deactivate() {
  SetAction("Deactivate");
  pLight->TurnOff();
}

func IsPrivateTeleporter() {
  return isPrivate;
}

func SetPrivateTeleporter(bool private)
{
  isPrivate = private;
}

/* Serialisierung */

public func Serialize(array& extra)
{
  if (pCurrentTarget)
    extra[GetLength(extra)] = ["SetTarget(Object(%d))", pCurrentTarget];
  if (GetAction() == "Activated")
    extra[GetLength(extra)] = "Activate()";
}
