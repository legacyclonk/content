/*-- Fallensteuerung --*/

#strict 2

local fCollectible;

// Damit bestimmte Fallen in Gebäuden hergestellt werden können
//protected func Initialize()
protected func Construction()
{
  fCollectible = true;
  DeArmExtra();
  return(1);
}

public func ArmCheck(object pClonk)
{
  var i, iNum, iX, iY;
  if(!pClonk) pClonk = Contained();
  if(!pClonk) return(0);
  iX = (GetDir(pClonk)*2-1)*(GetDefWidth(GetID())+GetDefOffset(GetID())+GetObjWidth(pClonk)+GetObjectVal("Offset",0,pClonk)+3);
  iY = -GetDefOffset(GetID(pClonk),1);

  iNum = iY;
  for(i = 0; i < iNum; i++) if(!GBackSolid(iX,iY-i)) break;
  if(i == iNum) {
    PlayerMessage(GetOwner(pClonk),"$MsgNoArm$",this());
    return(0);
  }
  if(!i) {
    iNum = -10;
    for(i = 0; i > iNum; i--) if(GBackSolid(iX,iY-i-1)) break;
    if(i == iNum) {
      PlayerMessage(GetOwner(pClonk),"$MsgNoArm$",this());
      return(0);
    }
  }
  // Lebewesen/Fallen in der Nähe?
  if(FindObject2(Find_AtPoint(iX,iY-i-1),Find_Or(Find_OCF(OCF_Alive),Find_Func("IsArmed")),Find_Not(TrapIgnoreTargets()))) {
    PlayerMessage(GetOwner(pClonk),"$MsgNoArm$",this());
    return(0);
  }
  // Kein Metall vor Gebäuden gilt von nun an auch für Fallen
  if(FindObject(NMTT))
    if(FindObject2(Find_AtPoint(iX,iY-i-10),Find_Category(C4D_Structure),Find_NoContainer(),Find_Hostile(GetOwner(pClonk)))) {
      PlayerMessage(GetOwner(pClonk),"$MsgNoTrapAllowedHere$",this());
      return(0);
    }
  // Miteingebaut: Die Anti-Wallhack-Regel
  if(FindObject(NW4V))
    if(IsWallhack(pClonk,iX,iY)) {
      PlayerMessage(GetOwner(pClonk),"$MsgNoArm$",this());
      return(0);
    }
  Message("",this());
  return([iX,iY-i]);
}

public func IsWallhack(object pClonk, int iX, int iY) {
  // Gerade durch
  if(PathFree(GetX(pClonk),GetY(pClonk),GetX(pClonk)+iX,GetY(pClonk)+iY-5)) return(0);
  // Per Loch
  if(PathFree(GetX(pClonk),GetY(pClonk),GetX(pClonk)+iX,GetY(pClonk)) &&
     PathFree(GetX(pClonk)+iX,GetY(pClonk),GetX(pClonk)+iX,GetY(pClonk)+iY-5)) return(0);
  // Falls oben
//  if(PathFree(GetX(pClonk),GetY(pClonk)+iY,GetX(pClonk)+iX,GetY(pClonk)+iY)) return(0);
  return(1);
}

public func Arm(object pClonk)
{
  // Sichtbarkeit
  if(GetOwner(this()) == -1) SetVisibility(VIS_God|VIS_Local,this());
  else  SetVisibility(VIS_Owner|VIS_Allies|VIS_God|VIS_Local,this());
  // Nicht mehr einsammelbar
  fCollectible = false;
  // Irgendwelche Spezialplatzierung?
  if(ArmExtra(pClonk)) return(1);
  // Gegebenfalls noch Aktion anpassen
  SetAction("Armed");
  // Falle ist scharf
  return(1);
}

// Die Spezialplatzierung
public func ArmExtra(object pClonk) { return(0); }

protected func Activate(object pClonk)
{
  [$TxtArm$|Condition=CanArm]
  // Nur Trapper haben die nötige Intelligenz
  if(!CanArm(pClonk)) return(0);
//  if(!(pClonk->~IsTrapper())) return(0);
  // Bedingung zum scharf machen
  if(ArmingConditions()) {
    // Der Clonk darf extra anhalten
    SetComDir(COMD_Stop, pClonk);
    // Zur richtigen Platzierung
    SetR(0,this());
    // Positionierung checken
    var iPosition = ArmCheck(pClonk);
    // Nicht platzierbar? Abbruch
    if(!iPosition) return(1);
    // Besitzer setzen
    SetOwner(GetOwner(pClonk));
    // Geräusch
    Sound("Click",0,0,0,GetOwner(pClonk)+1);
    // Falle einsetzen
    Exit(this(),iPosition[0],iPosition[1]);
    // Falle aufstellen
    Arm(pClonk);
    // Fertig
    return(1);
  }
}

