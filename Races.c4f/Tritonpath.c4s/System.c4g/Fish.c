/*-- Fisch --*/

#strict
#appendto FISH

public func Eat(object pByObject)
{
  pByObject->~Feed(50);
  pByObject->DoBreath(500);
  RemoveObject();
  return(1);
}
