/*-- Totes Bison --*/

#strict

/* Verwesung */

protected func Decaying()
{
  DoCon(-2);
}

/* Zerlegen */

public func ControlDigDouble(pClonk) {
  [$TxtEmbowel$|Image=KNFE|Condition=GetAlive]
  // Der Clonk soll dieses Objekt zerlegen, wenn er kann
  return(ObjectSetAction(pClonk,"Embowel",this()));
}

/* Wird zerlegt */

public func Embowel() {
  // In Bestandteile zerfallen
  Split2Components(this());
  return(1);
}
