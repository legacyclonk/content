/*-- Herd --*/

#strict

private func Burning()
{
  if(GetActTime() > 5000) SetAction("Idle");
}

protected func CheckContents()
{
  if(!ActIdle() ) return(1);
  // Holzverbrennung
  if(ContentsCount(WOOD) )
    return(BurnWood());
  // Kohleverbrennung
  var coal = FindContents(COAL);
  if(!coal) return(1);
  RemoveObject(coal);
  Sound("Inflame");
  SetAction("Burning");
}

private func BurnWood()
{
  var wood = FindContents(WOOD);
  if(!wood) return(0);
  RemoveObject(wood);

  Sound("Inflame");
  SetAction("Burning");
}

protected func RejectCollect(id def, object obj)
{
  return(def != COAL && def != WOOD);
}

/* CastleCheck */

public func CastleChange()
{
  if(!FindObject(CPB3, 1) ) return(1);
  while(Contents() ) Exit(Contents() );
  Split2Components();
}

