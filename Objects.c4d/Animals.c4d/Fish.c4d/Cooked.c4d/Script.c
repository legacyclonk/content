/*-- Gekochter Fisch --*/

#strict

protected func Construction() 
{
  var pEnv;
  if (pEnv=FindObject(CLFS))
    pEnv->CLFS::Colorize(this());
  else
    SetColorDw(RGB(255,255,255));
}

public func Activate(pByObject)
{
  [$CtrlEatDesc$]
  return(Eat(pByObject));
}

public func Eat(pByObject)
{
  pByObject->~Feed(80);
  RemoveObject();
  return(1);
}

/* Aufwertungszauberkombo: Mit Fisch wird der Clonk zum Aquaclonk */
public func GetRevaluationCombo(object pClonk) { return(ACLK); }
