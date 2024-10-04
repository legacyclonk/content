/*-- Zielscheibe --*/

#strict

func IsBulletTarget() { return(1); }

func Initialize() {
  //Zielscheibe dem Scriptspieler zuordnen
  SetOwner(1);
}

func Damage() {
  Sound("ArrowHit");

  CastParticles("Splinter",5,30,0,0,25,35,RGB(255,0,0));
  if(GetActionTarget())
    GetActionTarget()->JobDone();
  RemoveObject();
}
