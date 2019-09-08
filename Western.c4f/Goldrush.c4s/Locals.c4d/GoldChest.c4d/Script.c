/*-- Gold-Truhe --*/

#strict

#include CHST

public func Empty()
{
  // Gold verschwindet, der Rest wird ausgeschüttet	
  while(FindContents(GOLD)) RemoveObject(FindContents(GOLD));
  while(Contents()) Exit(Contents());
}
