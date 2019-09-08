/*-- Hochofen --*/

#strict
#include DOOR

protected func SoundOpenDoor()  { Sound("Airlock1"); }
protected func SoundCloseDoor() { Sound("Airlock2"); }

protected func Burning() {
  var pMaterial;
  // Flackern
  if(GetPhase()>9) SetPhase(RandomX(5,8));
  // Blubbern
  if(!Random(3)) {
    Bubble(-53+Random(10),-43);
    Bubble(-37+Random(10),-43);
  }
  if(GetActTime()<150) return(1);
  // Fertig
  SetAction("Burning2");
  if(pMaterial=FindContents(ORE1))      { CreateContents(METL); }
  else if(pMaterial=FindContents(BCOR)) { CreateContents(GLAS); }
  else return();
  RemoveObject(pMaterial);
  Sound("Pshshsh");
}

private func ContentsCheck() {
  var pFuel;
  // Warten...
  if (!ActIdle()) return(1);
  // Loren rauswerfen
  for(var pLorry,i=0;pLorry=Contents(i);++i)
    if(DefinitionCall(GetID(pLorry),"IsLorry")) {
      SetCommand(pLorry,"Exit");
    }
  // kein Brennmaterial (Kohle vorziehen)
  if(!(pFuel=FindContents(COAL)))
    if(!(pFuel=FindContents(RCOR)))
      return();
  // kein zu verbrennendes Material gefunden
  if(!FindContents(ORE1))
    if(!FindContents(BCOR))
      return();

  RemoveObject(pFuel);
  SetAction("Burning");
}

public func Collection() { Sound("Clonk"); }

/* Callbacks */

public func IsDeepSeaResearch() { return(1); }

public func GetResearchBase() { return(RSU1); }
