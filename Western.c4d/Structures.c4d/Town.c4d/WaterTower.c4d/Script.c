/*-- Wasserturm --*/

#strict
#include BAS4

// Hier wurde ziemlich alles verbessert

local pBarrel; // Das Fass (Siehe Pumpe)
local iFillLevel; // Fç¸llstand

// Anzahl Materialpixel, die in den Wasserturm passen
static const WTOW_MaxFill = 4000;
// 20fache Grˆﬂe eines Fasses

protected func Initialize()
{
  SetAction("PipeRaised");
  pBarrel = CreateContents(WBRL);
  pBarrel->~BarrelDoFill(-pBarrel->~GetAmount(), Material("Water")+1);
  return(1);
}

protected func ControlDownSingle()
{
  [$MsgLowerPipe$]
  if(GetAction() eq "PipeLowered" || GetAction() eq "LowerPipe") return(0);
  var iPhase;
  if(GetAction() eq "RaisePipe")
    iPhase = GetPhase(this())+1;
  SetAction("LowerPipe");
  if(iPhase)
    SetPhase(10-iPhase, this());
  return(1);
}
  
protected func ControlUp()
{
  [$MsgRaisePipe$]
  if(GetAction() eq "PipeRaised" || GetAction() eq "RaisePipe") return(0);
  var iPhase;
  if(GetAction() eq "LowerPipe")
    iPhase = GetPhase(this())+1;
  SetAction("RaisePipe");
  if(iPhase)
    SetPhase(10-iPhase, this());
  return(1);
}

// Wassergehalt Nachpr¸fen (Wasserflaschen herstellen)
protected func ControlDigDouble()
{
  [$MsgViewWater$]
  Message("%d{{WNTH}}", this(), GetFillLevel());
  return(1);
}

// Wasser Nachf¸llen/Nehmen
protected func ControlThrow(pClonk)
{
  [$MsgTakeWater$]
  var pObj = Contents(0, pClonk), iFilled;
  iFilled = FillObject(pClonk, pObj);
  if(GetOCF(pObj) & OCF_OnFire)
    if(GetFillLevel()) {
      Extinguish(pObj);
      Sound("Pshshsh",0,this());
      return(1);
    }
  Message("%d{{WNTH}}", this(), GetFillLevel());
  return(iFilled);
}

public func FillObject(pClonk, pObj)
{
  var iFill, idBarrel;
  if(GetID(pObj) == BARL || GetID(pObj) == WDBR) {
    iFill = DoFillLevel(-Min(pObj->~BarrelMaxFill() - pObj->~GetAmount(), GetFillLevel()));
    pObj->~BarrelDoFill(iFill, Material("Water")+1);
    if(pObj->~GetAmount()) {
      if(GetID(pObj) == BARL) ChangeDef(WBRL, pObj);
      if(GetID(pObj) == WDBR) ChangeDef(WDWB, pObj);
    }
    if(iFill)
      Sound("Splash1",0,this());
    return(1);
  }
  if(GetID(pObj) == WBRL || GetID(pObj) == WDWB) {
    iFill = DoFillLevel(Min(pObj->~GetAmount(), GetMaxFill() - GetFillLevel()));
    pObj->~BarrelDoFill(-iFill);
    if(!pObj->~GetAmount())
      if(idBarrel = pObj->~EmptyBarrelID())
        ChangeDef(idBarrel, pObj);
    if(iFill)
      Sound("Splash2",0,this());
    return(1);
  }
  if(GetID(pObj) == BOWL) {
    if(GetAction(pObj) eq "Idle") {
      iFill = DoFillLevel(-Min(30-LocalN("iWater", pObj), GetFillLevel()));
      LocalN("iWater", pObj) += iFill;
      if(LocalN("iWater", pObj) >= 30) {
        ObjectSetAction(pObj, "Water");
        SetGraphics("Full", pObj);
        SetColorDw(RGB(0,0,255), pObj);
      }
      if(iFill)
        Sound("Splash1",0,this());
      return(1);
    }
    else if(GetAction(pObj) eq "Water") {
      iFill = DoFillLevel(Min(LocalN("iWater", pObj), GetMaxFill() - GetFillLevel()));
      LocalN("iWater", pObj) -= iFill;
      if(!LocalN("iWater", pObj)) {
        ObjectSetAction(pObj, "Idle");
        SetGraphics("", pObj);
        SetColorDw(, pObj);
      }
      if(iFill)
        Sound("Splash2",0,this());
      return(1);
    }
  }
  if(GetID(pObj) == WSKI) {
    if(LocalN("iWater", pObj) < 90 && GetFillLevel()) {
      iFill = DoFillLevel(-Min(90-LocalN("iWater", pObj), GetFillLevel()));
      LocalN("iWater", pObj) += iFill;
      pObj->~UpdateGraphics();
      if(iFill)
        Sound("Splash1",0,this());
      return(1);
    }
    else {
      iFill = DoFillLevel(Min(LocalN("iWater", pObj), GetMaxFill() - GetFillLevel()));
      LocalN("iWater", pObj) -= iFill;
      pObj->~UpdateGraphics();
      if(iFill)
        Sound("Splash2",0,this());
      return(1);
    }
  }
}

