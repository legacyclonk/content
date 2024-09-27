/*-- Artillerie --*/

#strict

local pCannon;
local bRotate,bDirection;
local iCooldown;

func Initialize() {
  
  pCannon=CreateObject(_GES,0,49,-1);
  SetR(RandomX(-44,44),pCannon);
  return(1);
}

func Rotation() {

iCooldown--;

if(!bRotate) return(0);

if(GetR(pCannon)> 45) {bRotate=0; SetR(GetR(pCannon)-1,pCannon); Sound("CannonStop"); return(0);}
if(GetR(pCannon)<-45) {bRotate=0; SetR(GetR(pCannon)+1,pCannon); Sound("CannonStop"); return(0);}

if(bDirection==0) {Sound("CannonRotation"); SetR(GetR(pCannon)+1,pCannon);}
if(bDirection==1) {Sound("CannonRotation"); SetR(GetR(pCannon)-1,pCannon);}

}

func ControlRight() {
    bRotate=1;
    bDirection=0;
}

func ControlLeft() {
    bRotate=1;
    bDirection=1;
}

func ControlDown() {
    bRotate=0;
    Sound("CannonStop");
}

func ControlUp() {
    ControlDown();
}

func ControlDig(object byObj) {
   if(iCooldown > 0) return(1); 
   iCooldown=50;
   var iX=Sin(GetR(pCannon),34);
   var iY=-Cos(GetR(pCannon),34)-3;
   
   Sound("Blast2");  
   var pProjectile=CreateObject(_GSS,iX,iY,GetOwner(FindObject(HZCK,0,0,0,0,0,"Push",this())));
   SetPlrView(GetController(byObj),pProjectile);
   SetXDir( Sin(GetR(pCannon),RandomX(14,16)),pProjectile,1);
   SetYDir(-Cos(GetR(pCannon),RandomX(14,16)),pProjectile,1);
   SetPlrView(GetOwner(FindObject(HZCK,0,0,0,0,0,"Push",this())),pProjectile);
   ObjectSetAction(pCannon,"Backdraft");
   CreateParticle("LightFlash",iX,iY,0,0,500,RGBa(255,255,255,32));
   var iSmoke1=14;
   while(iSmoke1) {CreateParticle("Smoke",iX,iY,0,0,RandomX(50,100),RGB(96,96,96)); iSmoke1--;}
   MuzzleFlash(RandomX(30,75),this(),iX,iY,GetR(pCannon));
}



