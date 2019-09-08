/*-- Fleisch --*/

#strict 2
// Grundfunktionalität für Fleisch

protected func Hit() { Sound("WoodHit*"); }

// Essen
public func Eat(pEater) {
  if(pEater) {
    pEater->~Feed(GetFeedValue());
    // Bringt bei "Nicht Durst" kein Durst
    if(ObjectCount(NDWA))
      pEater->~Drink(this,-GetThirstValue(),1);
  }
  RemoveObject();
}

public func Activate(pClonk) {
  [$TxtEat$]
  Eat(pClonk);
  return(1);
}

// Kochen
public func Cook()
{
  if(!CanBeCooked()) return;
  ChangeDef(IDCooked());
  SetAlive(0);
  return(1);
}

// Überladungen
public func IDCooked() { return(MEAC); }
public func IsBaitMeat() { return(75); }
public func GetFeedValue() { return(15); }
public func CanBeCooked() { return(1); }
public func IsAlchemContainer() { return(true); }
public func AlchemProcessTime() { return(100); }
public func GetThirstValue()
{
  if(!this->CanBeCooked()) return(this->GetFeedValue()/2);
  return(this->GetFeedValue()*2);
}

/* Wird zerlegt */

public func Embowel() {
  // In Bestandteile zerfallen
  var obj;
  for(var i=0; i<3; i++)
  {
    obj = CreateObject(METB);
  }
  RemoveObject();
  return(1);
}

