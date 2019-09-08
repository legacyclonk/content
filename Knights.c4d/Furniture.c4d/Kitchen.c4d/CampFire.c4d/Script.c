/*-- Lagerfeuer --*/

#strict

protected func Initialize() 
{
  CreateContents(WOOD);
  CreateContents(WOOD);
  CreateContents(WOOD);
  BurnWood();
}

private func Smoking() 
{
  Smoke(Random(3), Random(3), Random(8) + 5);
}

protected func CheckContents() 
{
  if(GetActTime()>5000)
    SetAction("Idle");
  if(ActIdle())
    if(ContentsCount(WOOD))
      return(BurnWood());
}

private func BurnWood() 
{
  var pWood;
  if(pWood=FindContents(WOOD)) RemoveObject(pWood);
  Sound("Inflame");
  SetAction("Burn");
}

// auﬂer Holz nichts aufnehmen
protected func RejectCollect(def,pObj) { if(def!=WOOD) return(1); }