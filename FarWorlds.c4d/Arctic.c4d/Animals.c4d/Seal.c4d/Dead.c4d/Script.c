/*-- Toter Seehund --*/

#strict

/* Aktivierung */

public func Activate(pClonk) {
  [$TxtEmbowel$]
  // Der Clonk soll dieses Objekt zerlegen, wenn er kann
  return(ObjectSetAction(pClonk,"Embowel",this()));
}

/* Wird zerlegt */

public func Embowel() {
  // In Bestandteile zerfallen
  Split2Components(this());
  return(1);
}

/* Verwesung */

private func Decaying() { DoCon(-4); }