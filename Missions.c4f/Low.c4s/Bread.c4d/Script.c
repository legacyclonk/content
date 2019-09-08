/*-- Brot --*/

#strict

public func Activate(object pClonk)
{
  [$TxtEat$]
  Eat(pClonk);
  return(1);
}

public func Eat(object pClonk)
{
  pClonk->~Feed(80);
  RemoveObject();
  return(1);
}

/* Produkteigenschaften */

public func IsOvenProduct() { return(1); }
