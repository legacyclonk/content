/*-- Nest --*/

#strict

local pTree;

protected func Initialize()
{
  SetAction("Be");
  return(1);
}

private func Check4Tree()
{
  // Baum noch da?	
  if(pTree)	
   if(!OnFire(pTree))  
    if(!(GetCategory(pTree) & C4D_Vehicle))
      return(1);      	    
  SetAction("Decay");
  SetCategory(C4D_Vehicle());
  // Ei benachrichtigen
  var pEgg;
  if(pEgg = FindObject(BEGG, 0, 0, 0, 0, 0, 0, this())) pEgg->SetAction("Idle");
  return(1);
}

protected func Incineration()
{
  SetAction("Decay");
  SetCategory(C4D_Vehicle());
  // Ei benachrichtigen
  var pEgg;
  if(pEgg = FindObject(BEGG, 0, 0, 0, 0, 0, 0, this())) pEgg->SetAction("Idle");
  return(1);
}

public func RejectCollect(pObj, pID)
{
  if(pID == BIRD) return(0);
  return(1);
}

public func Breed()
{
  var pBird = FindContents(BIRD);
  if (pBird) pBird->LayEgg(); else SetAction("Be");
}

public func Ejection()
{
  SetAction("Be");	
}

private func Hit()
{
  if (GetAction()!="Decay")  SetAction("Decay");
  return(1);
}

protected func ActivateEntrance()
{
  SetEntrance(1);
  return(1);
}

private func Remove()
{
  return(RemoveObject());
}
