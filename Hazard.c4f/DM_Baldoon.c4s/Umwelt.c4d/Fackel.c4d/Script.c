/*-- Fackel --*/

#strict

local light;

func Initialize() { 
CreateParticle("LightFlash2",0,-21,0,0,1500,RGBa(255,255,1128,64));
  return(1);
}

func Effect() {
   CreateParticle("Fire",RandomX(-2,2),-21,0,RandomX(-6,-12),RandomX(40,60),RGBa(255,RandomX(224,255),0,128));
   CreateParticle("Fire2",RandomX(-2,2),-21,0,RandomX(-8,-14),RandomX(40,60),RGBa(255,255,255,64));
   }
