/*-- Spawnpoint --*/

#strict

local initialized, angle, player, nocollect, spawntimer;

global func PlaceSpawnpoint(id idObj, int iX, int iY, int timer)
{
  var spwn = CreateObject(SPNP, iX, iY, -1);
  spwn->CreateContents(idObj);
  if(timer)
    spwn->LocalN("spawntimer") = timer;
  return(spwn);
}

protected func Initialize()
{
  // Standardtimer = 1000 Frames
  spawntimer = 1000;
  SetClrModulation(RGBa(0,0,0,255));
  // Arrays anlegen
  nocollect = CreateArray();
  player = CreateArray();
}

protected func Timer()
{
  // Noch nicht initialisiert?
  if(!initialized)
    // Ein Inhaltsobjekt?
    if(Contents())
      // Initialisieren
      return(Initialized());
  // Kein Inhalt?
  if(!Contents())
    {
    // Nicht mehr initialisiert
    initialized = false;
    return();
    }
  // Tolle Effekt starten
  angle += 10;
  if(angle >= 360) angle = 0;
  SetObjDrawTransform(1000, 0,0,0, 1000, Sin(angle, 8)*1000 - 2000,0, 1);
  if(Random(2)) CreateParticle("NoGravSpark", RandomX(-5,5), RandomX(5,10), 0, -5, 25, RGBa(210, 210, 255, 100));
  // Alle Timer runterzählen
  DecreaseTimer();
}

private func Initialized()
{
  // Unsichtbar werden
  SetAction("Invis");
  // Objekt als Layer auflegen
  SetGraphics(0, 0, GetID(Contents()), 1, 1);
  // Transformation
  SetObjDrawTransform(1000, 0,0,0, 1000, -5000,0, 1);
  // Sammeleffekt starten
//  AddEffect("Collect", this(), 1, 3, this());
  // Waffen-Bleiben-Regel
  if(FindObject(WPST))
  {
    for(var i=0 ; i < GetPlayerCount() ; i++)
      InitializeVisibilityFor(i);

    SetVisibility(VIS_Local() | VIS_God());
  }
  // Initialisiert
  initialized = true;
}

// aufgerufen von WPST
private func InitializeVisibilityFor(int iPlr)
{
  var player = GetPlayerByIndex(iPlr);
  if(GetPlayerType(player))
    if(!(Local(0) & 1 << player))
      Local(0) += 1 << player;
}

/* Timer runterzählen */

private func DecreaseTimer()
{
  // Waffen-Bleiben-Regel
  if(FindObject(WPST))
    {
    for(var i=0, iPlr ; i < GetPlayerCount() ; i++)
      // Spielertimer runterzählen
      {
      iPlr = GetPlayerByIndex(i);
      if(GetPlayerType(iPlr) == C4PT_Script) continue;

      if(player[iPlr])
        player[iPlr] -= 5;
      if(player[iPlr] <= 0 && nocollect[iPlr])
        {
        // Spieler kann das Objekt wieder einsammeln
        player[iPlr] = 0;
        nocollect[iPlr] = false;
        Local() += 1<<GetPlayerByIndex(i);
        }
      }
    }
  else
    {
    // Timer runterzählen
    player[0] -= 5;
    if(player[0] <= 0)
      {
      // Objekt ist wieder da
      player[0] = 0;
      nocollect[0] = false;
      SetVisibility(VIS_All());
      }
    }
}

/* Einsammel-Effekte */
/*
public func FxCollectTimer()
{
  // Ist ein Clonk hier? -> Einsammeln lassen
  var pObj;
  for(var pClonk in FindObjects(Find_AtPoint(), Find_OCF(OCF_CrewMember()), Find_OCF(OCF_CrewMember()), Find_NoContainer()))
    if(CheckCollect(GetOwner(pClonk)))
      {
      pObj = CreateContents(GetID(Contents()));
      // Kann der Clonk einsammeln?
      Collect(pObj, pClonk);
      if(Contained(pObj) == this()) RemoveObject(pObj);
      else
        {
        Sound("Grab", 0, pClonk, 0, GetOwner(pClonk)+1);
        Collected(GetOwner(pClonk));
        // Munition
        if(pObj->~IsAmmoPacket())
        	pObj->~TransferAmmo(pClonk);
        }
    }
}*/