// Es sollte darin nur Wasser gelagert werden kˆnnen aus folgenden Gr¸nden:
// - Lava brennt den Turm nieder.
// - S‰ure frisst sich einfach durch.
// - ÷l kann man schˆn in F‰ssern lagern und gleich Verkaufen.
//   (Kˆnnte darin ÷l gelagert werden, dann w‰re es kein Wasserturm mehr!)

public func GetFillLevel(pObject)
{
  var i;
  if(!pObject) return(iFillLevel + pBarrel->~GetAmount());
  if(pObject == this()) return(iFillLevel);
  if(pObject == pBarrel) return(pBarrel->~GetAmount());
}

public func GetMaxFill() { return(WTOW_MaxFill); }

public func DoFillLevel(iAmount)
{
  var iToChange = Max(iAmount,-iAmount);
  var iSave;
  if(!iAmount) return(0);
  if(iAmount < 0) {
    if(GetFillLevel(this()) < iToChange) {
      if(GetFillLevel() < iToChange) {
        iSave = iFillLevel + pBarrel->~GetAmount();
        iFillLevel = 0;
        pBarrel->~BarrelDoFill(-pBarrel->~GetAmount(), Material("Water")+1);
        return(iSave);
      }
      else {
        iSave = iToChange - iFillLevel;
        iFillLevel = 0;
        pBarrel->~BarrelDoFill(-iSave, Material("Water")+1);
        return(iToChange);
      }
    }
    else {
      iSave = Min(pBarrel->~GetAmount() - pBarrel->~BarrelMaxFill()/2, iToChange);
      if(iSave < 0) iSave = 0;
      else pBarrel->~BarrelDoFill(-iSave, Material("Water")+1);
      iFillLevel -= (iToChange - iSave);
      return(iToChange);
    }
  }
  if(iAmount > 0) {
    if(GetFillLevel(this()) + iToChange > GetMaxFill() - pBarrel->~BarrelMaxFill()) {
      if(GetFillLevel() + iToChange > GetMaxFill()) {
        iSave = GetMaxFill() - iFillLevel - pBarrel->~GetAmount();
        iFillLevel = GetMaxFill() - pBarrel->~BarrelMaxFill();
        pBarrel->~BarrelDoFill(pBarrel->~BarrelMaxFill() - pBarrel->~GetAmount(), Material("Water")+1);
        return(iSave);
      }
      else {
        iSave = GetMaxFill() - iFillLevel - pBarrel->~BarrelMaxFill();
        iFillLevel = GetMaxFill() - pBarrel->~BarrelMaxFill();
        pBarrel->~BarrelDoFill(iToChange - iSave, Material("Water")+1);
        return(iAmount);
      }
    }
    else {
      iSave = Min(pBarrel->~BarrelMaxFill()/2 - pBarrel->~GetAmount(), iToChange);
      if(iSave < 0) iSave = 0;
      else pBarrel->~BarrelDoFill(iSave, Material("Water")+1);
      iFillLevel += (iToChange - iSave);
      return(iAmount);
    }
  }
}

