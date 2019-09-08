/*-- Wandteppich --*/

#strict

private func StructureCheck()
{
  if(!FindCastlePart(1,1) )
    RemoveObject();
}

public func CastleChange()
{
  StructureCheck();
}
