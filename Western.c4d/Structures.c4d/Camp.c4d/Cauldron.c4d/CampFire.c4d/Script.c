/*-- Lagerfeuer --*/

#strict

protected func Initialize() {
  CreateContents(WOOD);
  BurnWood();
}

private func Smoking() {
  //if(!Random(2))
    Smoke(Random(3),Random(3),Random(8)+5);
}

protected func CheckContents() {
  if(GetActTime()>6000)
    SetAction("Idle");
  if(ActIdle())
    if(ContentsCount(WOOD))
      return(BurnWood());
}

private func BurnWood() {
  var pWood;
  if(pWood=FindContents(WOOD)) RemoveObject(pWood);
  Sound("Inflame");
  SetAction("Burn");
}

// außer Holz nichts aufnehmen
protected func RejectCollect(idType, pObj) { if (idType != WOOD) return(1); }

// Mit Links und Rechts sollen Objekte herrausgenommen werden, nicht herrausaktiviert.
protected func ControlLeft(object pClonk)
{
  [$DescTake$]
  // Menü öffnen
  SetCommand(pClonk, "Get", this(), 0, 0, 0, 1);
}

protected func ControlRight(object pClonk)
{
  [$DescTake$]
  // Menü öffnen
  SetCommand(pClonk, "Get", this(), 0, 0, 0, 1);
}