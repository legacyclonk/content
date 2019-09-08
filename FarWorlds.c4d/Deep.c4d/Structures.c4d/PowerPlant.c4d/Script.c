#strict
#include DOOR

protected func SoundOpenDoor()  { Sound("Airlock1"); }
protected func SoundCloseDoor() { Sound("Airlock2"); }

public func Collection() { Sound("Clonk"); }

protected func Burning() {
  DoEnergy(+50);
  // Flackern
  if(GetPhase()>9) SetPhase(RandomX(5,8));
  // Blubbern
  if(!Random(3)) Bubble(-38+Random(10),-41);
  if(!Random(3)) Bubble(+28+Random(10),-41);

  if(GetActTime()<1000) return();
  SetAction("Burning2");
}

protected func ContentsCheck() {
  var pFuel;
  if(!ActIdle()) return();
  // Loren rauswerfen
  for(var pLorry,i=0;pLorry=Contents(i);++i)
    if(DefinitionCall(GetID(pLorry),"IsLorry")) {
      SetCommand(pLorry,"Exit");
    }
  if(!CheckEnergyNeedChain()) return(1);
  // kein Brennmaterial (Kohle vorziehen)
  if(!(pFuel=FindContents(COAL)))
    if(!(pFuel=FindContents(RCOR)))
      return();
  RemoveObject(pFuel);
  SetAction("Burning");
}

/* Callbacks */

public func IsDeepSeaResearch() { return(1); }

public func GetResearchBase() { return(RSU1); }
