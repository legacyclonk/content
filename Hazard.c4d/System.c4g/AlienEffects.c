/*-- Alienpartikeleffekte --*/

#strict

global func AlienSplatter(int iSize, int iX, int iY, int iColor) {
	// not on sky
	if(GetMaterialVal("Density","Material",GetMaterial(iX,iY)) != 0
       || GetMaterial(iX,iY) == -1) return();
	
	// behind everything (like the burn mark of the explosion)
	var boom;
	if(!ObjectCount(BOOM)) boom = CreateObject(BOOM,0,0,-1);
	else boom = FindObject(BOOM);
	
	var r = Random(360);
	var xdir = Sin(r,100);
	var ydir = -Cos(r,100);
	CreateParticle("AlienSplatter",iX,iY,xdir,ydir,iSize*5,iColor,boom,1);
}

global func AlienBlood(int iSize, int iX, int iY, int iColor, object attach) {
	
	var rand = Random(360);
	for(var i=0; i<5; ++i) {
		var size = iSize*RandomX(10,20)/15*GetCon()/100;
		var angle = rand+i*RandomX(50,80);
		var xdir = Sin(angle,100);
		var ydir = -Cos(angle,100);
		var xoff = Sin(angle,iSize/2-5);
		var yoff = -Cos(angle,iSize/2-5);
		CreateParticle("AlienBlood",iX+xoff,iY+yoff,xdir,ydir,iSize*5,iColor, attach);
	}
	
}

global func AlienHive(int iSize, int iX, int iY, int iColor, object attach, bool foreground) {
	
	// not on sky
	//if(GetMaterialVal("Density","Material",GetMaterial(iX,iY)) != 0
    //   || GetMaterial(iX,iY) == -1) return();
	
	var boom;	
	// to which object (if any)
	if(attach) boom = attach;
	else if(!foreground) {
		if(!ObjectCount(BOOM)) boom = CreateObject(BOOM,0,0,-1);
		else boom = FindObject(BOOM);
	}
	
	var r = Random(360);
	var xdir = Sin(r,100);
	var ydir = -Cos(r,100);
	CreateParticle("AlienHive",iX,iY,xdir,ydir,iSize*5,iColor,boom,!foreground);
}

/* Alienpartikel */

global func AddAlienFlareEffect(object target, int size) {
	if(!target) target = this;
	if(!target) return;
	
	AddEffect("AlienFlareEffect", target, 1, 1, 0, 0,size); 
	
}

global func FxAlienFlareEffectStart(object target, int effect, int temp, size) {
	EffectVar(0,target,effect) = size;
}

global func FxAlienFlareEffectTimer(object target, int effect, int time)
{
  var xdir = GetXDir(target);
  var ydir = GetYDir(target);
  var size = EffectVar(0,target,effect)+RandomX(-100,100);
 
  if(!Contained(target))
  	CreateParticle("FrSprk", GetX(target), GetY(target), xdir/3+RandomX(-10, 10), ydir/3+RandomX(-10, 10),
                 size, RGBa(160,255,0,0)); 
}
