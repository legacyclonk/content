/*-- Verbindung --*/

#strict
#include GOAL

/* Lokale Variablen */

local goalFulfilled;
local pStartPole,pEndPole;

public func Initialize()
{
  SetAction("Initializing");  //Genug Zeit um ggf. eigene Masten zu definieren
  return(_inherited());
}

public func Initialized()
{
  // Masten schon definiert
  if(pStartPole && pEndPole) return();
  // Die Masten stehen jeweils 20 Pixel vom Kartenrand entfernt, an der obersten möglichen Position
  var Sy, Ey;
  Sy = GetPolePosition(20); // <- PolePosition, harhar..
  Ey = GetPolePosition(LandscapeWidth()-20);
  // Keine ordentliche Position
  if(Sy == -1 && !pStartPole) return(NoGood());
  if(Ey == -1 && !pEndPole)   return(NoGood());
  // Masten erstellen
  if(!pStartPole)
    {
    pStartPole = CreateObject(TEL1, AbsX(20), AbsY(Sy), -1);
    AddEffect("NoDmg", pStartPole, 101, 0, 0, GetID());
    pStartPole->BorderPole(0);
    }
  if(!pEndPole)
    {
    pEndPole = CreateObject(TEL1, AbsX(LandscapeWidth()-20), AbsY(Ey), -1);
    AddEffect("NoDmg", pEndPole, 101, 0, 0, GetID());
    pEndPole->BorderPole(1);
    }
  // Alles ok
}

public func GetPolePosition(int iX) // Kann öffentlich benutzt werden, um Masten aufzustellen
{
  var y = -1;
  // Von oben nach unten durch die Karte wandern und schauen, ob dort Boden ist
  for(var i = 0 ; i < LandscapeHeight() ; i++)
    if(GBackSolid(AbsX(iX), AbsY(i)) && y < 0)
      y = i;
  return(y);
}

private func NoGood()
{
  // Wird benutzt, wenn keine Start-/Endpfosten gebaut werden konnten
  Log("$NoGood$");
  RemoveObject();
}

public func IsFulfilled()
{
  if(pStartPole&&pEndPole)
    return(pStartPole->IsConnectedTo(pEndPole));
  return();
}

protected func Activate(iPlayer)
{
  // Erfüllt
  if (IsFulfilled()) 
    return(MessageWindow("$MsgGoalFulfilled$",iPlayer));
  // Noch nicht erfüllt
  else
    return(MessageWindow("$MsgGoalUnfulfilled$",iPlayer)); 
}

public func FxNoDmgDamage(pTarget,iEffectNum,iDmgEngy,iCause)
{
  return();
}

public func FxNoDmgEffect(szNewEffect)
{
  // Feuer abblocken
  if (WildcardMatch(szNewEffect, "*Fire*")) return(-1);
  // Alles andere ist OK
  return();
}
