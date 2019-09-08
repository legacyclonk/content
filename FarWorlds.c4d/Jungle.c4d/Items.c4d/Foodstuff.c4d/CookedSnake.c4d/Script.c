#strict

public func Activate(object pClonk)
{
  [$TxtEat$]
  Eat(pClonk);
  return(1);
}

protected func Eat(object pClonk)
{
    pClonk->~Feed(50);
    RemoveObject();
    return(1);
}
