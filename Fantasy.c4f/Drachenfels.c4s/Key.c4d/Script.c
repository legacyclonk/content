/*-- Schlüssel --*/

#strict

/* Initialisierung */

protected func Initialize()
  {
  SetAction("Key1");
  return(true);
  }


/* Effekte */

protected func Hit()
  {
  Sound("Crystal*");
  return(true);
  }