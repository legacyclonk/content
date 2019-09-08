/*-- Dynamit --*/

#strict

#include _STY

protected func Initialize()
{
  if(GetY()<0)	
  {
    iFallSpeed = GetFallSpeed() + GetSpeedBoost();
    SetAlive(1);	
  }
  else Activate();
}

public func GetSpeedBoost() { return(10); }

public func ShotHit(iPlr)
{
   DoExplode(18);
}

protected func Hit() 
{
   Sound("WoodHit*");
   Activate();
}

protected func Timer()
{
  if(!iFallSpeed) return();
  SetYDir(iFallSpeed);
  SetRDir(5);
  var x,y;
  x = Cos(GetR()+90, 6);
  y = Sin(GetR()+90, 6);

  if(Contained()) { x=0; y=0; }

  for(var amount = RandomX(1,3) ; amount ; amount--)
    CreateParticle("DynamiteSpark", x, y, x*3+RandomX(-10,10), y*3+RandomX(-10,10), 20, RGBa(255,255,0,100));
}

/* Aktivierung */
public func Activate (pCaller)
{
  [$Zuenden$]
  Sound("Fuse");
  SetAction("Fuse");
  // Aktivator wird Besitzer
  SetOwner( GetOwner(pCaller) );
  return(1);
}
  
/* Aktivierung (durch Inhaltssteuerung) */
public func ControlDigDouble ()
{
  return(Activate());  
}

// Überprüfung der Brenndauer der Zündschnur
private func TimeCheck ()
{
  if(GetActTime() < 100+Random(50))
    return(0);
  DoExplode(18);
}
public func DoExplode(iLevel)
{
  CreateParticle("Blast", 0, 0, 0, 0, iLevel*10, RGB(255,255,255));
  Sound("Blast1");
  var obj;
  while(obj = FindObject(COWB, -iLevel*2, -iLevel*2, iLevel*4, iLevel*4, OCF_Alive, 0, 0, NoContainer(), obj))
    Punch(obj, iLevel);	  
  RemoveObject();
}

// Zündschnur-Effekte
private func Effects ()
{
  if(Random(4)) return(0);

  var x,y;
  x = Cos(GetR()+90, 6);
  y = Sin(GetR()+90, 6);

  if(Contained()) { x=0; y=0; }

  for(var amount = RandomX(1,3) ; amount ; amount--)
    CreateParticle("DynamiteSpark", x, y, x*3+RandomX(-10,10), y*3+RandomX(-10,10), 20, RGBa(255,255,0,100));
}
  
// Aktivierung durch Explosion von außen
protected func Incineration ()
{
  DoExplode(14);
}
