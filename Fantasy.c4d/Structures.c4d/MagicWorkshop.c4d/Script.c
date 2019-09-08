/*-- Zauberwerkstatt --*/

#strict

#include WTWR
#include WRKS
#include BAS8

local blub;

/* Initialisierung */

// BAS8 (bzw. BAS7) delegiert Initialize nicht weiter
protected func Initialize()
{
  SetAction("Ready");
  if (GetX() < LandscapeWidth()/2) SetDir(DIR_Right());
}

/* Ueberladen von Werkstatt */

private func CheckBuild()
{
  // Energie nachfüllen
  if((++blub)%6 == 0) RefillMagic();

  var bWorkingClonk=IsWorking();
  var bBuildingAction = (GetAction() S= "Build");
  if(bWorkingClonk  && !bBuildingAction) if (ActIdle()) SetAction("Build");
  if(!bWorkingClonk && bBuildingAction )
  {
    SetAction("Ready");
    EnergyCheck(0);
  }
  return(1);
}

public func ActIdle()
{
  return(GetAction() S= "Ready");
}

/* Produkteigenschaften (überladen) */
public func ProductType () { return(C4D_Object()); }
public func ProductCondition () { return("IsMagicProduct"); }

/* Produktion */

private func MenuProduction(object pCaller)
{
  // Menü erzeugen und mit Bauplänen des Spielers füllen
  CreateMenu(GetID(), pCaller, this(),1,"$TxtNoconstructionplans$");
  for(var i=0,idKnowledge; idKnowledge=GetPlrKnowledge(GetOwner(pCaller),0,i,ProductType ()); ++i)
    if (DefinitionCall(idKnowledge, ProductCondition()))
      AddMenuItem("$TxtProductions$", "SelectProduction", idKnowledge, pCaller, 0, pCaller);
  return(1);
}


