/*-- Clonk entlassen --*/

#strict

/* Regelscript */

protected func Activate(plr) {
  // Effekt für das Ausfaden des Clonks anhängen
  AddEffect ("ReleaseClonkFadeOut", GetCursor (plr), 320, 1, 0, GetID (), plr);
  return(1);
}

func FxReleaseClonkFadeOutStart (object pTarget, int iEffectNumber, int iTemp, int plr) {
  // Meldung über dem Clonk
  Message("$MsgReleased$", pTarget, GetName(pTarget)); 
}

func FxReleaseClonkFadeOutTimer (object pTarget, int iEffectNumber, int iEffectTime) {
  // Ausfaden
  if (!GetClrModulation (pTarget)) SetClrModulation(RGB(255, 255, 255), pTarget);
  SetClrModulation(DoRGBaValue(GetClrModulation(pTarget), Min(15, 255-GetRGBValue(GetClrModulation(pTarget),0)), 0), pTarget);
  // Wenn Overlay extra moduliert wird, auch extra ausfaden
  if (GetObjectBlitMode (pTarget) & 4) SetColorDw(DoRGBaValue(GetColorDw(pTarget), Min(15, 255-GetRGBValue(GetColorDw(pTarget),0)), 0), pTarget);
  if(GetRGBValue(GetColorDw(pTarget), 0) == 255 || GetRGBValue(GetClrModulation(pTarget), 0) == 255)
    return (-1);
}

func FxReleaseClonkFadeOutStop (object pTarget, int iEffectNumber, int iReason, bool fTemp) {
  if (iReason) return ();
  var plr = GetOwner (pTarget);
  Kill ( pTarget ) ; // Clonk muss getötet werden, damit er später nicht neu kaufbar wird und ggf. Kills gezählt werden!
  RemoveObject (pTarget, true);
}