/* TimerCall */

private func Timer()
{
  var i;

  if(GetCon() < 100) return();
  if(!pBarrel) {
    pBarrel = CreateContents(WBRL);
    pBarrel->~BarrelDoFill(-pBarrel->~GetAmount(), Material("Water")+1);
  }

  /* F¸ll-Check */

  // Zuerst wird das Fass im Turm gecheckt, ob es genug, bzw. zuwenig Wasser hat.
  // Bei "true" wird Wasser vom Turm oder vom Fass ¸bertragen.
  // (Wurde gemacht, da ich kein Extra-Script f¸r die Leitungen in den System.c4g tun mochte,
  //  das bei verlust oder weiteren #appendto's Errors verursacht)

  if(GetID(pBarrel) != WBRL) {
    if(GetID(pBarrel) == pBarrel->~EmptyBarrelID())
      ChangeDef(WBRL, pBarrel);
    else {
      var strMaterial;
      if(strMaterial = pBarrel->~BarrelMaterialName())
        CastPXS(strMaterial, pBarrel->~GetAmount(), 20);
      // Das Falsche Material sollte besser durch die Rˆhre abflieﬂen.
      pBarrel->~BarrelDoFill(-(pBarrel->~GetAmount()));
      ChangeDef(WBRL, pBarrel);
    }
  }

  if(pBarrel->~GetAmount() < pBarrel->~BarrelMaxFill()/2 && GetFillLevel(this()))
    if(i = Min(pBarrel->~BarrelMaxFill()/2 - pBarrel->~GetAmount(), GetFillLevel(this()))) {
      pBarrel->~BarrelDoFill(i, Material("Water")+1);
      iFillLevel -= i;
    }

  if(pBarrel->~GetAmount() > pBarrel->~BarrelMaxFill()/2 && GetMaxFill()-pBarrel->~BarrelMaxFill()-GetFillLevel(this()) > 0)
//  if(pBarrel->~GetAmount() > pBarrel->~BarrelMaxFill()/2 && GetMaxFill()-GetFillLevel() > 0)
// Darin sollte hˆchstens 4000pxl Wasser gelagert werden, desshalb wird das Wasser im Fass auch als Lagerung mitgez‰hlt
    if(i = Min(pBarrel->~GetAmount()-pBarrel->~BarrelMaxFill()/2, GetMaxFill()-pBarrel->~BarrelMaxFill()-GetFillLevel(this()))) {
      pBarrel->~BarrelDoFill(-i, Material("Water")+1);
      iFillLevel += i;
    }

  /* Abflieﬂer */

  if(GetAction() eq "PipeLowered")
    for(i = 0; i < 2; i++) {
      if(!DoFillLevel(-1)) break;
      InsertMaterial(Material("Water"), 32, 0, 0, Random(5));
    }
}

/* Zerstˆrung */

protected func Destruction() { CastPXS("Water",GetFillLevel(),30); if(pBarrel) RemoveObject(pBarrel); iFillLevel = 0; return(_inherited()); }
protected func Incineration() { CastPXS("Water",GetFillLevel(),30); if(pBarrel) RemoveObject(pBarrel); iFillLevel = 0; return(_inherited()); }
protected func IncinerationEx() { CastPXS("Water",GetFillLevel(),30); if(pBarrel) RemoveObject(pBarrel); iFillLevel = 0; return(_inherited()); }