public func RejectEntrance(object pClonk)
{
  // Objekt ist ein Clonk?
  if(!(GetOCF(pClonk) & OCF_CrewMember) && !(pClonk->~CanCollectFromSpawnpoints())) return(1);
  // Ich hab Contents?
  if(!Contents() || !initialized) return(1);
  // Darf einsammeln
  if(CheckCollect(GetOwner(pClonk), pClonk))
  {
    var pObj = CreateContents(GetID(Contents()));
    // Kann der Clonk einsammeln?
    if(pObj->~IsAmmoPacket()) {
      if(!(pObj->MayTransfer(pClonk)))
        return(1);
      if(NoAmmo()) return(1);
    }

    Collect(pObj, pClonk);
    if(Contained(pObj) == this()) RemoveObject(pObj);
    else
    {
      Sound("Grab", 0, pClonk, 0, GetOwner(pClonk)+1);
      Collected(GetOwner(pClonk), pClonk);
      // Munition
      if(pObj->~IsAmmoPacket()) {
      	  pObj->~TransferAmmo(pClonk);
      }
      // Waffe, gleich einsatzbereit!
      else if(pObj->~IsWeapon())
        DoAmmo(pObj->GetFMData(FM_AmmoID),pObj->GetFMData(FM_AmmoLoad),pObj);
    }
  }
  return(1);
}

private func CheckCollect(int iPlr, object pClonk) // Überprüft, ob ein Spieler das Objekt einsammeln darf
{
  if(!initialized) return();
  // Waffen-Bleiben-Regel
  if(FindObject(WPST))
  {
    if(GetPlayerType(iPlr) == C4PT_User)
      return(!nocollect[iPlr]);
    if(!nocollect[iPlr])
    {
      nocollect[iPlr] = CreateArray();
      return(true);
    }
    for(var pClnk in nocollect[iPlr])
      if(pClnk == pClonk)
        return(false);
    return(true);
  }
  else
    return(!nocollect[0]);
}

private func Collected(int iPlr, object pClonk) // Regelt, dass ein Spieler das Objekt einsammelt
{
  // Waffen-Bleiben-Regel
  if(FindObject(WPST))
    {
    if(GetPlayerType(iPlr) == C4PT_Script)
    {
      if(!nocollect[iPlr])
        nocollect[iPlr] = CreateArray();
      for(var i=0 ; i < GetLength(nocollect[iPlr]) ; i++)
        if(!nocollect[iPlr][i])
          break;
      nocollect[iPlr][i] = pClonk;
      ScheduleCall(this(), "AICollectTimer", spawntimer, 0, iPlr, pClonk);
      return();
    }
    // Spielertimer hochsetzen
    player[iPlr] = spawntimer;
    nocollect[iPlr] = true;
    // Nicht mehr sichtbar für den Spieler
    if(iPlr >= 0)
      if(Local() & 1<<iPlr)
        Local() -= 1<<iPlr;
    }
  else
    {
    // Timer hochsetzen
    player[0] = spawntimer;
    // Nicht mehr einsammelbar
    nocollect[0] = true;
    // Nicht mehr sichtbar
    SetVisibility(VIS_God());
    }
}

protected func AICollectTimer(int iPlr, object pClonk)
{
  for(var i=0 ; i < GetLength(nocollect[iPlr]) ; i++)
    if(nocollect[iPlr][i] == pClonk)
      nocollect[iPlr][i] = 0;
}

/* Serialisierung */

public func Serialize(array& extra)
{
	extra = Format("PlaceSpawnpoint(%i,%%d+%d,%%d+%d)", Contents(0)->GetID(), GetX(), GetY());
	return !false;
}
