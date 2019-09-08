/* Saloon */

#strict

#appendto SALO

public func RejectConstruction(iX, iY, pClonk)
{
  if((GetX(pClonk)+iX)<590) if((GetY(pClonk)+iY)<240) return(0);
  Message("<c ff0000>$MsgSaloonInIndianland$</c>", pClonk);	
  return(1);	
}
