/*-- Toter Vogel --*/

#strict

#include DBRD

/* Wird zerlegt */

public func Embowel(object pByClonk) {
  var pObj, iID, i, iCount, pContainer;
  pContainer = Contained();
  for(i = 0; iID = GetComponent(0,i,this()); i++) {
    if(!FindDefinition(iID)) continue;
    for(iCount = GetComponent(iID,0,this()); iCount > 0; iCount--) {
      pObj = CreateObject(iID,0,0);
      // Seine Federn sind schwarz
      if(iID == FETH)
        SetClrModulation(RGB(80,145,255),pObj);
      if(pContainer) { Enter(pContainer, pObj); continue; }
      SetXDir(RandomX(-1,1)*10, pObj);
      SetYDir(RandomX(-1,1)*10, pObj);
      SetRDir(RandomX(-1,1)*10, pObj);
      SetR(Random(360), pObj);
    }
  }
  RemoveObject(this());
  return(1);
}