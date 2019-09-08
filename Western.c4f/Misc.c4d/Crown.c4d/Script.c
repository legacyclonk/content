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
  // Effekt
  return(SetAction(Format("Glitter%d", Random(5))));
  }
