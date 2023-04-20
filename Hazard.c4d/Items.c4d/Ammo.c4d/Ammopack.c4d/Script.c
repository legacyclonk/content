#strict

public func IsAmmoPacket() { return(true); }
public func AmmoID()       { return(STAM); }
public func AmmoCount()    { return(100); }

private func OnTransfer() {}

protected func Activate(object pObj) {		// Doppelgraben
  return(TransferAmmo(pObj));
}

public func MayTransfer(object pObj)
{
  if(!pObj) return(false);
  var MaxAmmo = AmmoID()->~MaxAmmo();
  if(MaxAmmo)
    if(GetAmmo(AmmoID(),pObj) + AmmoCount() > MaxAmmo)
      return(false);
  return(true);
}

public func TransferAmmo(object pObj)		// Ammo dem Clonk geben
{
  if(!pObj) return(false);
  if(NoAmmo()) return(false);
  
  // nicht wenn er schon zu viel hat
  if(!MayTransfer(pObj)) {
    PlayerMessage(GetOwner(pObj),"$NotMoreAmmo$",pObj,AmmoID());
    return();
  }

  // Nachricht ausgeben
  HelpMessage(GetOwner(pObj),"$Collected$",pObj,AmmoCount(),AmmoID());
  DoAmmo(AmmoID(),AmmoCount(),pObj);
  pObj->~AmmoTransferred();
  if(!OnTransfer()) RemoveObject();

  return(true);
}
