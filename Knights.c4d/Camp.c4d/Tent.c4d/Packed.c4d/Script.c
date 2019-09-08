/*-- Zeltpack --*/

#strict

public func Activate(pClonk)
{
  [$SetupTent$]
  // Effekt
  Sound("TentPack");
  // Zelt erzeugen
  var pTent = CreateObject(TENT,0,Min(11, LandscapeHeight()-GetY()-1),GetOwner(pClonk) );
  // Zelt ins Aufbaustadium versetzen
  pTent->BeginCon();
  // Bau beginnen lassen
  AddCommand(pClonk, "Grab", pTent);
  // Pack entfernen
  RemoveObject();
  // Fertig
  return(1);
}

protected func Hit()
{
  // Effekt
  Sound("TentPackHit");
  return(1);
}

/* Produkteigenschaften */

public func IsAnvilProduct() { return(1); }
