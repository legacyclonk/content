/*-- Netz --*/

#strict 2
#include TRAP

local iOverlay;

public func DeArmExtra(object pClonk)
{
  SetAction("Idle");
  var pObj;
  if(pObj = FindObject(TNNT,0,0,0,0,0,"Net",this())) RemoveObject(pObj);
  Collect(this(),pClonk);
  if(GetActionTarget()) {
    SetGraphics("",GetActionTarget(),0,iOverlay);
    if(!Contained()) SetPosition(GetX(),GetDefBottom(GetActionTarget()),this());
  }
  SetActionTargets();
  iOverlay = 0;
}

protected func Check()
{
  if(InLiquid()) return(DeArm());
  if(!GetActionTarget(1)) return(DeArm());
  if(!(GetCategory(GetActionTarget(1)) & C4D_StaticBack)) return(DeArm());

  if(GetActTime() < 50) return(0);

  var pLiving = FindObject2(Sort_Distance(),Find_InRect(-5,-10,10,35),Find_Or(Find_OCF(OCF_Alive),Find_Func("IsHorse")),
		  Find_NoContainer(),Find_Not(TrapIgnoreTargets()));
  if(pLiving) return(Trap(pLiving));
}

protected func Trapped()
{
  var pTrapped = GetActionTarget();
  if(!pTrapped) return(DeArm());
  if(InLiquid()) return(DeArm());
  if(!GetActionTarget(1)) return(DeArm());
  if(!(GetCategory(GetActionTarget(1)) & C4D_StaticBack)) return(DeArm());

  if(ObjectDistance(pTrapped) > 20) return(DeArm());
  if(GetActTime() > 1000) return(DeArm());

  if(GetAction(pTrapped) != "Jump")
    ObjectSetAction(pTrapped, "Jump");
  SetPosition(GetX(),GetY()+10,pTrapped);
  SetXDir(0,pTrapped);
  SetYDir(0,pTrapped);
}

private func Trap(object pToTrap)
{
  // Geräusch
  Sound("Arrow");
  // Opfer ist zu groß
  if(GetDefWidth(GetID(pToTrap)) > 30) return(DeArm());
  // Ist bereits in einem Netz
  if(FindObject2(Find_ID(GetID()),Find_ActionTarget(pToTrap))) return(DeArm());
  SetAction("Trapped",pToTrap);
  SetActionTargets(pToTrap,GetActionTarget(1));
  // Anhalten
  SetPosition(GetX(),GetY()-GetObjectVal("Offset",0,pToTrap,1),pToTrap);
  SetXDir(0,pToTrap);
  SetYDir(0,pToTrap);
  // Netz drüber
  iOverlay = GetUnusedOverlayID(20,pToTrap);
  SetGraphics(0,pToTrap,TNNT,iOverlay,1);
  ObjectSetAction(CreateObject(TNNT,0,0,-1),"Net",this(),pToTrap);
  SetComDir(COMD_Stop,pToTrap);
  // Ist das Objekt mal geritten? So sollte es jetzt nicht mehr!
  ObjectSetAction(pToTrap,"Jump");
}

public func ArmCheck(object pClonk)
{
  if(!pClonk) pClonk = Contained();
  if(!pClonk) return(0);
  // Zuerst den nächsten Baum im Hintergrund suchen
  var pTree = FindObject2(Find_AtPoint(),Find_OCF(OCF_Chop),Find_Func("IsTree"));
  if(!pTree) {
    PlayerMessage(GetOwner(pClonk),"$MsgNoTree$",this());
    return(0);
  }
  // Ist dieser Baum nicht groß genug?
  if(GetObjHeight(pTree) < GetDefHeight(GetID(pClonk))*3/2) {
    PlayerMessage(GetOwner(pClonk),"$MsgSmallTree$",this());
    return(0);
  }
  // Hängt da bereits eine Falle am Baum?
  if(FindObject2(Find_ID(GetID()),Find_ActionTarget2(pTree))) {
    PlayerMessage(GetOwner(pClonk),"$MsgNoArm$",this());
    return(0);
  }
  // Nochmal Höhe zur Mitte des Baumes berechnen
  var i, iX = AbsX(GetX(pTree)), iY = AbsY(GetDefBottom(pTree));
  for(i = 0; i <= 30; i++) {
    if(i == 30) {
      // Die Falle funktioniert nicht unter der Erde
      PlayerMessage(GetOwner(pClonk),"$MsgNoArm$",this());
      return(0);
    }
    if(GBackSolid(iX,iY-i)) continue;
    if(GetObjHeight(pTree) < i + 30) {
      // Der Baum steckt zu tief im Untergrund
      PlayerMessage(GetOwner(pClonk),"$MsgSmallTree$",this());
      return(0);
    }
    if(!PathFree(GetX()+iX,GetY()+iY-i,GetX()+iX,GetY()+iY-30-i)) {
      // Es gibt nicht genügend Platz über dem Baum
      PlayerMessage(GetOwner(pClonk),"$MsgNoArm$",this());
      return(0);
    }
    iY -= (30+i);
    break;
  }
  // Miteingebaut: Die Anti-Wallhack-Regel
  if(FindObject(NW4V))
    if(IsWallhack(pClonk,iX,iY)) {
      PlayerMessage(GetOwner(pClonk),"$MsgNoArm$",this());
      return(0);
    }
  SetActionTargets(0,pTree);
  Message("",this());
  // Position festgelegt
  return([iX,iY]);
}

public func IsWallhack(object pClonk, int iX, int iY) {
  // Nur gerade durch
  if(PathFree(GetX(pClonk),GetY(pClonk),GetX(pClonk)+iX,GetY(pClonk)+iY)) return(0);
  return(1);
}