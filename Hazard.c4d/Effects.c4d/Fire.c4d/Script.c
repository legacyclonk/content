/*-- Feuer --*/

#strict

//Farben
static const FIRE_Blue  = 340810495; // RGBa(80,90,255,20);
static const FIRE_Green = 169148190; // RGBa(20,255,30,10);
static const FIRE_Red   =  33554431; // intern geregelt

/* Schadenszeugs und so! */

global func FxDmgCheckStart(object pTarget, int iEffectNumber, int iTemp) {
  if(iTemp)
    return();
  if(GetEffectCount("DmgCheck",pTarget) > 1)
    return(-1);
}

global func FxDmgCheckDamage(object pTarget, int iEffectNumber, int iDmgEngy, int iCause) {
  var type;
  //Explosion
  if(iCause == 1 || iCause == 33)
    type = DMG_Explosion;
  //Feuer
  else if(iCause == 2 || iCause == 35)
    type = DMG_Fire;
  //Säure (Bio)
  else if(iCause == 38)
    type = DMG_Bio;
  //Melee = Clonk zu Clonk Kampf
  else if(iCause == 34 || iCause == 40)
    type = DMG_Melee;
  //Was anderes -> nichts tun.
  else
    return(iDmgEngy);
  
  //Ziel sagen, welcher Schadenstyp gemach wurde
  if(type)
    pTarget->~LastDamageType(type);
  
  var red = pTarget->~OnDmg(iDmgEngy/100,type); 
  //reduzierten Schaden berechnen
  var dmg;
  dmg = (100-red)*iDmgEngy;
  // OnHit aufrufen
  pTarget->~OnHit(dmg/100, type);
  return(dmg/100); //und zurückgeben
}

/* Der Feuereffekt. Ganz ohne Feuer! :) */

global func AddFireEffect(object pTarget, int iDuration, int dwColor,bool bOverride, int iSize) {
  if(!pTarget)
    pTarget = this();
  return(AddEffect("FireEffect",pTarget,2,4,0,0, iDuration, dwColor, bOverride,iSize));
}

global func FxFireEffectStart(object pTarget, int iEffectNumber, int iTemp, int iDuration, int dwColor, bool bOverride, int iSize) {
  if(iTemp)
    return(0);
  EffectVar(0,pTarget,iEffectNumber) = iDuration; //Wie lange der Effekt anhalten soll
  EffectVar(1,pTarget,iEffectNumber) = dwColor; //die Farbe des Feuers. Teh Rok. :)
  //Größe des Feuerzeugs. Relativ zur Standardgröße
  if(iSize)
    EffectVar(2,pTarget,iEffectNumber) = iSize;
}

global func FxFireEffectTimer(object pTarget, int iEffectNumber, int iEffectTime) {
  if(Contained(pTarget))
    return(1);
  
  var firemode;
  if(GetOCF(pTarget) & OCF_Living)
    firemode = 1;
  
  var width, height, yoff;
  var id = GetID(pTarget);
  
  width = Max(GetObjWidth(pTarget),1);
  height = GetObjHeight(pTarget);
  yoff = height/2-GetDefFireTop(id);
  
  var count = Sqrt(width*height)/4;
  var basesize = 30,
      diffsize = 10,
      relsize = 12;
  var x = GetX(pTarget),
      y = GetY(pTarget);
  var xdir, ydir, con, wdtcon, a, size, wind;
  var clr2;
  
  con=wdtcon=Max(100*GetCon(pTarget)/100,1);
  if(!GetDefStretchGrowth(id))
    if(wdtcon<100)
      wdtcon=100;
      
  x += GetDefOffset(id,0)+GetObjWidth(pTarget)/2;
  y += GetDefOffset(id,1)+GetObjHeight(pTarget)/2;
  
  count = Max(2,count*wdtcon/100);
  
  a = Sqrt(Sqrt(width*height)*(con+20)/120)*relsize;
  
  wind = GetWind(x,y)/3;
  
  clr2 = EffectVar(1,pTarget,iEffectNumber);
  
  var countmod = GetEffectCount("FireEffect",pTarget);
  
  for(var i=0; i < count/countmod; ++i) {
    size = Random(diffsize+1) + basesize - diffsize/2-1 + a + EffectVar(2,pTarget,iEffectNumber);
    
    var clr, particle;
    if(i < count/(2*countmod)) {
      clr = 32004000 + ((Random(59) + 196) << 16);
      particle = "ColorFire1";
      if(IsAlphaOnly(clr2))
        particle = "Fire1";
    }
    else {
      clr = 16777215;
      particle = "ColorFire2";
      if(IsAlphaOnly(clr2))
        particle = "Fire2";
    }
//    if(!firemode)
  //    clr += 62000000;
    
/*    var clrr,clrg,clrb,clra;
    clrr = BoundBy(ModulateColor(GetRGBValue(clr,1),GetRGBValue(clr2,1)),65536,16711680);
    clrg = BoundBy(ModulateColor(GetRGBValue(clr,2),GetRGBValue(clr2,2)),256,65280);
    clrb = BoundBy(ModulateColor(GetRGBValue(clr,3),GetRGBValue(clr2,3)),1,255);
    clra = ModulateColor(GetRGBValue(clr,0),GetRGBValue(clr2,0));
    clr = RGBa(clrr,clrg,clrb,clra); */

    clr = ModulateColor(clr,clr2);
    
    var randx = Random(width+1)-width/2-1;
    var px = randx*wdtcon/100;
    var py = yoff*con/100;
    
    if(firemode) {
      py -= px*px*100/width/wdtcon;
      
      xdir = randx*con/400 - px/3 + wind - GetXDir(pTarget,1)*3;
      ydir = -Random(15+height*con/300) - 1 - GetYDir(pTarget,1)*3;
    }
    else {
      xdir = -GetXDir(pTarget,1)*3;
      ydir = -GetYDir(pTarget,1)*3;
      if (!ydir)
        ydir = -Random(13+height/4)-1;
      ydir -= EffectVar(2,pTarget,iEffectNumber)/(size/3);
    }
    
//    Log("%s %d %d %d %d %d %s:",particle, x+px, y+py, xdir, ydir, size, GetName(pTarget));
    
    CreateParticle(particle,x+px,y+py, xdir, ydir, size, clr, pTarget, !!Random(2));
  }
  if(EffectVar(0,pTarget,iEffectNumber) && EffectVar(0,pTarget,iEffectNumber) < iEffectTime)
    return(-1);
}

global func FxFireEffectEffect(string szNewEffectName, object pTarget, int iEffectNumber, int iNewEffectNumber, int iDuration, int dwColor, bool bOverride) {
  if(szNewEffectName S= "Fire" || szNewEffectName S= "FireEffect")
    if(bOverride)
      return(-2);
  
}

global func FxFireEffectAdd(object pTarget, int iEffectNumber, string szNewEffectName, int iNewEffectTimer, int iDuration, int dwColor, bool bOverride) {
  EffectVar(0,pTarget,iEffectNumber) = iDuration;
  EffectVar(1,pTarget,iEffectNumber) = dwColor;
  return(1);
}

global func IsAlphaOnly(int iColor) {
  return((GetRGBaValue(iColor,0) && GetRGBaValue(iColor,1) == 255 && GetRGBaValue(iColor,2) == 255 && GetRGBaValue(iColor,3) == 255) || !iColor);
}
