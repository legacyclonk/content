/*-- Vogelei --*/

#strict

local iBreedTime;

/* TimerCall */

protected func Breed()
{
  if(GetAction()S="Nest") iBreedTime++;
  if(iBreedTime>=50) DoBreak();
}
  
/* Einwirkungen */  

protected func Hit()
{
  if (Random(2)) DoBreak();
}

protected func Damage()
{
  return(DoBreak());
}

protected func Entrance()
{
  SetAction("Idle");
  SetActionTargets();
  iBreedTime=0;
  if(FindContents(BIRD)) RemoveObject(FindContents(BIRD));
}

/* Aktionen */
  
private func DoBreak()
{
  if (GetAction()S="Break") return(0);
  Sound("EggBreak");
  SetAction("Break");
  if(iBreedTime<25) return(0); // Noch nicht richtig ausgebildeter Vogel
  if (GBackSolid()) return(0);
  var pChild = CreateConstruction(BIRD, 0, 0, -1, 40);
  SetOwner(-1, pChild);
  pChild->~Birth();
}
  
private func Destroy()
{
  RemoveObject();
}

public func Activate(object pClonk)		// essen
{
  [$TxtEat$]
  Eat(pClonk);
  return(1);
}

protected func Eat(object pClonk)
{
    pClonk->~Feed(25);
    pClonk->DoBreath(100);
    RemoveObject();
    return(1);
}

public func GetFeedValue() { return(25); }
