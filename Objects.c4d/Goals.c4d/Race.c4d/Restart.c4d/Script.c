/*-- Neustart --*/

#strict

protected func Activate(iPlr)
{
  // Szenario benachrichtigen
  if(GameCall("OnRestart", iPlr)) return();
  // Den Clonk des Spielers löschen
  var pClonk = GetCrew(iPlr), pWipf;
  RemoveObject(pClonk, 1);
}
