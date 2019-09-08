/*-- Meteor --*/

#strict

local explosion_base;

protected func SmokeTrail() {
  Smoke(0, 0, 5);
  Smoke(0, -5, Random(7));
  var i = Random(50);
  while (i--) {
    var c = 20+Random(41);
    CreateParticle("PxSpark", Random(11)-5, Random(11)-5, 0, 0, c,
      RGBa(255, Random(100), c*4, 128));
  }
  CreateParticle("PxSpark", 0, 0, 0, 0, 20,
    RGBa(255, 155, 0, 128));
}

public func Completion() {
  explosion_base = 10;
  SetAction("Evaporate");
}

public func SetExplosionBase(new_base) {
  explosion_base = new_base;
}

protected func Hit() {
  Explode(explosion_base+Random(12));
}
