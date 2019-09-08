/*-- Haftbombe --*/

#strict 2

local fContained;

public func Activate(pClonk)
{
  if(Contained(pClonk)) return(0);
  var pObj, iVehicles = FindObjects(Sort_Distance(),Find_Distance(20),Find_Category(C4D_Vehicle),Find_NoContainer());
  SetComDir(COMD_None,pClonk);
  CreateMenu(GetID(),pClonk,this(),0,"$TxtNoTargets$");
  for(pObj in iVehicles) {
    if(GetCategory(0,GetID(pObj)) == C4D_Vehicle) continue;
    if(FindObject2(Find_ID(GetID()),Find_Action("Stick"),Find_ActionTarget(pObj))) continue;
    if(GetMaterialVal("Extinguisher","Material",GetMaterial(AbsX(GetX(pObj)),AbsY(GetY(pObj))))) continue;
    AddMenuItem("$TxtArm$","Arm",GetID(pObj),pClonk,0,pObj,"$DescArm$",4,pObj);
  }
  return(1);
}

public func Arm(id iID, object pVehicle)
{
  if(!Contained()) return(0);
  if(ObjectDistance(this(),pVehicle) > 30) return(0);
  if(FindObject2(Find_ID(GetID()),Find_Action("Stick"),Find_ActionTarget(pVehicle))) return(0);
  if(GetMaterialVal("Extinguisher","Material",GetMaterial(AbsX(GetX(pVehicle)),AbsY(GetY(pVehicle))))) return(0);
  SetOwner(GetOwner(Contained()));
  Exit(this());
  SetAction("Stick",pVehicle);
  Sound("Fuse",0,this());
  SetVertex(0,0,GetVertex(0,0,pVehicle),this());
  SetVertex(0,1,GetVertex(0,1,pVehicle),this());
  // Damit es mit einem Wasserfass gelöscht werden kann
  var pObj, iEffect;
  pObj = CreateObject(TIM1);
  iEffect = AddEffect("Fire",pObj,100,1,0,GetID());
  EffectVar(0,pObj,iEffect) = this();
  // Um das Anzünden anderer Objekte zu vermeiden
  SetObjectLayer(pObj,pObj);
  SetVisibility(VIS_None,pObj);
  return(pVehicle);
}

public func FxFireTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  var pStickBomb = EffectVar(0,pTarget,iEffectNumber);
  if(!pStickBomb) return(-1);
  if(GetAction(pStickBomb) != "Stick") return(-1);
  if(Contained(pStickBomb)) {
    if(Contained(pTarget) != pStickBomb) Enter(pStickBomb, pTarget);
  }
  else {
    if(Contained(pTarget)) Exit(pTarget);
    SetPosition(GetX(pStickBomb),GetY(pStickBomb),pTarget);
  }
  return(1);
}

public func FxFireStop(object pTarget, int iEffectNumber, int iReason, bool fTemp)
{
  if(fTemp) return(0);
  if(!EffectVar(0,pTarget,iEffectNumber)) return(RemoveObject(pTarget));
  if(iReason == 3) return(0);
  EffectVar(0,pTarget,iEffectNumber)->~DoExtinguish();
  RemoveObject(pTarget);
  return(1);
}

public func Spark()
{
  if(GetMaterialVal("Extinguisher","Material",GetMaterial())) return(DoExtinguish());
  if(GetActTime(this()) > 175) return(Detonate(GetActionTarget(0)));
  if(Contained(GetActionTarget(0))) {
    if(!fContained) {
      // Damit das Rauschen immer noch zu hören ist
      //SetVisibility(VIS_None);
      SetGraphics(0,this(),TIM1);
      SetCategory(GetCategory() | 16777216, this());
      fContained = 1;
    }
    if(GetActTime(this()) > 175) return(Detonate(GetActionTarget(0)));
    return(0);
  }
  if(fContained) {
    SetGraphics(0,this());
    SetCategory(GetCategory() - 16777216, this());
    fContained = 0;
  }

  SetVertex(0,0,GetVertex(0,0,GetActionTarget(0)),this());
  SetVertex(0,1,GetVertex(0,1,GetActionTarget(0)),this());
  if(Random(4)) return(0);
  for(var iAmount = RandomX(1,3); iAmount >= 0; iAmount--)
    CreateParticle("DynamiteSpark",0,0,RandomX(-10,10),RandomX(-10,10),20,RGBa(255,255,0,100));
}

