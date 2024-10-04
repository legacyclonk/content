/*-- Lasergranate --*/

#strict

// Ist eine Granate
#include GREN

local alreadyhit; // was wir schon getroffen haben
local angle; // momentaner Schusswinkel

func HitObject()
{
  RemoveEffect("Grenade", this(),0,1);
  RemoveEffect("HitCheck", this(),0,1);

  CastLasers();
}

func CastLasers()
{
  alreadyhit = CreateObject(L_OS,0,0,NO_OWNER);
  angle = Random(360);
	
  Sound("LaserGrenade");
  SetSpeed(0,0);
  SetAction("Float");
}

func Detonating()
{
  CreateObject(LASR, 0,0, GetOwner())->Set(angle, 8, 200, 20, this(), 0, 3);
  angle += RandomX(20,50) + 180;
}

func Detonated()
{
  RemoveObject();
}

func LaserStrike(object pObj)
{
  if(!pObj)
    return();

  // Objekt wurde schon mal getroffen -> weniger schaden
  if(alreadyhit->AddItem(pObj))
    DoDmg(iDamage, DMG_Energy, pObj);
  else
    DoDmg(iDamage/10, DMG_Energy, pObj);
  
  return(1);
}

func Destruction()
{
	if(alreadyhit) RemoveObject(alreadyhit);
}

func ExplodeDelay() {   return(45); }
