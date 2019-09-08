/* Karmaenergie für besitzerlose Clonks */

#strict 2

global func DoKarmaEnergy(iAmount, iPlr)
{
  // Hat da jemand fälschlicher Weise ein Objekt angegeben?
  if(GetType(iPlr)==C4V_C4Object) iPlr = GetOwner(iPlr);
  // Kein Besitzer? Dann Zauberenergie des Clonks. Bleibt zu hoffen, dass der Clonk auch der Aufrufer ist...
  if (iPlr == NO_OWNER) return DoMagicEnergy(iAmount);
  // Sonst regulärer Aufruf
  return _inherited(iAmount, iPlr, ...);
}

global func GetKarmaEnergy(iPlr)
{
  // Kein Besitzer? Dann Zauberenergie des Clonks. Bleibt zu hoffen, dass der Clonk auch der Aufrufer ist...
  if (iPlr == NO_OWNER) return GetMagicEnergy();
  // Sonst regulärer Aufruf
  return _inherited(iPlr, ...);
}
