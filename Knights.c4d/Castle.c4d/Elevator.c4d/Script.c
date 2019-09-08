/*-- Burgfahrstuhl --*/

#strict
#include CPW1 // Burgteilfunktion
#include CXEC // Energieversorgung

local is_shaft, type_set, pCase;

/* Ausrichtung von Burgbauteilen */

public func CastlePartBasement() { return(BASE); }

/* Forschung */

public func GetResearchBase() { return(CPW1); }

/* Grundgerüst ist der Aufzugsschacht */
public func Construction()
{
  SetGraphics("Shaft");
  return(_inherited());
}

public func RecheckBasement() { return(0); }

protected func Initialize()
{
  _inherited();
  if (!type_set)
  {
    SetGraphics();
    // Fahrstuhlkorb erzeugen
    SetAction("LiftCase", pCase = CreateObject(CPEC, +0, +37, GetOwner()));
    LocalN("RangeTop", pCase) = GetY() + 23;
  }
  // An Aufzuege anpassen
  CheckAdjust();
}

/* Anpassung */

public func CheckAdjust()
{
  // Aufzuege unten und oben suchen
  var cpel_down = FindCastlePart(0, GetObjHeight() / 2);
  var cpel_up = FindCastlePart(0, -GetObjHeight() / 2);
  // Und anpassen
  if (cpel_down)
    if (GetID(cpel_down) == CPEL)
      AdjustTo(COMD_Down(), cpel_down);
  if (cpel_up)
    if (GetID(cpel_up) == CPEL)
      AdjustTo(COMD_Up(), cpel_up);
}

/* Schachterzeugung */
    
public func AdjustTo(int iDir, object pToObj) 
{
  // Ausrichtung nach unten
  if (iDir==COMD_Down()) 
  {
    // Das untere Burgteil ist ein Fahrstuhl 
    // -> untere Burgteil muss zum Schacht werden und dieses Burgteil zum Fahrstuhl
    if ((GetID(pToObj) == CPEL) && !LocalN("is_shaft", pToObj)) 
    {
      // ggf. Stromleitung nach oben legen - leider gibt es kein Find_ActionTarget2
      for (var pwrline in FindObjects(Find_ID(PWRL)))
        if (GetActionTarget(0, pwrline) == pToObj)
          SetActionTargets(this(), GetActionTarget(1, pwrline), pwrline);
        else if (GetActionTarget(1, pwrline) == pToObj)
          SetActionTargets(GetActionTarget(0, pwrline), this(), pwrline);
      // Umwandeln
      ToShaft(pToObj);
      ToElevator(this());
    }
    // Das untere Burgteil ist ein Schacht
    // -> dieses Burgteil muss zum Fahrstuhl werden
    if ((GetID(pToObj) == CPEL) && LocalN("is_shaft", pToObj)) 
    	ToElevator(this());
  }
  // Ausrichtung nach oben
  if (iDir == COMD_Up())
    // oberes Burgteil ist ein Fahrstuhl oder Schacht
    // -> Dieses Burgteil muss ein Schacht sein
    if (GetID(pToObj) == CPEL) 
    	ToShaft(this());
}

public func ToShaft(object pObj) 
{
  if (!LocalN("is_shaft", pObj) ) 
  {
    ObjectSetAction(pObj, "Idle");
    // Fahrstuhlkorb entfernen
    LocalN("oElev", GetActionTarget(0, pObj)) = 0;
    RemoveObject(GetActionTarget(0, pObj));
    // Solidmask entfernen
    SetSolidMask(42,13,40,5,0,13,pObj);
    // umwandeln
    SetGraphics("Shaft",pObj);
    LocalN("is_shaft",pObj)=1;
    LocalN("type_set",pObj)=1;
  }
}

public func ToElevator(object pObj) 
{
  if(LocalN("is_shaft", pObj)) 
  {
    // Fahrstuhlkorb erzeugen
    ObjectSetAction(pObj, "LiftCase", pCase = CreateObject(CPEC,+0,+37,GetOwner()));
    // Solidmask erzeugen
    SetSolidMask(0,14,40,5,0,14,pObj);
    // umwandeln
    SetGraphics(0,pObj);
    LocalN("is_shaft",pObj)=0;
    LocalN("type_set",pObj)=1;
  }
}
    
/*-- Fahrstuhl -- */

public func RemoveCase()
{
  // Komplett, aber kein Fahrstuhlkorb: reduzieren für Wiederaufbau/Reparatur
  if (GetCon() == 100)
  {
    SetAction("Idle");
    DoCon(-1);
  }
  return(1);
}

// Zum externen Aufruf von Szenarienscripten
public func CreateShaft(int iLength)
{
  // SolidMask des Fahrstuhlkorbs wegschieben
  var oCase, iCaseY;
  if (oCase = GetActionTarget())
  {
  	iCaseY = oCase->GetY();
  	SetPosition(oCase->GetX(), GetY() - 20, oCase);
  }
  // Schacht freigraben
  DigFreeRect(GetX() - 12, GetY() + 25, 24, iLength);
  // Fahrstuhlkorb zurückschieben
  if (oCase) 
  	SetPosition(oCase->GetX(), iCaseY, oCase);
  return(1);
}

public func Incineration() 
{ 
	if (pCase) RemoveObject(pCase); 
	return(_inherited());
}

public func IncinerationEx()
{ 
  if (pCase) RemoveObject(pCase); 
  return(_inherited());
}

public func Destruction() 
{ 
  if (pCase) RemoveObject(pCase); 
  return(_inherited());
}
