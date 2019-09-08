/*--- Kanonelkugel ---*/

#strict

local fActivated;

public func ExplodeSize() { return(25); }

protected func Hit()
{   
  Sound("RockHit*");
  // Nur wenn von der Kanone abgeschossen explodieren
  if(!fActivated) return();
  // Teile schleudern, Besitzer soll erhalten bleiben, für die Zurückverfolgung von Kills
  CastObjectsOwner(BALP, RandomX(20,30), 70, 0, 0); 
  Explode(ExplodeSize());
  return(1);
}

protected func Departure(pObj)
{
  // Von der Kanone abgeschossen -> beim Auftreffen explodieren
  if(GetID(pObj)==CCAN) fActivated = 1; 
}

protected func Entrance()
{
  // Wieder eingesammelt, nicht mehr explodieren	
  fActivated = 0; 
}

// Gleich wie CastObjects nur dass der Besitzer bleibt
private func CastObjectsOwner(idType,am,lev,x,y) 
{
  var ang, obj, xdir;
  for(var i=0;i<am;i++) 
  {
    ang=Random(360);
    obj=CreateObject(idType,x,y,GetOwner());
    SetXDir(xdir=Cos(ang,lev)+RandomX(-3,3),obj);
    SetYDir(Sin(ang,lev)+RandomX(-3,3),obj);
    SetRDir((10+Random(21))*xdir/Abs(xdir),obj);
  }
}

// Kann in der Waffenschmiede hergestellt werden
public func IsArmoryProduct() { return(1); }
