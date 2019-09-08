/*-- Segelboot --*/

#strict

protected func Initialize() { SetAction("OnLand"); }

private func Floating()
{
  // Aufrichten
  SetRDir(BoundBy(GetR(),-20,+20)/-5);
  // Nicht mehr im Wasser
  if (!(GetOCF()&OCF_InLiquid())) 
    return(SetAction("OnLand"));
  // Segeln, wenn aufgerichtet
  if (Inside(GetR(),-5,+5))
    { ChangeDef(SLBS); SetAction("LowerSail"); }
}
