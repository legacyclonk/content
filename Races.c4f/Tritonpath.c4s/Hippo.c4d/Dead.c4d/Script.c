/*-- Toter Fisch --*/

#strict

protected func Construction() 
{
  var pEnv;
  if (pEnv=FindObject(CLFS))
    pEnv->CLFS::Colorize(this());
  else
    SetColorDw(RGB(255,255,255));
}

private func Decaying() 
{ 
  DoCon(-4); 
}

public func Cook()
{
  // Verwesende Fische können nicht mehr gekocht werden
  if (GetAction() S= "Decay") return(0);
  // Kochen
  ChangeDef(CFSH);
  return(1);
}
