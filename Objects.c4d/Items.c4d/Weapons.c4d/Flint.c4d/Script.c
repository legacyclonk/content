/*--- Flint ---*/

#strict

public func ExplodeSize() 
{ 
  return(18); 
}

protected func Hit()
{
  Explode(ExplodeSize());
}

/* Kann in der Chemiefabrik hergestellt werden */
public func IsChemicalProduct() { return(1); }

func IsAlchemContainer() { return(true); }
func AlchemProcessTime() { return(100); }