/*-- Geschütz --*/

#strict

func Initialize() {
   
  return(1);
}

func EjectBullet() {
  CreateParticle("BulletCasing",0,0,RandomX(-30,0),RandomX(-30,-10),40);
}
