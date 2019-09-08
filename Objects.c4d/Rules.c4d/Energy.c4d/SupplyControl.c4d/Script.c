/*-- Energieversorgung --*/

#strict

public func ContextEnergy(pCaller)
{
  [$TxtEnergysupply$|Image=CXEC|Condition=NeedsEnergy]
  SetCommand(pCaller, "Energy", this());
  return(1);
}
  
public func NeedsEnergy()
{
  // Energieverbrauch nicht aktiviert
  if (!FindObject(ENRG)) return(0);  
  // Objekt ist kein Energieverbraucher (oder noch nicht fertig)
  if (!(GetOCF() & OCF_PowerConsumer)) return(0);
  // Energieleitung ist bereits angeschlossen
  if (FindObject(PWRL,0,0,0,0,0,"Connect",this())) return(0);
  // Objekt braucht noch Energieversorgung
  return(1);
}
