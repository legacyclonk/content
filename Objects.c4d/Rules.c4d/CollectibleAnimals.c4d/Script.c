/*-- Einsammelbare Tiere --*/

#strict

protected func Activate(iByPlayer)
{
  MessageWindow(GetDesc(), iByPlayer);
  return(1);
}

// Globale Einstellung: Kann von Paketen oder Szenarien überladen werden
global func ANIM_IsCollectible(object pByContainer)
  {
  // Wenn Spielregel aktiviert.
  return ObjectCount(COAN);
  }
