/*-- Kraftwerk --*/

#strict

#include DOOR
#include BAS4

/* Initialisierung */

protected func Initialize()
{
  // Netterweise gibt's zu Beginn einmal Kohle
  CreateContents(COAL);
}

/* Eingangssteuerung */

private func SoundOpenDoor()
{
  Sound("GateOpen");
}

private func SoundCloseDoor()
{
  Sound("GateClose");
}
  
protected func Collection(object pObj)
{
  Sound("Clonk");
}

/* Produktion */

private func Burning()
{
  // Rauch
  Smoke(-15,-20,12);
  Smoke(-1,-23,8);
  // Energieerzeugung
  DoEnergy(+25);
  // Weiter
  if(GetActTime() < GetBurnTime()) return(1);
  // Fertig
  SetAction("Idle");
}

/* Inhaltsüberprüfung */

private func ContentsCheck()
{
  // Noch aktiv
  if(!ActIdle()) return(1);
  var i = -1, obj, def;
  while(obj = Contents(++i) )
  {
    if(obj->~IsLorry() )
    {
      SetCommand(obj, "Exit");
    }
  }

  // Erstes Inhaltsobjekt prüfen
  obj = Contents(0);
  def = GetID(obj);
  
  // Alles außer Brennmaterial raus
  if(def != COAL && def != OBRL && def != WOOD)
    SetCommand(obj, "Exit");
    
  // Wird Energie benötigt
  if(!CheckEnergyNeedChain() ) return(1);
  
  // Holz?
  if(ContentsCount(WOOD) > 1)
    return(BurnWood() );
    
  // Öl?
  if(ContentsCount(OBRL) )
    return(BurnOil() );
    
  // Kohle
  if(!(obj = FindContents(COAL)) ) return(1);
  RemoveObject(obj);
  SetAction("Burning");
}

private func BurnWood()
{
  // Zwei Holz löschen
  for(var i = 0; i < 2; ++ i)
    RemoveObject(FindContents(WOOD) );

  // Los gehts
  SetAction("Burning");
}

private func BurnOil()
{
  // Fass entleeren
  var barrel = FindContents(OBRL);
  if(!barrel) return(0);
  ChangeDef(BARL, barrel);
  
  // Los geth.
  SetAction("Burning");
}

/* Brenndauer */
private func GetBurnTime() { return(2000); }
