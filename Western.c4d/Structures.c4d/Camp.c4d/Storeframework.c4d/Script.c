/*-- Lagergestell --*/

#strict

local iMeat;
local iTime;

local iCollectObj;

public func RackedItemCount() { return(7); }

protected func Initialize()
{
  iMeat = [0,0,0,0,0,0,0];
  iTime = [0,0,0,0,0,0,0];
  return(1);
}

protected func RejectCollect(idObj, pObj)
{
  // Verhindert Aufnahme aller nichtpassender Objekte
  if(!(pObj->~GetFeedValue() || pObj->~CanBeCooked())) return(1, Message("$MsgCanNotSmoke$",this(),GetName(pObj)));
//  if(!(pObj->~CanBeCooked())) return(1);
  // Maximal 7 Stück
  if (ContentsCount() >= RackedItemCount())
    return(1, Message( "$MsgRackFull$",this() ));
  if(Contained(pObj)) iCollectObj = Contained(pObj);
  return(0);
}
  
private func CheckMeat()
{
  if(GetCon() != 100) return();
  var i, iColor, szName;
  for(i = 0; i < 7; i++) {
    if(!iMeat[i]) continue;
    if(!iMeat[i]->~CanBeCooked()) continue;
    iTime[i]++;
    if(iTime[i] >= 120) {
      szName = Format("%s ($TxtSmoked$)", GetName(iMeat[i]));
      iMeat[i]->~Cook();
      SetName(szName, iMeat[i]);
      SetClrModulation(RGB(180, 180, 180), iMeat[i]);
      SetClrModulation(RGB(150, 150, 150), this(), i+1);
      iTime[i] = 0;
    }
    else {
      iColor = 255 - iTime[i]*10/15;
//      iColor = 255 - iTime[i]*80/120;
//      iColor = 255 - iTime[i]*105/120;
      SetClrModulation(RGB(iColor, iColor, iColor), this(), i+1);
    }
  }
}

protected func Collection2(pObj)
{
  var i, iList, iEffect, iColor;
  Sound("RackPut");
  if(iCollectObj) iList = SortMeat(iCollectObj);
  else iList = [0,1,2,3,4,5,6];
  iCollectObj = 0;

  for(i = 0; i < 7; i++) {
    if(iMeat[iList[i]]) continue;
    SetGraphics(Format("%d",iList[i]+1),this(),GetID(),iList[i]+1,1);
    iMeat[iList[i]] = pObj;
    if(!pObj->~CanBeCooked())
      if(WildcardMatch(GetName(pObj),"*$TxtSmoked$*"))
        SetClrModulation(RGB(150, 150, 150), this(), iList[i]+1);
      else
        SetClrModulation(RGB(200, 200, 200), this(), iList[i]+1);
    if(iEffect = GetEffect("IntSmokeValue", pObj)) {
      iTime[i] = EffectVar(0,pObj,iEffect);
      iColor = 255 - iTime[i]*10/15;
      SetClrModulation(RGB(iColor, iColor, iColor), this(), i+1);
      RemoveEffect(0,pObj,iEffect);
    }
    break;
  }
  return(1);
}

protected func Ejection(pObj)
{
  for(var i = 0; i < 7; i++) {
    if(iMeat[i] != pObj) continue;
    SetGraphics(0,this(),0,i+1);
    iMeat[i] = 0;
    // Damit man beim kurzen herrausnehmtest nicht nochmal eine Lange Zeit warten muss
    if(iTime[i]) {
      var iEffect = AddEffect("IntSmokeValue", pObj, 1, 100, 0, GetID());
      EffectVar(0,pObj,iEffect) = iTime[i];
      iTime[i] = 0;
    }
    break;
  }
  Sound("RackPut");
  return(1);
}

protected func FxIntSmokeValueTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  EffectVar(0,pTarget,iEffectNumber) -= 5;
  if(EffectVar(0,pTarget,iEffectNumber) <= 0) return(-1);
  return();
}

protected func ControlDig(pClonk)
{
  [$CtrlTake$|Image=RMET]
  // Gegenstand nehmen
  if (!Contents()) return(Message("$MsgRackEmpty$", this()));
  var pItem = FindSmokedMeat(pClonk);
  if(pItem) Collect(pItem, pClonk);
  return(1);
}

public func SortMeat(pClonk)
{
  var iList = [];
  var iPoints = [-27,-20,-13,-6,3,12,20];
  var i, iSort, iDif, iNumb;

  for(i = 0; i < 7; i++) {
    iList[i] = i;
    if(i == 0) continue;
    for(iSort = i-1; iSort >= 0; iSort--) {
      iDif = GetDifference(AbsX(GetX(pClonk)), iPoints[iList[iSort]]);
      if(GetDifference(AbsX(GetX(pClonk)), iPoints[i]) == iDif) iDif += (GetDir(pClonk)*2-1);
      if(GetDifference(AbsX(GetX(pClonk)), iPoints[i]) < iDif) {
        iNumb = iList[iSort+1];
        iList[iSort+1] = iList[iSort];
        iList[iSort] = iNumb;
      }
      else break;
    }
  }
  return(iList);
}

private func FindSmokedMeat(pClonk)
{
  var i, iNumb, iList;
  iNumb = -1;
  iList = SortMeat(pClonk);
  for(i = 0; i < 7; i++) {
    if(!iMeat[iList[i]]) continue;
    if(!iMeat[iList[i]]->~CanBeCooked()) return(iMeat[iList[i]]);
    if(iNumb != -1) if(iTime[iList[i]] >= iTime[iNumb]) continue;
    iNumb = iList[i];
  }
  if(iNumb != -1) return(iMeat[iNumb]);
  return(0);
}

public func GetDifference(iA,iB) { return(Max(iA,iB)-Min(iA,iB)); }

public func GetResearchBase() { return(STIP); }

/* Kann vom Indianer hergestellt werden */
public func IsIndianHandcraft() { return(1); }

/* Kann von Indianern erforscht werden */
public func IsIndianResearch() { return(1); }
