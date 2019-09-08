/*-- Hydroclonk --*/

#strict
#appendto HCLK

public func Recruitment()
{
  SetPhysical("Breath", 250000, 2);
}

//! Der Hydroclonk braucht einen Sattel zum Reiten. 
public func NeedSaddle()  
{  
  return(1);  
}
