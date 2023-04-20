/*-- Gasflasche --*/

#strict

local damaged;

protected func Damage()
{
  // Die geht schnell hoch
  if(GetDamage() < 20) return();
  if(damaged) return();
  damaged = 1;
  // Es gibt drei Explosionsmöglichkeiten
  var rand = Random(3);
  // 1 - viele Explosionen!
  if(!rand--)
    {
    // TIM1 nicht geladen
    if(!GetCategory(0, TIM1)) return(InstaExplode());
    // TIM1 ist geladen. Also kleine Bomben verteilen!
    for(var i=0, tim ; i < 4 ; i++)
      {
      tim = CreateObject(TIM1, RandomX(-40, 40), RandomX(-30,30), -1);
      Schedule(Format("Explode()", 15+Random(20)), 5 + Random(15), 0, tim);
      }
    // Und explodieren!
    return(InstaExplode());
    }
  // 2 - Explosion + Schleudern
  if(!rand--)
    {
    InstaExplode(1);
    SetSpeed(RandomX(-25,25), RandomX(-55,-30));
    SetRDir(RandomX(-10,10));
    if(!GetRDir()) SetRDir(2);
    SetClrModulation(RGB(50,50,50));
    return(1);
    }
  // 3 - Raketenartig rumfliegen!
  if(!rand)
    {
    Explode(20+Random(10), CreateObject(HSGN,0,0,-1));
    Incinerate();
    Sound("Inflame");
    Sound("Fire",0,0,0,0,1);
    SpeedUp();
    return(1);
    }
}

private func InstaExplode(bool noRemove)
{
  // Effekte
  CastParticles("Wave", 7+Random(5), RandomX(35,55), 0,0, 20,40, RGBa(150,150,200,50), RGBa(150,150,255,100));
  AddLightFlash(60, 0,0, RGBa(255,255,200,100));
  Sparks(7+Random(5), RGBa(255,255,150,100));
  // Explosion
  Explode(25+Random(20), CreateObject(HSGN, 0,GetDefHeight(HSGN)/2, -1));
  if(noRemove) return(1);
  // Weggehen
  RemoveObject();
}

private func SpeedUp(int iTime)
{
  // aufhören
  if(iTime > 80) { Sound("Fire",0,0,0,0,-1); return(InstaExplode()); }
  // Losdüsen
  SetR(GetR()+RandomX(-10,10));
  var xdir, ydir;
  xdir = Sin(GetR(), 35);
  ydir = -Cos(GetR(), 35);
  SetSpeed(xdir, ydir);
  Smoke(0,0,RandomX(5,15));
  ScheduleCall(this(), "SpeedUp", 1, 0, iTime+1);
}
