#strict

protected func Construction() {			// init
  // zufällige Grafik
  SetAction("Exist");
  SetDir(Random(4));
}

public func Place(object pTree, int iGrowth) {	// neu entstanden
  // kleben
  SetCategory(1);
  SetActionTargets(pTree);

  // Größe
  if(!iGrowth) iGrowth=Random(100)+1;
  SetCon(iGrowth);
}

public func Drop() {				// wird gepflückt (~Gegenteil von Place)
  // wenn nicht Vollständig: nö, löschen
  if(GetCon()<100) return(RemoveObject());

  SetActionTargets();
  SetCategory(16);
}

protected func RejectEntrance() {		// nicht pflücken wenn nicht reif
  if(GetCon()<100) return(1);
}

protected func Entrance() {			// gepflückt
  if(GetCategory()==1) Sound("Grab");
  Drop();
}


public func Activate(object pClonk)		// essen
{
  [$TxtEat$]
  Eat(pClonk);
  return(1);
}

protected func Eat(object pClonk)
{
    pClonk->~Feed(30);
    pClonk->DoBreath(100);
    RemoveObject();
    return(1);
}

public func GetFeedValue() { return(30); }

protected func Hit() { Sound("WoodHit*"); }
