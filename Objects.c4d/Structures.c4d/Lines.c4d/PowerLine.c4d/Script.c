/*--- Energieleitung ---*/

#strict

protected func Initialize()
{
  SetAction("Connect");  
  SetVertex(0,0,GetX()); SetVertex(0,1,GetY());
  SetVertex(1,0,GetX()); SetVertex(1,1,GetY());
  SetPosition(0, 0, this() );
}

private func Transfer()
{
  var energy = GetEnergy(GetActionTarget()) / 3;
  var limit = 100 - GetEnergy(GetActionTarget(1) );
  energy = Min(Min(energy, limit), 20);
  DoEnergy(-energy, GetActionTarget() );
  DoEnergy(energy, GetActionTarget(1) );
}

protected func LineBreak(bool fNoMsg)
{
  Sound("LineBreak");
  if(!fNoMsg) BreakMessage();
}
  
private func BreakMessage()
{
  // Meldung bei Leitungsbausatz am einen oder anderen Ende ausgeben
  var line = GetActionTarget(0);
  if(GetID(line) != LNKT) line = GetActionTarget(1);

  Message("$TxtLinebroke$", line);
}
