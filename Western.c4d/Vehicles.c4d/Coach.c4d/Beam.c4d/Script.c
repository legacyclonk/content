/*--- Deichsel ---*/

#strict

/* Locals */

// 0,1: Farben
// 2,3: Verbindungsvertices

protected func Initialize()
{
  // Local 0 und 1 sind die Lininenfarben
  SetLocal(0,66); 
  SetLocal(1,66);  
  // Die ersten beiden Vertices auf aktuelle Position legen
  SetVertex(0,0,GetX()); SetVertex(0,1,GetY());
  SetVertex(1,0,GetX()); SetVertex(1,1,GetY());
}

public func Connect(pTarget1, pTarget2)
{
  SetAction("Connect",pTarget1,pTarget2);
  return(1);
}

private func Connecting()
{
  var pTarget1 = GetActionTarget(0), pTarget2 = GetActionTarget(1);
  if (!((pTarget1->~IsHorse() && pTarget1->GetActionTarget() == pTarget2 && CheckProcedure(pTarget1)) ||
        (pTarget2->~IsHorse() && pTarget2->GetActionTarget() == pTarget1 && CheckProcedure(pTarget2))))
  {
    RemoveObject();
    return(0);
  }
  return(1);
}

private func CheckProcedure(pTarget)
{
  if(GetProcedure(pTarget) ne "PULL" && GetProcedure(pTarget) ne "NONE" && GetProcedure(pTarget)) return;
  return 1;
}
