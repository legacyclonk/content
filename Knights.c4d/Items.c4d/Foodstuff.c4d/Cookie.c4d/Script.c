/*-- Keks --*/

#strict

public func Activate(object pClonk)
{
  [$TxtEat$]
  Eat(pClonk);
  return(1);
}

public func Eat(object pClonk)
{
  pClonk->~Feed(30);
  RemoveObject();
  return(1);
}

/* Produkteigenschaften */

public func IsOvenProduct() { return(1); }
