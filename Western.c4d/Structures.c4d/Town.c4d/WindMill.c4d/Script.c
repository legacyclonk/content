/*-- Windrad --*/

#strict

#include BAS2

/* Initialisierung */

protected func Initialize()
{
  CreateWindwing();
  SetAction("Base");
  CheckWindwing();
}

/* Windrad erzeugen */

private func CreateWindwing()
{
  ObjectSetAction(CreateObject(WWHL,0,0,GetOwner()),"Turn1",this());
}

/* TimerCall */

private func CheckWindwing()
{
  // Komplett, aber kein Windrad da: reduzieren für Neuaufbau/Reparatur
  if(GetCon() == 100)
    if(!FindObject(WWHL,0,0,0,0,0,0,this()) )
      return(DoCon(-1));
  // An Wind anpassen
  SetPhase(BoundBy(GetWind()/18 +5, 0, 11));
}
