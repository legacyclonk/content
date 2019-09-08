/*-- Clonks nicht entfernen beim entlassen --*/

#strict
#appendto REAC

func FxReleaseClonkFadeOutStop (object pTarget, int iEffectNumber, int iReason, bool fTemp) {
  if (iReason) return ();
  var plr = GetOwner (pTarget);
  Kill ( pTarget ) ; // Clonk muss getötet werden, damit er später nicht neu kaufbar wird und ggf. Kills gezählt werden!
  //RemoveObject (pTarget, true);
  SetCategory(1, pTarget); // Nicht mehr fallen
  SetClrModulation(RGBa(255,255,255,255)); // Und unsichtbar
}