public func Detonate(object pObj)
{
  var iID = GetID(pObj), idComponent, iList, iNum;
  var i, iCount, iObj;
  var iR, iSpeed;
  var pContainer = Contained(pObj);
  DoDamage(1000,pObj);
  if(GetDefBurnTo(iID) || GetID(pObj) != iID || GetID(pObj) == DRGN) {
    if(GetDefBurnTo(iID)) Incinerate(pObj);
    // Zum Spaß ^^
    if(GetID(pObj) == DHRS) {
      SetPosition(GetX(pObj),GetY(pObj)-20,pObj);
      pObj->~Embowel();
    }
    if(pContainer) { SetAction("Idle"); Enter(pContainer, this()); }
    Sound("Blast1",0,this());
    Explode(30);
    return(1);
  }
  // Inhalt checken
  iList = FindObjects(Find_Container(pObj));
  Sound("Blast1",0,this());
  if(pContainer) { SetAction("Idle"); Enter(pContainer, this()); }
  Explode(30);
  pContainer = Contained(pObj);
  for(iObj in iList) {
    if(pContainer) {
      Enter(pContainer,iObj);
      if(GetOCF(iObj) & OCF_Inflammable) Incinerate(iObj);
      continue;
    }
    iSpeed = Max(RandomX(60,30),5);
    iR = RandomX(-95,95);
    Exit(iObj,0,0,Random(360),0,0,RandomX(-10,10));
    SetXDir( Sin(iR,iSpeed),iObj);
    SetYDir(-Cos(iR,iSpeed)*2,iObj);
    if(GetOCF(iObj) & OCF_Inflammable) if(!Random(3)) Incinerate(iObj);
    if(GetID(iObj) == CABL) if(Random(100) <= Max(iR,-iR)) LocalN("fActivated", iObj) = 0;
  }
  while(idComponent = GetComponent(0,iNum++,0,iID)) {
    if(GetDefContactIncinerate(idComponent) || GetDefBlastIncinerate(idComponent)) {
      if(idComponent == OBRL) {
        if(!pContainer) CreateObject(FBMP)->~Hit();
        continue;
      }
      i = GetComponent(idComponent,0,0,iID);
      for(iCount = RandomX(i/2,i); iCount > 0; iCount--) {
        iObj = CreateObject(idComponent,0,0,GetOwner(pObj));
        SetCon(RandomX(100,75),iObj);
        Incinerate(iObj);
        if(pContainer) Enter(pContainer,iObj);
        iSpeed = Max(RandomX(60-GetMass(0,idComponent)*2,20),5);
        iR = RandomX(-95,95);
        SetR(Random(360),iObj);
        SetRDir(RandomX(-10,10),iObj);
        SetXDir( Sin(iR,iSpeed),iObj);
        SetYDir(-Cos(iR,iSpeed)*2,iObj);
      }
    }
    else if(idComponent == METL || GetComponent(METL,0,0,idComponent)) {
      if(pContainer) continue;
      for(i = GetComponent(idComponent,0,0,iID); i > 0; i--)
        for(iCount = RandomX(3,5); iCount > 0; iCount--) {
          iObj = CreateObject(BALP,0,0,GetOwner(pObj));
          SetR(Random(360),iObj);
          SetRDir(RandomX(-10,10),iObj);
          iSpeed = RandomX(50,20);
          iR = RandomX(-95,95);
          SetXDir( Sin(iR,iSpeed),iObj);
          SetYDir(-Cos(iR,iSpeed)*2,iObj);
        }
    }
  }
  RemoveObject(pObj);
  return(1);
}

private func DoExtinguish() {
  Sound("Pshshsh");
  return(TargetLost());
}

public func TargetLost()
{
  SetVertex(0,0,0,this());
  SetVertex(0,1,1,this());
  SetAction("Idle");
  return(1);
}

protected func Hit() { Sound("StickyBombHit"); }

public func RejectEntrance(object pObj)
{
  if(GetAction() == "Stick") return(1);
  return(0);
}

// Kann in der Waffenschmiede hergestellt werden
public func IsArmoryProduct() { return(1); }

// Da man auch nur ein Fahrzeug mit sich herrum schleppen kann
public func CarryLimit() { return(1); }