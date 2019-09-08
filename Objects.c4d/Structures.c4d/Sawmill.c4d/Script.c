/*-- Sägewerk --*/

#strict

#include CXEC
#include BAS7
#include DOOR

/* Produktion */

public func IsProducerOf (object clonk, id def) {
  if (def != WOOD) return (0);
  if (!IsBuilt ()) return (0);
  if (NeedsEnergy ())
    if (FindSuppliedObjectCloseTo (clonk)) return (0);
//  if (!FindWorkTree (clonk)) return (0);
  return(1);
}

private func FindSuppliedObjectCloseTo (object obj, id def) {
  var obj2;
  if (!def) def = GetID ();
  while (obj2 = FindObject (def, (GetX (obj) - 1000) - GetX (), (GetY (obj) - 500) - GetY (), 2000, 1000,
    OCF_Fullcon (), 0, 0, 0, obj2))
      if (FindObject (PWRL, 0,0,0,0, 0,"Connect",obj2))
        return(obj2);
  return(0);
}

// Kann Holz produzieren
public func HowToProduce (object obj, id def) {
  //Eventuell erst versorgen
  if (NeedsEnergy ())
  {
    AddCommand (obj, "Call", this (), WOOD, 0, 0, 0, "HowToProduce");
    AddCommand (obj, "Energy", this ());
    return (1);
  }
  AddCommand (obj, "Wait", 0, 0, 0, 0, 0, 70);
  // Arbeitsbefehl setzen
  Production (obj);
  return (1);
}

public func Production(object clonk)
{
  // Baum suchen
  var pTree;
  if (!(pTree = FindWorkTree(clonk))) {
    // Kein geeigneter Baum (fertig)
    AddMessage("$TxtNomoresuitabletrees$",clonk);
    // Habe ich schon erwähnt, dass ich C4Command hasse?
    FinishCommand (clonk, 0, 0);
    FinishCommand (clonk, 0, 1);
    return(0); /* fail command - if only the Call-Command would check the return value... */
   }
  // Baum markieren, damit er nicht nochmal verwendet wird, falls er nicht schiebbar ist
  AddEffect ("IntSawmillTreeMarker", pTree, 1, 5000, this (), 0, clonk);
  // Baum ins Sägewerk schieben...
  AddCommand(clonk,"PushTo",pTree,0,0,this(), 500, 0,0, 3);
  // ...aber erst fällen
  if (!(GetOCF(pTree) & OCF_Grab()))
    AddCommand(clonk,"Chop",pTree, 0,0,0,0,0,0, 3);
  // Fertig
  return(1);
}

private func FindWorkTree(object clonk)
{
  // Arbeiter fällt bereits einen Baum
  if (GetAction(clonk) eq "Chop") 
    return(GetActionTarget(0, clonk));
  // Arbeiter hat bereits einen Baum angefaßt
  if (GetAction(clonk) eq "Push") 
    if (GetActionTarget(0,clonk)->~IsTree())
      return(GetActionTarget(0,clonk));
  // Baum zum Anfassen suchen
  var obj;
  if (obj = FindTreeToGrab()) return(obj);
  // Baum zum Fällen suchen
  if (obj = FindTreeToChop()) return(obj);
  // Kein Baum gefunden
  return(0);
}

private func FindTreeToChop()
{
  var obj;
  while (obj = FindObject(0,0,0,-1,-1,OCF_Chop(),0,0,0,obj))
    // Ausgewachsen
    if (GetOCF(obj) & OCF_Fullcon())
    // Im freien Bereich
    if (GetOCF(obj) & OCF_InFree())
    // Nicht markiert
    if (!GetEffectCount ("IntSawmillTreeMarker", obj))
    // Nicht zu weit weg
    if (ObjectDistance(obj) < 500)
    // Brennt nicht
    if (!OnFire(obj))
    // Baum wird nicht bereits von einem Clonk gefällt
    if (!FindObject(0,0,0,0,0,OCF_CrewMember(),"Chop",obj))
      // Passenden Baum gefunden
      return(obj);
  // Keinen passenden Baum gefunden
  return(0);
}
  
