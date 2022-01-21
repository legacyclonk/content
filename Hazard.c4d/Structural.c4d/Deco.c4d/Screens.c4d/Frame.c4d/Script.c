/*-- Rahmen --*/
 
#strict
 
local smashed;
local width, height;
 
global func AddFrame(object pObj) {
	if(!pObj)
		if(!(pObj = this()))
			return(0);
	var tmp = CreateObject(FRME,0,25,GetOwner(pObj));
	ScheduleCall(tmp,"Set",1,0,pObj);
	return(tmp);
}
 
global func RemoveFrame(object pObj) {
	if(!pObj)
		if(!(pObj = this()))
			return(0);
	var pFrame = pObj->FindObject(FRME, 1,1);
	if(pFrame) RemoveObject(pFrame);
}
 
protected func Initialize() {
	SetClrModulation(RGBa(255,255,255,200));
	SetObjectBlitMode(GFX_BLIT_Additive);
	SetGraphics("Frame", this(),GetID(),4,1);
}
 
public func Set(object pTarget) {
	// *1000/(51-2-3) => Normiert auf 1000 und Verhältnis Zielobjekt <-> Fenster ohne Rahmen
	width = GetObjWidth(pTarget)*1000/56;
	height = GetObjHeight(pTarget)*1000/56;
	var width2 = GetObjWidth(pTarget)*1000/61;
	var height2 = GetObjHeight(pTarget)*1000/61;
 
	SetShape((-32*width)/1000,(-32*height)/1000,(64*width)/1000,(64*height)/1000);
	SetPosition(GetX(pTarget),GetY(pTarget));
 
	SetObjDrawTransform(width, 0, 0, 0, height, 0 ,0, 0);
	SetObjDrawTransform(width2, 0, 0, 0, height2, 0, 0, 4);
}
 
/* Farbmodulationen */
 
public func PanelColor(int rgba) {  // Frontscheibenmodulation
	SetClrModulation(rgba);
}
 
public func FrameColor(int rgba) {  // Rahmenmodulation
	SetClrModulation(rgba, 0, 4);
}
 
// Schaden
protected func Damage() {
	if(!smashed || (smashed < 3 && !Random(4)))
	{
		smashed++;
		CastObjects(SPRK, 7, 50);
		Sparks(15, RGB(120, 150, 135));
		Sound("CrystalHit*");
 
		var graphic = Random(4)+1;
 
		SetGraphics(Format("%s%d","Broken",graphic),this(),GetID(),smashed,1,0, 1);
		SetObjDrawTransform(width, 0,0,0, height,0,0,smashed);
	}
	if(GetDamage() >= 75)
	{
		smashed = 4;
		CastParticles("SplinterGlass", 1, 35, RandomX(-20,20), -5, 20, 20, GetClrModulation(), GetClrModulation());
		Sound("CrystalHit*");
 
		SetGraphics("BrokenGlass", this(),GetID(), 0,1,0, 1);
		SetGraphics("BrokenEnd", this(),GetID(), 1,1,0, 1);
		SetGraphics(0,0,0,2);
		SetGraphics(0,0,0,3);
	}
}

/* Serialisierung */

public func NoSerialize() { return !false; }
