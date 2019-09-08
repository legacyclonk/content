#strict
#include DOOR

protected func SoundOpenDoor()  { Sound("Airlock1"); }
protected func SoundCloseDoor() { Sound("Airlock2"); }

protected func Bubbling() {
  if(GetOCF()&OCF_Fullcon())
    if(!Random(5))
      Bubble(-20,-30);
}

protected func Completion() {
  // Hydroclonk erzeugen
  var pClonk=CreateContents(HCLK);
  MakeCrewMember(pClonk,GetOwner());
  Message("$TxtAnewCrewmember$",this());
}

protected func Ejection(object pObj) {
  // Objekte bei Austritt nach oben zur Tür verschieben
  SetPosition(GetX(pObj),GetY(pObj)-22,pObj);
}

/* Callbacks */

public func IsDeepSeaResearch() { return(1); }
