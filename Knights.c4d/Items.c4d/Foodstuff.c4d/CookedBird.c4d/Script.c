/*-- Gekochter Vogel --*/

#strict

public func Activate(object pClonk)
{
  [$TxtEat$]
  Eat(pClonk);
  return(1);
}

public func Eat(object pClonk)
{
  pClonk->~Feed(60);
  RemoveObject();
  return(1);
}
