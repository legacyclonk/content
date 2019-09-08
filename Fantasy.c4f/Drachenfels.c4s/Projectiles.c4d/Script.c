// Projektile mit Respawn
// Erzeugt Projektile nach Abwurf im KI-Clonk neu

#strict

#appendto ROCK
#appendto FLNT
#appendto STFN
#appendto FBMP

local re_iIdleTime;   // Anzahl Timeraufrufe, die das Objekt herum lag
local re_iRespawns; // Anzahl der verbleibenden Respawns - negativ für \infty

static const _PRO_ProjectileRespawnTime = 63; // Frames
static const _PRO_ProjectileFadeoutTime = 6; // Sekunden

public func SetRespawns(int iToVal) { re_iRespawns = iToVal; return(true); }

protected func Departure(object pOfContainer)
  {
  if (!pOfContainer || !re_iRespawns) return(_inherited(pOfContainer));
  // Respawn initiieren
  Schedule(Format("g_CreateProjectile(%i,%d)", GetID(), re_iRespawns-1), _PRO_ProjectileRespawnTime, 1, pOfContainer);
  // Selbst nicht mehr respawnen
  re_iRespawns = 0;
  // Automatisches Ausfaden initiieren
  ScheduleCall(0, "Re_FadeOut", 38, 999999);
  return(_inherited(pOfContainer));
  }
  
protected func Re_FadeOut()
  {
  // Ausfaden, wenn das Objekt lange genug offen herumlag
  if ((GetOCF() & OCF_HitSpeed2) || Contained()) return(re_iIdleTime=0);
  if (++re_iIdleTime <= _PRO_ProjectileFadeoutTime) return();
  FadeOut(5, 32);
  }
  
global func g_CreateProjectile(id idItem, int iRespawns)
  {
  if (!this() || !GetAlive()) return();
  var pObj = CreateContents(idItem);
  pObj->~SetRespawns(iRespawns);
  if (pObj == Contents()) ShiftContents(); // Vorheriges Objekt wählen
  return(pObj);
  }
  