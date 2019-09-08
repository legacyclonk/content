/*-- Rüstung --*/

#strict

/* Status */
public func IsArmor()
{
  return(1);
}

/* Schutz */
public func ShieldFactor()
{
  return(50);
}

/* Grafik */
public func GetGraphics()
{
  return("Armored");
}

/* Anlegen */
public func Activate(pClonk)
{                  
  [$CtrlActivateDesc$]
  // Der Clonk kann dieses Objekt nicht verwenden
  if (!pClonk->~CanUse(GetID())) return(0);
  // Ritter trägt bereits eine Rüstung?
  if (pClonk->KNIG::HasArmor()) return(0);
  // Anlegen  
  return(pClonk->WearArmor(this()));
}
  
/* Aufschlag */  
protected func Hit()
{
  Sound("ClonkHit*");
}

/* Kann geschmiedet werden */
protected func IsAnvilProduct() { return(1); }
