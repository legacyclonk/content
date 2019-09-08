/*-- Toter Wipf --*/

#strict

/* Verwesung */

private func Decaying()
{
  DoCon(-4);
}
  
/* Aktionen von außen */  
  
public func Eat(pBy)
{
  pBy->~Feed(10);
  RemoveObject();
  return(1);
}

/* Zerlegen */

public func Activate(pClonk) {
  [$TxtEmbowel$|Image=KNFE]
  // Der Clonk soll dieses Objekt zerlegen, wenn er kann
  return(ObjectSetAction(pClonk,"Embowel",this()));
}

/* Wird zerlegt */

public func Embowel() {
  // In Bestandteile zerfallen
  Split2Components(this());
  return(1);
}
