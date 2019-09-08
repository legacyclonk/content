/*-- Telegrafenmast (beweglich) --*/


#strict

protected func ControlDigDouble()
{
  [$CtrlErect$]
  // Nur wenn Mast fast senkrecht
  if(!(Inside(GetR(),-20,20)||Inside(GetR(),340,359)))
    return(Message("$MsgErectFirst$",this()));
  PutUp();
  return(1);
}
  
private func PutUp()
{
  // Aufgestellten Mast fixieren
  SetR(0);
  Sound("Connect");
  DoDamage(-300);
  SetCategory(C4D_StaticBack());
  ChangeDef(TEL1);
}
