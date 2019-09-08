/*-- Totes Pferd --*/

#strict

local iGraphics;

/* Verwesung */

private func Decaying()
{
  DoCon(-4);
}

/* Zerlegen */

public func ControlDigDouble(pClonk) {
  [$TxtEmbowel$|Image=KNFE|Condition=GetAlive]
  // Der Clonk soll dieses Objekt zerlegen, wenn er kann
  return(ObjectSetAction(pClonk,"Embowel",this()));
}

/* Wird zerlegt */

public func Embowel(object pByClonk) {
  // Tote Pferde haben Sonderbehandlung 
  var pSkin, pBone;
  if (pSkin = CreateObject(SKIN, 0, 10)) pSkin->~SetSkinGraphics(Format("%d", LocalN("iGraphics")));
  if (pSkin = CreateObject(SKIN, 0, 10)) pSkin->~SetSkinGraphics(Format("%d", LocalN("iGraphics")));
  if (pBone = CreateObject(BBON, 0, 10)) SetName("$NameHorseBone$", pBone);
  if (pBone = CreateObject(BBON, 0, 10)) SetName("$NameHorseBone$", pBone);
  CreateObject(RMET, 0, 10);
  CreateObject(RMET, 0, 10);
  RemoveObject();
  return(1);
}
