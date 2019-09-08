/*-- Munitionspack --*/

#strict

/* Auftreffen */

protected func Hit()
{
  Sound("MetalHit*");
}
  
public func Unpack(object target) {
  for (var i = 0; i < 10; ++i)
    CreateContents(ADB1, target);
  RemoveObject();
  return(1);
}

/* Forschung */  

public func GetResearchBase() { return(CTW0); }

/* Kann in der Chemiefabrik hergestellt werden */
public func IsChemicalProduct() { return(1); }