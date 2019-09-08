/*--- Zepter ---*/

#strict

protected func Hit() { Sound("WoodHit*"); }

public func Activate(pClonk) {
  [$TxtUse$]
  var pSeal=FindObject(SEAL,-350,-250,-1,-1);
  if(ObjectDistance(pSeal)>600) pSeal=0;
  if(!pSeal || GetBreath(pSeal)<80) {
    Message("$TxtNosealinrange$",this());
    Sound("Error");
    return(0);
  }
  if(!(pSeal->SEAL::TakePossession(pClonk))) return();
  Sound("Magic*");
  return(1);
}