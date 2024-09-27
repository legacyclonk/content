/*-- Spawnpoint --*/

#strict

local initialized, angle, spawntimer, objs, nextobj, timer;

global func PlaceBonusSpawnpoint(array ids, int iX, int iY, int timer)
{
  var spwn = CreateObject(BSPN, iX, iY, -1);
  spwn->AddBonusList(ids);
  if(timer)
    spwn->LocalN("spawntimer") = timer;
  return(spwn);
}

/* Listenverwaltung */

// Bonusliste füllen
public func AddBonusList(array ids)
{
  objs = ids;
}

public func AddBonus(id idB)
{
  objs[GetLength(objs)] = idB;
}

public func RemoveBonus(id idB)
{
  var newarray = CreateArray();
  for(var bon in objs)
    if(bon != idB)
      newarray[GetLength(newarray)] = bon;
  objs = newarray;
  if(nextobj == idB)
    GetRandomBonus();
}

/* Sonstiges */

protected func Initialize()
{
  // Standardtimer = 2500 Frames
  spawntimer = 2500;
  SetClrModulation(RGBa(0,0,0,255));
  objs = CreateArray();
}

protected func Timer()
{
  // Noch nicht initialisiert?
  if(!initialized)
    // Ein Objekt zur Verfügung?
    if(GetLength(objs))
      // Initialisieren
      return(Initialized());
  // Kein Bonus?
  if(!GetLength(objs))
    {
    // Nicht mehr initialisiert
    initialized = false;
    return();
    }
  // Tolle Effekt starten
  angle += 10;
  if(angle >= 360) angle = 0;
  SetObjDrawTransform(1000, 0,0,0, 1000, Sin(angle, 8)*1000 - 2000,0, 1);
  if(Random(2)) CreateParticle("NoGravSpark", RandomX(-5,5), RandomX(5,10), 0, -5, 25, nextobj->~Color());
  // Timer runterzählen
  if(timer) DecreaseTimer();
}

private func Initialized()
{
  // Zufallsbonus aussuchen
  GetRandomBonus();
  // Unsichtbar werden
  SetAction("Invis");
  // Transformation
  SetObjDrawTransform(1000, 0,0,0, 1000, -5000,0, 1);
  Local() = true;
  // Initialisiert
  initialized = true;
}

/* Timer runterzählen */

private func DecreaseTimer()
{
  // Timer runterzählen
  timer -= 5;
  if(timer <= 0)
  {
    // Objekt ist wieder da
    timer = 0;
    Local() = true;
    SetVisibility(VIS_All());
  }
}

public func RejectEntrance(object pClonk)
{
  // Objekt ist ein Clonk?
  if(!(GetOCF(pClonk) & OCF_CrewMember)) return(1);
  // Darf einsammeln
  if(Local())
  {
    if(nextobj->~Activate(pClonk)) {
      PlayerMessage(GetOwner(pClonk),Format("<c %x>%s</c>",nextobj->~Color(),GetDesc(0,nextobj)),pClonk);
      Sound("Grab", 0, pClonk, 0, GetOwner(pClonk)+1);
      Collected(GetOwner(pClonk));
    }
  }
  return(1);
}

private func Collected(int iPlr) // Regelt, dass ein Spieler das Objekt einsammelt
{
  // Timer hochsetzen
  timer = spawntimer;
  // Nicht mehr einsammelbar
  Local() = false;
  // Nicht mehr sichtbar
  SetVisibility(VIS_God());
  // Neuen Bonus auswählen
  GetRandomBonus();
}

private func GetRandomBonus()
{
  // nextobj belegen
  nextobj = objs[Random(GetLength(objs))];
  SetGraphics(0, 0, nextobj, 1, 1);
}
