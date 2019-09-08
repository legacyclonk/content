/*-- Groﬂer Stein --*/

#strict

protected func Initialize()
{
  Roll();
  //if (!Random(3)) 
  if(GetCon()==100)  DoCon(-80);
  return(1);
}
  
protected func Roll()
{
  var rnd = Random(3);
  if (rnd == 0) SetAction("Rolling1");
  if (rnd == 1) SetAction("Rolling2");
  if (rnd == 2) SetAction("Rolling3");
  return(1);  
}
  
protected func ContactTop()
{
  SetXDir(5 - Random(11));
  if (!Random(3)) Destruct();
  Crash(-4,-14);
  Crash(0,-16);
  Crash(4,-14);
  ShakeViewPort(GetCon() / 5, this());  
  Roll();
  return(1);
}
  
protected func ContactBottom()
{
  SetXDir(50 - Random(101));
  SetYDir(-45 - Random(10));
  Crash(-4,14);
  Crash(0,16);
  Crash(4,14);  
  Roll();
  ShakeViewPort(GetCon() / 5, this());  
  if (!Random(10)) CrashFree();
  if (!Random(8)) Destruct();
  return(1);
}
  
protected func ContactLeft()
{
  SetXDir(10 + Random(10));
  SetYDir(-40 - Random(10));
  Crash(-14,-4);
  Crash(-16,0);
  Crash(-14,4);
  Roll();
  ShakeViewPort(GetCon() / 5, this());  
  if (!Random(5)) Destruct();
  return(1);
}
  
protected func ContactRight()
{
  SetXDir(-10 - Random(10));
  SetYDir(-40 - Random(10));
  Crash(14,-4);
  Crash(16,0);
  Crash(14,4); 
  Roll();
  ShakeViewPort(GetCon() / 5, this());  
  if (!Random(5)) Destruct();
  return(1);
}

protected func Crash(x, y)
{
  Sound("RockHit*");
  if (GetCon() < 25) return(0);
  if (!Random(4)) BlastObjects(GetX() + x, GetY() + y, 3, Contained());
  if (!Random(4)) BlastObjects(GetX() + x, GetY() + y, 2, Contained());
  if (!Random(75)) Explode(Random(2) + 3, CreateObject(ROCK, x, y, -1));
  return(1);
}

protected func Destruct()
{
  var i;
  if(GetCon()>25) while (++i < 4) CastSplitRock();
  Sound("RockBreak*");
  Smoke(Random(11) - 5, Random(11) - 5, Random(25));
  Smoke(Random(11) - 5, Random(11) - 5, Random(25));
  Smoke(Random(11) - 5, Random(11) - 5, Random(25));
  Smoke(Random(11) - 5, Random(11) - 5, Random(25));
  RemoveObject();
  return(1);
}
 
protected func DoSink()
{
  if(GetYDir()<0) SetYDir(0);
}

protected func CastSplitRock()
{
  return(1);
  var pRock = CreateObject(BIRK, 0, 0, -1);
  SetXDir(-50 + Random(101), pRock);
  SetYDir(-60 + Random(80), pRock);
  SetCon(GetCon() - 5, pRock);
// AddEffect("IntArrowVanish",pRock,1,3,0,MGBW);
  return(1);
}
  
protected func CrashFree()
{
  ShakeFree(GetX() - 50 + Random(100), GetY() + Random(120), GetCon()/5);
  ShakeFree(GetX() - 50 + Random(100), GetY() + Random(120), GetCon()/5);
  ShakeFree(GetX() - 50 + Random(100), GetY() + Random(120), GetCon()/5);
  if (!Random(3)) Sound("Earthquake");
  return(1); 
}  
