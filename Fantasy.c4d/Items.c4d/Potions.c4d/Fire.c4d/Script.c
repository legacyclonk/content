/* Feuertrank */

// uuund: (Glitzer+Fade)-effekt
// sowie: Funkenfeffekt nach dem Trinken

#strict

protected func Hit() { Sound("Crystal*"); }

public func IsPotion() { return(1); }
public func BrewTime() { return(6*35); }

private func EffectDuration() { return(30*35); } // ~30s

public func Glow(iTime)   { return(RGB(
  80+Sin(iTime*2 ,40),
  40+Cos(iTime*3/2 ,40),
  0));
}

public func Spark() { return(RGBa(255,Random(2)*100,0,120)); }

public func Light(iTime) { return(RGBa(255,Random(2)*100,0,RandomX(130,240))); }
public func LightSpeed() { return(5); }
public func LightSize()  { return(120); }

func Initialize() { AddEffect("Potion",this(),100,1,0,PFIR); }

protected func Activate(object pClonk) {
  // Sound
  Sound("Drink");
  // Aktion
  pClonk->~Drink(this());
  // Effekt starten (zeitverzögert)
  Schedule("AddEffect(\"BanBurnPotion\",this(),210,1,0,PFIR,PFIR->EffectDuration())",80,0,pClonk);
  // fertig..
  RemoveObject();
  return(1);
}

private func FxBanBurnPotionStart (object pTarget, int iNumber, int iTemp, int iTime) { 
  if(iTemp) return();

  AddEffect("Obj3D",pTarget,300,1,0,PFIR,PFIR);

  // Zeit speichern
  EffectVar(0,pTarget,iNumber)=iTime;
  // löschen
  if(OnFire(pTarget)) Extinguish(pTarget);
  // Färben
  SetObjectBlitMode(4,pTarget);
  SetClrModulation(RGB(255,150,100),pTarget);
}

private func FxBanBurnPotionDamage(object pTarget, int iNumber, int iDmgEngy, int iCause) {
  if(iCause != 33 && iCause != 35) return(iDmgEngy);
  // Effekt
  CreateParticle("Blast",GetX(pTarget),GetY(pTarget),0,0,5*30);

  // Feuerschaden: keiner
  if(iCause == 35) return(0);
  // Explosionsschaden: hälfte
  if(iCause == 33) return(iDmgEngy/2);
}

private func FxBanBurnPotionEffect(string szNewEffectName, object pTarget) {
  // nicht anbrennen lassen
  if(WildcardMatch(szNewEffectName,"*Fire*")) {
    CreateParticle("Blast",GetX(pTarget),GetY(pTarget),0,0,5*30);
    return(-1);
  }
  // gleichen Effekt zusammenfassen (Trank zweimal hintereinander getrunken)
  if(szNewEffectName eq "BanBurnPotion") return(-2);
}

private func FxBanBurnPotionAdd(object pTarget, int iNumber, string szNewEffectName, int iNewEffectTimer, int iTime) {
  // Effekt übernehmen
  EffectVar(0,pTarget,iNumber) += iTime;
}

private func FxBanBurnPotionTimer(object pTarget, int iNumber, int iTime) {
  // grafischer Effekt
   var x,y,a,xdir,ydir;

   a = Random(360);
   x = Sin(a,8);
   y = Cos(a,16);
   CreateParticle("Fire2",GetX(pTarget),GetY(pTarget),x,y,80,RGBa(255,255,255,140),pTarget,!!Random(8));

  // Zeit abgelaufen!
  if(iTime >= EffectVar(0,pTarget,iNumber)) return(-1);
}

private func FxBanBurnPotionStop(object pTarget, int iNumber, int iReason, bool fTemp) {
  if(fTemp) return();

  for(var num,i=0; num=GetEffect("Obj3D",pTarget,i); ++i) { 
    if(EffectVar(0,pTarget,num) == PFIR)
      { RemoveEffect(0,pTarget,num); break; } }

  // Zurück färben
  SetObjectBlitMode(0,pTarget);
  SetClrModulation(RGB(255,255,255),pTarget);
}


// Glitzereffekt des Tranks

private func FxPotionStart(object pTarget, int iNumber, int iTemp) {
  if(iTemp) return();
  SetObjectBlitMode(2,pTarget);
}

private func FxPotionTimer(object pTarget, int iNumber, int iTime) {
  SetClrModulation(pTarget->Glow(iTime),pTarget);
  if(!Contained(pTarget)) if(pTarget->~Spark())  // opt
    CreateParticle("PSpark",RandomX(-3,3)+GetX(pTarget),RandomX(-3,3)+GetY(pTarget),0,0,40,pTarget->Spark());
}