// Trapper-Check
public func CanArm(pClonk) { return(pClonk->~IsTrapper()); }

// Status
public func IsTrap() { return(1); }

// Bedingungen, die zum Aufstellen erfüllt sein müssen
public func ArmingConditions() { return(1); }

// Abbauen
public func DeArm(object pClonk)
{
  SetVisibility(VIS_All);
  for(var i = GetPlayerByIndex(GetPlayerCount()-1)/32; i >= 0; i--)
    Local(i) = 0;
  fCollectible = true;
  SetAction("Idle");
  DeArmExtra(pClonk);
}

public func DeArmExtra(object pClonk) { return(0); } // Falls Fallen noch etwas machen müssen

// Einsammeln
protected func RejectEntrance() { return(IsArmed()); }

// Abfrage
public func IsArmed() { return(!fCollectible); }

// Bestimmte Ziele können Fallen nicht auslösen
public func TrapIgnoreTargets() {
  return(Find_Or(Find_ID(ZAP1)));
}

// Ein Trapper kommt nahe an diese Falle heran, so dass er sie entdecken kann
public func MakeVisibleTo(int iPlr, object pClonk)
{
  // Schon sichtbar?
  if(Local(iPlr/32) & 2**(iPlr%32)) return(0);
  if(GetOwner() != -1 && !Hostile(iPlr,GetOwner())) return(0);
  // Sichtbar machen
  Local(iPlr/32) += 2**(iPlr%32);
  // Effekt starten, damit die Falle wieder unsichtbar wird
  AddEffect("IntTrapVisCheck",this(),1,5,this(),0,iPlr,pClonk);
  return(1);
}

/* Effekt */

public func FxIntTrapVisCheckStart(object pTarget, int iEffectNumber, bool fTemp, int iPlr, object pClonk)
{
  if(fTemp) return(0);
  // Spieler
  EffectVar(0,pTarget,iEffectNumber) = iPlr;
  // Sein Trapper
  EffectVar(1,pTarget,iEffectNumber) = pClonk;
}

public func FxIntTrapVisCheckTimer(object pTarget, int iEffectNumber)
{
  // Falle aus irgendeinem Grund nicht sichtbar
  if(!(Local(EffectVar(0,pTarget,iEffectNumber)/32,pTarget) & 2**(EffectVar(0,pTarget,iEffectNumber)%32)))
    return(-1);
  // Der Trapper ist tot
  if(!GetAlive(EffectVar(1,pTarget,iEffectNumber)))
    return(-1);
  // Trapper ist weiter weg, als er Fallen sehen kann
  if(ObjectDistance(EffectVar(1,pTarget,iEffectNumber),pTarget) > EffectVar(1,pTarget,iEffectNumber)->~TrapSeeRange())
    return(-1);
}

public func FxIntTrapVisCheckStop(object pTarget, int iEffectNumber, int iReason, int iTemp)
{
  if(iTemp) return(0);
  // Der Spieler ist befreundet
  if(!Hostile(GetOwner(pTarget),EffectVar(0,pTarget,iEffectNumber))) {
    if(Local(EffectVar(0,pTarget,iEffectNumber)/32,pTarget) & 2**(EffectVar(0,pTarget,iEffectNumber)%32))
      Local(EffectVar(0,pTarget,iEffectNumber)/32,pTarget) -= 2**(EffectVar(0,pTarget,iEffectNumber)%32);
    return(1);
  }
  // Es befindet sich ein weiterer lebender Trapper gleichen Spielers in der Nähe
  if(EffectVar(1,pTarget,iEffectNumber) = FindObject2(Find_Owner(EffectVar(0,pTarget,iEffectNumber)),Find_OCF(OCF_Alive),
     Find_FuncEqual(Format("ObjectDistance(this(),Object(%d)) <= this()->~TrapSeeRange()",ObjectNumber(pTarget)),1)))
    // Falle bleibt weiterhin sichtbar für den Spieler
    return(-1);
  // Falle wird unsichtbar
  if(Local(EffectVar(0,pTarget,iEffectNumber)/32,pTarget) & 2**(EffectVar(0,pTarget,iEffectNumber)%32))
    Local(EffectVar(0,pTarget,iEffectNumber)/32,pTarget) -= 2**(EffectVar(0,pTarget,iEffectNumber)%32);
  return(1);
}