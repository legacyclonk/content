/*-- Automat --*/

#strict

local broken;

/* Was so passieren kann */

public func EMPShock()
{
  if(broken) return();
  // Viele bunte Dosen!
  Schedule(Format("CreateParticle(\"Tin\", -5, 12, RandomX(-10, 10), -1, 30, %d);", TinColour()), 2, 10, this());
  Schedule("CastObjects(SPRK, 1, 15, RandomX(-10, 10), RandomX(-15, 15));", 3, 15, this());
  Sound("CokeCrash");
  Sound("FreezerLoop", 0,0,0,0, -1);
  SetClrModulation(RGB(100,100,100));
  broken = true;
  return(1);
}

/* Status */

public func IsMachine() { return(true); } // Für EMP

public func TinColour() { return(RGB(255, 25, 25)); }

public func IsCraneGrabable() { return(true); }

public func CanBeRecycled() { return(2); }
