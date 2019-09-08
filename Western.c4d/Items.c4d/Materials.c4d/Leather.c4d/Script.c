/*-- Leder --*/

#strict 2

// Alternative Baustoffe
public func GetAlternativeID() { return SLTR; }

public func GetBestAlternativeID(pClonk)
{
  var i, pObj;
  for(i = 0; i < 3; i++)
    if(FindContents([SKIN,SNSK,WSKN][i],pClonk)) return([LTHR,SLTR,WLTR][i]);
  if(pObj = FindObject2(Sort_Distance(GetX(pClonk),GetY(pClonk)),Find_OCF(OCF_Available),Find_Or(Find_ID(SKIN),Find_ID(SNSK),Find_ID(WSKN)))) {
    if(GetID(pObj) == SKIN) return(LTHR);
    if(GetID(pObj) == SNSK) return(SLTR);
    if(GetID(pObj) == WSKN) return(WLTR);
  }
  return(0);
}

public func AdaptGraphics(szGraphic)
{
  if(!szGraphic) return;
  SetGraphics(szGraphic);
}

public func GetGraphicString(pClonk)
{   
  var obj = FindContents(SKIN, pClonk);
  if(!obj) return;
  else return LocalN("szGraphic", obj);
}

/* Kann von Indianern erforscht werden */
public func IsIndianResearch() { return 1; }

/* Kann vom Indianer in Handarbeit hergestellt werden */
public func IsIndianHandcraft() { return 1; }

/* Kann vom Trapper in Handarbeit hergestellt werden */
public func IsTrapperHandcraft() { return 1; }

// Verkauf
protected func SellTo() {
  if(GetName(0,C4Id(GetAction()))) return C4Id(GetAction());
  return LTHR;
}
