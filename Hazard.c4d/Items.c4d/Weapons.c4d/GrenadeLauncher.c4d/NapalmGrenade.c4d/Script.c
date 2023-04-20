/*-- Napalmgranate --*/

#strict

// Ist eine Granate
#include GREN

func HitObject(object pTarget)
{

  for(var i=RandomX(5,6); i>0; --i)
  {
    var obj=CreateObject(NGFR, 0, 0, GetController());
    var xdir = GetXDir()+RandomX(-10,10);
    var ydir = GetYDir()+RandomX(-10,10);

    SetSpeed(xdir, ydir, obj);
  }
	
  return(_inherited());
}

func ExplodeDelay() { return(40); }