// Effekt für nach dem Trinken - vereinfachte (spezialisierte) Version von jsbs 3D-Engine

func FxObj3DStart(object pTarget, int iNumber, int iTemp, id idCallback) {
  if(iTemp) return();

  EffectVar(0,pTarget,iNumber) = idCallback;

  EffectVar(1,pTarget,iNumber) = Random(360);
  EffectVar(2,pTarget,iNumber) = Random(360);
  EffectVar(3,pTarget,iNumber) = Random(360);

}

func FxObj3DTimer(object pTarget, int iNumber, int iTime) {

  var idCallback = EffectVar(0,pTarget,iNumber);

  var pitch = EffectVar(1,pTarget,iNumber);
  var yaw = EffectVar(2,pTarget,iNumber);
  var roll = EffectVar(3,pTarget,iNumber);
  
  pitch += idCallback->LightSpeed();
  yaw += idCallback->LightSpeed()/2;
  roll += idCallback->LightSpeed()/6;


  // Punkte definieren
  var zoom = 14;

  var point1x = -zoom;
  var point1y = 0;
  var point1z = 0;

  var point2x = +zoom;
  var point2y = 0;
  var point2z = 0;

  var point3x = 0;
  var point3y = -zoom;
  var point3z = 0;

  var point4x = 0;
  var point4y = +zoom;
  var point4z = 0;

  var point5x = 0;
  var point5y = 0;
  var point5z = +zoom;

  var point6x = 0;
  var point6y = 0;
  var point6z = -zoom;

  // Drehung
  for(var i = 1; i <= 6; i++)
  {
    var tempx = VarN(Format("point%dx", i));
    var tempy = VarN(Format("point%dy", i));
    VarN(Format("point%dx", i)) = Cos(roll,tempx) - Sin(roll,tempy);
    VarN(Format("point%dy", i)) = Cos(roll,tempy) + Sin(roll,tempx);
  }

  for(var i = 1; i <= 6; i++)
  {
    var tempy = VarN(Format("point%dy", i));
    var tempz = VarN(Format("point%dz", i));
    VarN(Format("point%dy", i)) = Cos(pitch,tempy) - Sin(pitch,tempz);
    VarN(Format("point%dz", i)) = Cos(pitch,tempz) + Sin(pitch,tempy);
  }

  for(var i = 1; i <= 6; i++)
  {
    var tempx = VarN(Format("point%dx", i));
    var tempz = VarN(Format("point%dz", i));
    VarN(Format("point%dx", i)) = Cos(yaw,tempx) - Sin(yaw,tempz);
    VarN(Format("point%dz", i)) = Cos(yaw,tempz) + Sin(yaw,tempx);
  }

  var zbuffer = 5*zoom;
  point1z += zbuffer;
  point2z += zbuffer;
  point3z += zbuffer;
  point4z += zbuffer;
  point5z += zbuffer;
  point6z += zbuffer;

  var size = idCallback->LightSize();
  CreateParticle("PSpark",GetX(pTarget) + zbuffer*point1x/point1z, GetY(pTarget) + zbuffer*point1y/point1z,0,0,size,idCallback->Light(iTime)    ,pTarget,point1z>zbuffer);
  CreateParticle("PSpark",GetX(pTarget) + zbuffer*point2x/point2z, GetY(pTarget) + zbuffer*point2y/point2z,0,0,size,idCallback->Light(iTime+30) ,pTarget,point2z>zbuffer);
  CreateParticle("PSpark",GetX(pTarget) + zbuffer*point3x/point3z, GetY(pTarget) + zbuffer*point3y/point3z,0,0,size,idCallback->Light(iTime+60) ,pTarget,point3z>zbuffer);
  CreateParticle("PSpark",GetX(pTarget) + zbuffer*point4x/point4z, GetY(pTarget) + zbuffer*point4y/point4z,0,0,size,idCallback->Light(iTime+90) ,pTarget,point4z>zbuffer);
  CreateParticle("PSpark",GetX(pTarget) + zbuffer*point5x/point5z, GetY(pTarget) + zbuffer*point5y/point5z,0,0,size,idCallback->Light(iTime+120),pTarget,point5z>zbuffer);
  CreateParticle("PSpark",GetX(pTarget) + zbuffer*point6x/point6z, GetY(pTarget) + zbuffer*point6y/point6z,0,0,size,idCallback->Light(iTime+150),pTarget,point6z>zbuffer);

  EffectVar(1,pTarget,iNumber) = pitch;
  EffectVar(2,pTarget,iNumber) = yaw;
  EffectVar(3,pTarget,iNumber) = roll;

}