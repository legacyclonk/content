/*-- Werkstatt --*/

#strict

/* Stellt die Grundfunktionalität für Werkstätten zur Verfügung. */

#include CXEC
#include BAS7
#include DOOR


/* Produkteigenschaften (überladbar) */
func ProductType() { return(C4D_Vehicle()); }
func ProductCondition() { return(); }

/* Produktion */

public func IsProducerOf(caller, def) 
{
  if (!(GetCategory (0, def) & ProductType())) return (0);
  if (!IsBuilt ()) return (0);
  if (!GetPlrKnowledge (GetOwner (caller), def)) return (0);
  if (ProductCondition ())
    if (!DefinitionCall (def, ProductCondition ()))
      return (0);
  //Suche nach besserer...
  if (NeedsEnergy ())
    if (FindSuppliedObjectCloseTo (caller)) return (0);
  return(1);
}

func FindSuppliedObjectCloseTo (obj, def) {
  var obj2;
  if (!def) def = GetID ();
  while (obj2 = FindObject (def, (GetX (obj) - 1000) - GetX (), (GetY (obj) - 500) - GetY (), 2000, 1000,
    OCF_Fullcon (), 0, 0, 0, obj2))
      if (FindObject (PWRL, 0,0,0,0, 0,"Connect",obj2))
        return(obj2);
  return(0);
}

public func HowToProduce (clonk, def) {
  if(NeedsEnergy())
  { 
    AddCommand (clonk, "Call", this (), def, 0, 0, 0, "HowToProduce");
    AddCommand (clonk, "Energy", this ());
    return(1);    
  }
  AddCommand (clonk, "Wait", 0, 0, 0, 0, 10);
  AddCommand (clonk, "Call", this (), def, 0, 0, 0, "StartProduction");
  AddCommand (clonk, "Enter", this ());
  return(1);
}

private func MenuProduction(pCaller) {
  // Menü erzeugen und mit Bauplänen des Spielers füllen
  CreateMenu(CXCN,pCaller,this(),1,"$NoPlrKnowledge$");
  for(var i=0,idKnowledge; idKnowledge=GetPlrKnowledge(GetOwner(pCaller),0,i,ProductType ()); ++i)
  {
    if(ProductCondition())
      if(!DefinitionCall(idKnowledge, ProductCondition()))
        continue;
    AddMenuItem("$Construction$: %s", "SelectProduction", idKnowledge, pCaller, 0, pCaller);
  }
  return(1);
}

public func SelectProduction(idType,pWorker,bSpecial2) {
  // Mit der Arbeit beginnen
  AddCommand(pWorker,"Call",this(),idType,bSpecial2,0,0,"StartProduction", 0, 1);
  // vorher die Werkstatt betreten
  AddCommand(pWorker,"Enter",this());
  return(1);
}
  
public func StartProduction(pWorker,idType,bSpecial2) {
  var pToBuild;
  // Nach unfertigem Objekt zum Weiterbauen suchen
  pToBuild=FindIncompleteContents(idType);
  // Sonst neues Objekt erzeugen
  if(!pToBuild)
    if(!(pToBuild=CreateConstruction(idType,0,0,GetOwner(),1))) return(0);
  // Erzeugtes Objekt in die Werkstatt versetzen
  if (Contained(pToBuild)!=this()) 
    Enter(this(),pToBuild);
  // Produkt benachrichtigen
  pToBuild->~OnStartProduction(this);
  // Baubefehl
  AddCommand(pWorker,"Build",pToBuild, 0,0,0,0,0,0, 3);
  //melden wenn er fertig ist (damit er ggf. Neues beginnen kann)
  if(bSpecial2)
    AppendCommand(pWorker,"Call",this(),idType,bSpecial2,0,0,"ProductionComplete", 0, 1);
  return(1);
}

public func ProductionCompleteFailed(pWorker,idType,bSpecial2) {
  //Aktion "Build" nicht ausführbar (Rohstoffe fehlen)
  //Um an die "x braucht y"-Meldung zu kommen, noch einmal das Kommando "Build" ausführen
  //damit dieses fehlschlägt und die Meldung schließlich ausgegeben wird. Die Meldung kommt
  //bei Dauerproduktion also etwa 2sec später da der Clonk das Bauen "doppelt" versucht
  AddCommand(pWorker,"Build",FindIncompleteContents(idType)); 
  return(1);
}

public func ProductionComplete(pWorker,idType,bSpecial2) {
  //Dauerproduktion nicht an: beenden.
  if(!bSpecial2) return(0);
  //sonst weiter... (noch ne sec warten)
  AppendCommand(pWorker,"Wait",0,0,0,0,35);
  AppendCommand(pWorker,"Call",this(),idType,bSpecial2,0,0,"StartProduction");
  return(1);
}

/* Kontext */

public func ContextConstruction(pCaller) {
  [$Production$|Image=CXCN|Condition=IsBuilt]
  return(MenuProduction(pCaller));
}

protected func IsBuilt() {
  return(GetCon() >= 100);
}

/* Steuerung */

protected func ContainedUp(pCaller) 
{
  [$Production$|Image=CXCN]
  return(MenuProduction(pCaller));
}

/* Aktivität */

private func CheckBuild() {
  // TimerCall: die Werkstatt startet ihre eigene Build-Aktion, sobald
  // jemand in der Werkstatt arbeitet. Die Arbeit in einem Gebäude schreitet
  // nur voran, wenn das Gebäude dies durch eine eigene Build-Aktion unterstützt.
  var bWorkingClonk=IsWorking();
  var bBuildingAction=(GetAction()S="Build");
  if(bWorkingClonk  && !bBuildingAction) if (ActIdle()) SetAction("Build");
  if(!bWorkingClonk && bBuildingAction ) {
    SetAction("Idle");
    EnergyCheck(0);
  }
  return(1);
}

private func IsWorking() {
  // Arbeitet jemand in der Werkstatt?
  if (!Contents()) return(0);
  return(FindObject(0,0,0,0,0,0,"Build",0,this()));
}

private func Smoking() {  
  if (GetPhase()%3) return(1);
  if (Random(6)) Smoke(+16,-20,8);
  if (Random(8)) Smoke(10,-24,5+Random(3));
  return(1);
}


/* Hilfsfunktionen */

private func FindIncompleteContents(idSearched) {
  for(var i=0,pContent; pContent=Contents(i); ++i)
    if(GetID(pContent)==idSearched)
      if(GetCon(pContent)<100)
        return(pContent);
  return(0);
}
