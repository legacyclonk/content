/*-- Wandteppich --*/

#strict

private func DecorationID() { return(DCO1); }

protected func Activate(pClonk)
{
  var castle_part = FindCastlePart();
  if(!castle_part) return(1);
  if(GetID(castle_part) == CPW3) return(1);
  Sound("TentPack");
  CreateObject(DecorationID(),GetX(pClonk)-GetX(),GetY(pClonk)-18-GetY(),GetOwner());
  RemoveObject();
  return(1);
}

/* Kann geschmiedet werden */
public func IsAnvilProduct() { return(1); }


/* Forschung */

public func GetResearchBase() { return(CPW2); }
