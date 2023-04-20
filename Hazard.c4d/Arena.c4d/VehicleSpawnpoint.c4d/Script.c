/*-- Vehicle Spawnpoint --*/

#strict

local spawnId, angle, spawnwait, spawntime;

local spawnedobj;

global func PlaceVehicleSpawnpoint(id idObj, int iX, int iY,  int timer)
{
  var spwn = CreateObject(VSPN, iX, iY, -1);
  spwn->Set(idObj, timer);
  return(spwn);
}
protected func Timer()
{
  if(!spawnId) return;

  angle += 10;
  if(angle >= 360) angle = 0;
  SetObjDrawTransform(1000, 0,0,0, 1000, Sin(angle, 8)*1000 - 2000,0, 1);
  CreateParticle("NoGravSpark", RandomX(-10,10), RandomX(10,15), 0, -5, 25, RGBa(210, 210, 255, 100), this, true);

	if(spawnwait >= spawntime)
	{
		spawnedobj = CreateObject(spawnId,0,GetDefHeight(spawnId)/2,-1);
		VehicleSpawnEffect(spawnedobj);
		spawnwait = -1;
	}
	else if(spawnwait == -1 && !spawnedobj)
	{
		spawnwait = 0;
	}
	
	if(spawnwait > -1) spawnwait+=5;
}

private func Set(id spawn, int timeToSpawn)
{
	if(!timeToSpawn) timeToSpawn = 4000; //default: ~2.5 minuten
	spawnId = spawn;
	spawntime = timeToSpawn;
	
  SetGraphics(0, 0, spawnId, 1, 4);
  SetObjDrawTransform(1000, 0,0,0, 1000, -10000,0, 1);
	spawnwait = spawntime;
}

/* Serialisierung */

public func Serialize(array& extra)
{
	extra = Format("PlaceVehicleSpawnpoint(%i,%%d+%d,%%d+%d,%%d+%d)", spawnId, GetX(), GetY(), spawntime);
	return true;
}
