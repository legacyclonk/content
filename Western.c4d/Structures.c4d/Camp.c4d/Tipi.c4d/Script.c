/*-- Tipi --*/

#strict 2

local pBuild;
local iColor;

protected func Construction() { return(SetAction("Wait")); }

protected func StartConstruction()
{
  if(GetCon() == 100) return(1);

  if(!iColor) SetRndGraphics();
  // Um zusätzliche Siedlungspunkte zu vermeiden
  pBuild = CreateObject(GetID(),0,0,-1);
  pBuild->~SetTentGraphics(iColor);
  ObjectSetAction(pBuild, "Construction", this());
  AddEffect("Build", pBuild, 1, 1, pBuild);
  SetObjectLayer(this(), pBuild);
  SetClrModulation(RGBa(0,0,0,255));
  return(1);
}

protected func Initialize()
{
  // Damit nach dem Bauen nicht der "CommandFailure1"-Sound erscheint, wird das richtig-gebaute Tipi genommen
  if(pBuild) {
    SetPosition(GetX(pBuild),GetY(pBuild));
    RemoveObject(pBuild);
  }
  if(!iColor) SetRndGraphics();
  SetClrModulation();
  SetEntrance(1);
  SetAction("Idle");
  this()->~Constructed();
  return(1);
}

public func FxBuildTimer(pTarget, iEffectNumber, iEffectTime)
{
  var iObj = GetActionTarget(0,pTarget);
  if(!iObj) {
    RemoveObject(pTarget);
    return(-1);
  }
  SetPhase(GetCon(iObj)/5,pTarget);
  // Noch ein Check, damit das Zelt sich beim Anbrennen schwarz verfärbt und danach noch fehlerfrei wiederaufgebaut werden kann.
  if(!EffectVar(1,pTarget,iEffectNumber)) {
    if(OnFire(iObj)) {
      SetClrModulation(RGB(50,50,50),pTarget);
      EffectVar(1,pTarget,iEffectNumber) = 1;
    }
  }
  else if(!OnFire(iObj) && EffectVar(1,pTarget,iEffectNumber))
    if(GetID(iObj) == GetID(pTarget)) {
      LocalN("pBuild", iObj) = pTarget;
      LocalN("iColor", iObj) = LocalN("iColor", pTarget);
      SetClrModulation(0,pTarget);
      EffectVar(1,pTarget,iEffectNumber) = 0;
    }
}

func SetTentGraphics(iIndex)
{
  iColor = iIndex;
  if(iColor > 1) SetGraphics(Format("%d", iColor));
  else SetGraphics(0);
}

func SetRndGraphics()
{
  iColor = Random(GetMaxGraphics())+1;
  if(iColor > 1) SetGraphics(Format("%d", iColor));
  else SetGraphics(0);
}

protected func Destruction() { if(pBuild) RemoveObject(pBuild); }

public func GetMaxGraphics() { if(GetID()==TIPI) return(3); return(1); }

public func GetResearchBase() { return(ROPE); }

/* Kann vom Indianer hergestellt werden */
public func IsIndianHandcraft() { return(1); }

/* Kann von Indianern erforscht werden */
public func IsIndianResearch() { return(1); }

/* Ist ein Zelt */
public func IsTent() { return(1); }