private func FindTreeToGrab()
{
  var obj;
  while (obj = FindObject(0,0,0,-1,-1,OCF_Grab(),0,0,0,obj))
    // Ist ein Baum
    if (obj->~IsTree())
    // Nicht in einem Behälter
    if (!Contained(obj))
    // Nicht markiert
    if (!GetEffectCount ("IntSawmillTreeMarker", obj))
    // Nicht zu weit weg
    if (ObjectDistance(obj) < 500)
    // Brennt nicht
    if (!OnFire(obj))
    // Steckt nicht fest
    if (!Stuck(obj))
    // Baum wird nicht bereits von einem Clonk angefaßt
    if (!FindObject(0,0,0,0,0,OCF_CrewMember(),"Push",obj))
      // Passenden Baum gefunden
      return(obj);
  // Keinen passenden Baum gefunden
  return(0);
}
  
/* Kontext */

public func ContextConstruction(object clonk)
{
  [$TxtProducewood$|Image=WOOD|Condition=IsBuilt]
  return(ProductionLoop(clonk));
}

protected func IsBuilt() {
  return(GetCon() >= 100);
}

/* Steuerung */

protected func ContainedUp(object clonk)
{
  [$TxtProducewood$|Image=WOOD]
  return(ProductionLoop(clonk));
}

public func ProductionLoop (clonk) { //Horus
  // 2.) ...danach nochmal produzieren.
  AppendCommand(clonk,"Call",this(),0,0,0,0,"ProductionLoop");
  // 1.) Erst produzieren...
  Production(clonk);
  return(1);
}

public func ProductionLoopFailed () {
  return (1); // Schweig!
}
  
/* Verarbeitung */

private func ContentsCheck()
{
  // Noch aktiv
  if (!ActIdle()) return(0);
  // Inhalts- und Energiecheck
  EnergyCheck(0);
  var obj;
  if (!(obj = ContentsNotCrewMember())) return(0);
  if (!EnergyCheck(25000)) return(0);  
  // Kann zersägt werden?
  var fSawable = GetID(obj) != WOOD && GetComponent(WOOD,0,obj) && ComponentAll(obj,WOOD) && !obj->~IsArrow();
  // Nicht zersägbare empfindliche Objekte oder Fahrzeuge über den Ausgang rauswerfen
  if(!fSawable) if(GetDefFragile(GetID(obj)) || GetCategory(obj) & C4D_Vehicle)
  {
    SetCommand(obj, "Exit");
    return();
  }
  // Objekt zerlegen
  if (fSawable)
  {
    var iWoodCount = obj->GetComponent(WOOD);
    for (var i = 0; i < iWoodCount; i++)
      CreateContents(WOOD);
    obj->RemoveObject();
  }
  // sonst einfach auswerfen
  else if(GetID(obj) != WOOD)
    return Exit(obj,-20,14,0,-1);
  // Sägen
  SetAction("Saw");
  Sound("Sawmill");
  return(1);
}

private func Sawing()
{
  // Noch nicht fertig
  if (GetActTime() < 35) return(0);
  // Fertig
  SetAction("Idle");
  // Inhalt auswerfen
  Exit(ContentsNotCrewMember(1),-20,14,0,-1);
  return(1);
}

private func SawingDust()
{
  if (!Random(5)) 
    CastParticles("Dust",4,8,-23,6,10,12);
} 

private func ContentsNotCrewMember(fWood)
{
  var obj;
  for(var i = 0; obj = Contents(i); i++)
    if(!(GetOCF(obj) & OCF_CrewMember()) && (!fWood || GetID(obj)==WOOD))
      return(obj);
  return(0);
}
