/*-- Fleisch --*/

#strict

protected func Hit() { Sound("WoodHit*"); }

public func Eat(pEater) {
  if(pEater) pEater->~Feed(80);
  RemoveObject();
}

public func Activate(pClonk) {
  [$TxtEat$]
  Eat(pClonk);
  return(1);
}

/* Köderqualität */  
  
public func IsBait() { return(70); }

/* Action-Call */

private func Baiting() {
  // Wurde eingesammelt
  if (Contained())
    return(SetAction("Idle"));
  // Clonk-Aktion überprüfen
  if (GetAction(GetActionTarget())ne"Fish")
    return(SetAction("Idle"));
  // Sollte hier noch die Position des Clonks
  // überprüfen, falls dieser verrutscht ist.
}
