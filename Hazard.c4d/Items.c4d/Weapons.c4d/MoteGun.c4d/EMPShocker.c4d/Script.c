/* EMP */

#strict
#include SHT1

local Speed, Power, Ang;

public func Launch(int iAngle, int iSpeed, int iDist, int iSize, int iTrail, int iDmg)
{
  _inherited(iAngle, iSpeed, iDist, iSize, iTrail, iDmg);

  Speed = iSpeed*100;
  Power = iDist/5;
  Ang = iAngle;
  Sound("ShockerLoop",0,0,0,0,1);

  return(1);
}

private func CreateTrail(int iSize, int iTrail) {
  // no trail
}

/* Fliegen */

private func Traveling() {
  if(GetActTime() <1)
    return(0);

  var laser = CreateObject(LASR, 0,0, GetOwner());
  laser->Set(Random(360), 5, BoundBy(Power + RandomX(-10,10), 5, 55), 5, laser, this());
  SetClrModulation(Color(GetActTime()), laser);

  CreateParticle("EMPShock", 0,0, GetXDir(), GetYDir(), Power * RandomX(200,500) / 70, SetRGBaValue(Color(GetActTime())), this());

  //LaserLight(Power,RGB(255,255,255),RandomX(-Power/2,Power/2), RandomX(-Power/2,Power/2));

  Power--;
  if(!Power) return(Remove());

  // Geschwindigkeit
  SetSpeed(+Sin(Ang,Speed/100),-Cos(Ang,Speed/100));

  return(_inherited());
}

/* Treffer */

private func HitObject(object pObject) {
  var angle = Angle(GetX(),GetY(),GetX(pObject),GetY(pObject));
  var dist = Distance(GetX(),GetY(),GetX(pObject),GetY(pObject));

  //Objekte schocken (Objekt auf schockbarkeit prüfen)
  if(pObject->~IsMachine())
   if(pObject->~EMPShock())
    {
    var laser = CreateObject(LASR, 0,0, GetOwner());
    laser->Set(angle, 10, dist, 20, pObject, pObject);
    SetClrModulation(Color(GetActTime()), laser);
    Sound("EMPShock");
    }

  if(GBackSolid())
    Remove();
}

private func Hit() {
  Sparks(20,Color(GetActTime()));
  for(var i = 0 ; i < 7 ; i++)
  {
    var laser = CreateObject(LASR, 0,0, GetOwner());
    laser->Set(Random(360), 5, RandomX(10,30), 5, laser, laser);
    SetClrModulation(Color(GetActTime()), laser);    
  }

  Remove();
}

/* Effekt für Trefferüberprüfung aus SHT1 kopieren und für Schockwelle ändern weil alle Gegenstände*/
public func FxHitCheckTimer(object target, int effect, int time)
{
  var obj;
  // Oh man. :O
  var oldx = EffectVar(0, target, effect);
  var oldy = EffectVar(1, target, effect);
  var newx = GetX(target);
  var newy = GetY(target);
  var dist = Distance(oldx, oldy, newx, newy);
  EffectVar(0, target, effect) = GetX(target);
  EffectVar(1, target, effect) = GetY(target);

  if(dist < 10) // Normal checken
  {
    while(obj=FindObject(0, newx-50, newy-50, 100, 100, 0, 0, 0, NoContainer(), obj))
     if(obj ->~IsMachine())
      if(PathFree(newx, newy, GetX(obj), GetY(obj)))
      {
        target-> ~HitObject(obj);
      }
    return();
  }
  
  var steps = Abs(Distance(oldx, oldy, newx, newy))/5; // Anzahl der Schritte, wenn wir 5 Pixel wollen

  var curx, cury;
  // Schleife
  for(var i = 1; i <= steps; ++i)
  {
    curx=oldx+(newx-oldx)*i/steps;
    cury=oldy+(newy-oldy)*i/steps;
    while(obj=FindObject(0, curx-50, cury-50, 100, 100, 0, 0, 0, NoContainer(), obj))
     if(obj->~IsMachine())
      if(PathFree(curx, cury, GetX(obj), GetY(obj)))
      {
        SetPosition(curx, cury, target);
        target-> ~HitObject(obj);
      }
  }
  EffectVar(0, target, effect) = GetX(target);
  EffectVar(1, target, effect) = GetY(target);
}

private func GlowColor() {
  return(RGB(50,130,255));
}

private func Color(int iATime) {
  var iPrg = 20*iATime/iTime;
  return(RGBa(255-iPrg,255-iPrg,255,100+iPrg*3));
}

public func TrailColor(int iATime) {
  var iPrg = 40*iATime/iTime;
  return(RGBa(155+iPrg,155+iPrg,255,140+iPrg*2));
}
