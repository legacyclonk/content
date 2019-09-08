/*-- Sack --*/

#strict

protected func Initialize() { SetAction("Be"); }

protected func Hit() { Sound("RockHit*"); }

/* Wird eingesammelt */

protected func Entrance() {
  // Im einem Behälter ist der Sack nehmbar
  if(GetID()==SACK) ChangeDef(SAC1);
  return(1);
}
  
/* Ins Freie */

protected func Departure() {

  /*Log("HALOOoooooooooo");
  Message("HIER BIN ICH!!1111111",this());
  CastParticles("MSpark",100,100,0,0,50,500,RGB(255,255,255),RGB(255,255,255));*/


  // Im Freien ist der Sack nicht nehmbar
  if (GetID()==SAC1) ChangeDef(SACK);

  SetAction("Be");
  //Sound("Winning");

  return(1);
}

/* Nicht mehr aufnehmen als er kann */

protected func RejectCollect(idSak) {
  if (idSak==SACK) return(1);
  if (idSak==SAC1) return(1);
  if(ContentsCount()<=10) return(0);
  return(1);
}


public func ControlDigDouble(pClonk) {
  [$TxtCollect$]
  Enter(pClonk);
  return(1);
}