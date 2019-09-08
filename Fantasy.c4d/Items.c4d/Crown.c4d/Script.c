#strict

protected func Initialize()
  {
  // Blinkaktivität setzen
  SetAction("Glitter0");
  return(1);
  }

// Auftreffsound
protected func Hit() { return(Sound("Crystal*")); }

// Aktivität
protected func Glittering()
  {
  // Mana-Regeneration (nur wenn der König im Freien ist)
  if (!Random(20))
    if (Contained())
      if (!Contained(Contained()))
        if (DoMagicEnergy(1, Contained())) CreateParticle("PxSpark",0,0,0,0,10,RGB(0,0,255));
  // Effekt
  return(SetAction(Format("Glitter%d", Random(5))));
  }

// Einsammelbar?
protected func RejectEntrance(pNewContainer)
  {
  // Nur aus dem König heraus (in Container ablegen), und in den König hinein (aufsammeln)
  if (GetID(Contained()) == KING) return();
  if (GetID(pNewContainer) == KING) return();
  // Ansonsten ablehnen
  return(1);
  }

public func IsMagicProduct() { return(true); }
