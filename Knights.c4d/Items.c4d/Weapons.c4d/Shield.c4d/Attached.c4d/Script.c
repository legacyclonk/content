#strict

public func AttachTargetLost()
{
  ChangeDef(UnbuckledID() );
}

// In alten Szens tragen die Ritter die Schilde beim Laden noch attached
// dies wird hier auf das neue System umgestellt
protected func UpdateTransferZone()
{
  if(!UnbuckledID()->GetShieldGraphics()) return(); 
  var pClonk = GetActionTarget();
  if(SetGraphics(UnbuckledID()->GetShieldGraphics(), pClonk, GetID(pClonk), KNIG_ShieldLayer, GFXOV_MODE_ExtraGraphics))
  {
    ChangeDef(UnbuckledID());
    if(!pClonk->WearShield(this(), "Shield"))
      ChangeDef(SHIA);
  }
}

public func Unbuckle()
{
  ChangeDef(UnbuckledID() );
  Sound("Connect");
  return(1);
}

public func UnbuckledID() { return(SHIE); }

