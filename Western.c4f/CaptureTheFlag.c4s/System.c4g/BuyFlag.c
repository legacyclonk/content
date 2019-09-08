/*-- Ersatzflagge erzeugen --*/

#strict
#appendto HEDQ

public func ContainedUp(pClonk)
{
  if(Hostile(GetOwner(pClonk), GetOwner())) return();
  if(!FindObject(FLAG, 0, 0, 0, 0, 0, "FlyBase", this()))
  {
    if(GetWealth(GetOwner(pClonk))>=GetValue(0, FLAG)*3/2)
    {
      DoWealth(GetOwner(pClonk), -GetValue(0, FLAG)*3/2);
      Sound("Cash");
      Enter(this(), CreateObject(FLAG, 0, 0, GetOwner(pClonk)));
      return();
    }
    Message("$NotEnoughMoney$", this(), GetValue(0, FLAG)*3/2);
  }
}
