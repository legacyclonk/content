/*-- Dynamo --*/

#strict
#include BAS2

/* Steuerung */

public func ContextProduction(pCaller)
{
  [$TxtProduce$|Image=RSR1|Condition=IsBuilt]
  return(StartWork(pCaller));
}

protected func IsBuilt() {
  return(GetCon() >= 100);
}

func ContainedThrow() { return(1); }
  
public func ControlDigDouble(pCaller)
{
  [$TxtProduce$|Image=RSR1]
  return(StartWork(pCaller));
}

public func ContainedDown(pCaller)
{
  [$TxtCancelproduction$|Image=RSR2]
  WorkCancel(pCaller);
  return(1);
}

func StartWork(pWorker)
  {
  if(GetID(pWorker) != CLNK) return(Message("$TxtClonksOnly$", pWorker, GetName(0, GetID(pWorker))));
  if(GetAction() eq "Working") return(Message("$TxtAlreadyInUse$", pWorker)); 
  // Arbeit beginnen
  AddCommand(pWorker,"Call",this(),0,0,0, 0, "Work", 0, 1);  
  // Vorher: Position einnehmen
  AddCommand(pWorker,"MoveTo",0,GetX()+10,GetY());
  }

func Work(pWorker)
  {
  // Clonk einsaugen
  Enter(this(),pWorker);
  // Besitzer setzen für Farbe des animierten Clonk
  SetOwner(GetOwner(pWorker));
  // Farbe nach Clonkfarbe richten
  SetColorDw(GetColorDw(pWorker));
  // Aktion starten
  SetAction("Working");
  return(1);
  }

func Working()
  {
  // Ich bin ja ein vielbeschäftigter Timer
  if(!FindContents(CLNK)) return(SetAction("Idle"));
  CheckForSolid();
  return(DoEnergy(+20));
  }

func CheckForSolid()
  {
  if(GBackSemiSolid()) return(WorkCancel());
  }

private func WorkCancel(pWorker)
  {
  if(!pWorker) pWorker=FindContents(CLNK);
  // Arbeit abbrechen
  if (GetAction() eq "Working") SetAction("Idle");
  Exit(pWorker,10,10);
  SetDir(DIR_Left(), pWorker);
  // Fertig
  return(1);
  }